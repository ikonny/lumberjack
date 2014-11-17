/*
 * Information obtained from the MSDN article "Extensible Storage Engine Reference":
 *
 *  http://msdn.microsoft.com/en-us/library/gg269245(v=exchg.10).aspx
 */

 #include <tchar.h>

#ifndef _ESENT_H_
#define _ESENT_H

#ifdef __MINGW32__
#define __nullterminated
#define __in_out_opt
#define __in
#define __in_opt
#define __out
#define __out_opt
#endif

/* Extensible Storage Engine Error Codes*/
#define JET_errSuccess							0
#define JET_wrnRemainingVersions				321
#define JET_wrnUniqueKey						345
#define JET_wrnSeparateLongValue				406
#define JET_wrnExistingLogFileHasBadSignature	558
#define JET_wrnExistingLogFileIsNotContiguous	559
#define JET_wrnSkipThisRecord					564
#define JET_wrnTargetInstanceRunning			578
#define JET_wrnDatabaseRepaired 595
#define JET_wrnColumnNull 1004
#define JET_wrnBufferTruncated 1006
#define JET_wrnDatabaseAttached 1007
#define JET_wrnSortOverflow 1009
#define JET_wrnSeekNotEqual 1039
#define JET_wrnRecordFoundGreater				JET_wrnSeekNotEqual
#define JET_wrnRecordFoundLess JET_wrnSeekNotEqual
#define JET_wrnNoErrorInfo 1055
#define JET_wrnNoIdleActivity 1058
#define JET_wrnNoWriteLock 1067
#define JET_wrnColumnSetNull 1068
#define JET_wrnTableEmpty 1301
#define JET_wrnTableInUseBySystem 1327
#define JET_wrnCorruptIndexDeleted 1415
#define JET_wrnColumnMaxTruncated 1512
#define JET_wrnCopyLongValue 1520
#define JET_wrnColumnSkipped 1531
#define JET_wrnColumnNotLocal 1532
#define JET_wrnColumnMoreTags 1533
#define JET_wrnColumnTruncated 1534
#define JET_wrnColumnPresent 1535
#define JET_wrnColumnSingleValue 1536
#define JET_wrnColumnDefault 1537
#define JET_wrnDataHasChanged 1610
#define JET_wrnKeyChanged 1618
#define JET_wrnFileOpenReadOnly 1813
#define JET_wrnIdleFull 1908
#define JET_wrnDefragAlreadyRunning 2000
#define JET_wrnDefragNotRunning 2001
#define JET_wrnCallbackNotRegistered 2100
#define JET_errNoCurrentRecord -1603

/* Extensible Storage Engine Constants */

/* JET_CBTYP */
#define JET_cbtypNull 						0x00000000
#define JET_cbtypFinalize 					0x00000001
#define JET_cbtypBeforeInsert 				0x00000002
#define JET_cbtypAfterInsert 				0x00000004
#define JET_cbtypBeforeReplace 				0x00000008
#define JET_cbtypAfterReplace 				0x00000010
#define JET_cbtypBeforeDelete 				0x00000020
#define JET_cbtypAfterDelete 				0x00000040
#define JET_cbtypUserDefinedDefaultValue 	0x00000080
#define JET_cbtypOnlineDefragCompleted 		0x00000100
#define JET_cbtypFreeCursorLS 				0x00000200
#define JET_cbtypFreeTableLS 				0x00000400

/* JET_COLTYP */
#define JET_coltypNil 			0
#define JET_coltypBit 			1
#define JET_coltypUnsignedByte 	2
#define JET_coltypShort 		3
#define JET_coltypLong 			4
#define JET_coltypCurrency 		5
#define JET_coltypIEEESingle 	6
#define JET_coltypIEEEDouble 	7
#define JET_coltypDateTime 		8
#define JET_coltypBinary 		9
#define JET_coltypText 			10
#define JET_coltypLongBinary 	11
#define JET_coltypLongText 		12
#define JET_coltypSLV 			13
#define JET_coltypUnsignedLong 	14
#define JET_coltypLongLong 		15
#define JET_coltypGUID 			16
#define JET_coltypUnsignedShort 17
#define JET_coltypMax 			18

/* Error Handling Constants */
#define JET_ExceptionMsgBox	0x0001
#define JET_ExceptionNone	0x0002

/* GRBit Constants */
#if 0
-- These are not defined in MSDN
#define JET_bitTableDenyRead
#define JET_bitTableDenyWrite
#define JET_bitTableNoCache
#define JET_bitTablePermitDDL
#define JET_bitTablePreread
#define JET_bitTableReadOnly
#define JET_bitTableSequential
#define JET_bitTableUpdatable
#endif

/* Extensible Storage Engine System Parameters: Backup and restore parameters */
#define JET_paramAlternateDatabaseRecoveryPath						113
#define JET_paramCleanupMismatchedLogFiles							77
#define JET_paramDeleteOutOfRangeLogs								52
#define JET_paramOSSnapshotTimeout 									82
#define JET_paramZeroDatabaseDuringBackup							71

/* Extensible Storage Engine System Parameters: Callback parameters */
#define JET_paramDisableCallbacks 									65
#define JET_paramEnablePersistedCallbacks							156
#define JET_paramRuntimeCallback 									73

/* Extensible Storage Engine System Parameters: Database parameters */
#define JET_paramCheckFormatWhenOpenFail							44
#define JET_paramDatabasePageSize 									64
#define JET_paramDbExtensionSize									18
#define JET_paramEnableIndexChecking								45
#define JET_paramEnableIndexCleanup									54
#define JET_paramOneDatabasePerSession								102
#define JET_paramEnableOnlineDefrag									35
#define JET_paramPageFragment										20
#define JET_paramRecordUpgradeDirtyLevel							78
#define JET_paramWaypointLatency									153
#define JET_paramDefragmentSequentialBTrees							160
#define JET_paramDefragmentSequentialBTreesDensityCheckFrequency	161
#define JET_paramIOThrottlingTimeQuanta								162

/* Extensible Storage Engine System Parameters: Database cache parameters */
#define JET_paramBatchIOBufferMax									22
#define JET_paramCacheSize											41
#define JET_paramCacheSizeMin										60
#define JET_paramCacheSizeMax										23
#define JET_paramCheckpointDepthMax									24
#define JET_paramCheckpointIOMax									135
#define JET_paramEnableViewCache									127
#define JET_paramLRUKCorrInterval									25
#define JET_paramLRUKHistoryMax										26
#define JET_paramLRUKPolicy											27
#define JET_paramLRUKTimeout										28
#define JET_paramLRUKTrxCorrInterval								29
#define JET_paramStopFlushThreshold									32

/* Extensible Storage Engine System Parameters: Error handling parameters */
#define JET_paramErrorToString										70
#define JET_paramExceptionAction									98

/* Extensible Storage Engine System Parameters: Event log parameters */
// #define JET_paramEventLogCache ??? - TODO: This value is not specified in MSDN (http://msdn.microsoft.com/en-us/library/gg294086(v=exchg.10).aspx)
// #define JET_paramEventLoggingLevel ??? - TODO: This value is not specified in MSDN (http://msdn.microsoft.com/en-us/library/gg294086(v=exchg.10).aspx)
#define JET_paramEventSource 										4
#define JET_paramEventSourceKey										49
#define JET_paramNoInformationEvent									50

/* Extensible Storage Engine System Parameters: I/O Parameters */
#define JET_paramAccessDeniedRetryPeriod							53
#define JET_paramCreatePathIfNotExist								100
#define JET_paramEnableFileCache									126
#define JET_paramIOPriority											152
#define JET_paramOutstandingIOMax									30
#define JET_paramMaxCoalesceReadSize								164
#define JET_paramMaxCoalesceWriteSize								165
#define JET_paramMaxCoalesceReadGapSize								166
#define JET_paramMaxCoalesceWriteGapSize							167

/* Extensible Storage Engine System Parameters: Index Parameters */
#define JET_paramIndexTupleIncrement								132
#define JET_paramIndexTupleStart									133
#define JET_paramIndexTuplesLengthMax								111
#define JET_paramIndexTuplesLengthMin								110
#define JET_paramIndexTuplesToIndexMax								112
#define JET_paramUnicodeIndexDefault								72

/* Extensible Storage Engine System Parameters: Informational Parameters */
#define JET_paramKeyMost											134
#define JET_paramMaxColtyp											131
#define JET_paramLVChunkSizeMost									163

/* Extensible Storage Engine System Parameters: Meta Parameters */
#define JET_paramConfiguration										129
#define JET_paramEnableAdvanced										130

/* Extensible Storage Engine System Parameters: Resource Parameters */
#define JET_paramCachedClosedTables									125
#define JET_paramDisablePerfmon 									107
#define JET_paramGlobalMinVerPages									81
#define JET_paramMaxCursors											8
#define JET_paramMaxInstances										104
#define JET_paramMaxOpenTables										6
#define JET_paramMaxSessions										5
#define JET_paramMaxTemporaryTables									10
#define JET_paramMaxVerPages										9
#define JET_paramPageHintCacheSize									101
#define JET_paramPreferredMaxOpenTables								7
#define JET_paramPreferredVerPages									63
#define JET_paramVerPageSize										128
#define JET_paramVersionStoreTaskQueueMax							105

/* Extensible Storage Engine System Parameters: Temporary Database Parameters */
#define JET_paramEnableTempTableVersioning							46
#define JET_paramPageTempDBMin										19
#define JET_paramTempPath											1

/* Extensible Storage Engine System Parameters: Transaction Log Parameters */
#define JET_paramBaseName											3
#define JET_paramCircularLog										17
#define JET_paramCommitDefault										16
#define JET_paramDeleteOldLogs										48
#define JET_paramIgnoreLogVersion									47
#define JET_paramLegacyFileNames									136
#define JET_paramLogBuffers											12
#define JET_paramLogCheckpointPeriod								14
#define JET_paramLogFileCreateAsynch								69
#define JET_paramLogFilePath										2
#define JET_paramLogFileSize										11
#define JET_paramLogWaitingUserMax									15
#define JET_paramRecovery											34
#define JET_paramSystemPath											0
#define JET_paramWaitLogFlush										13


/* */

#if defined(_WIN64)
    typedef unsigned __int64 JET_API_PTR;
#elif !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
    typedef __w64 unsigned long JET_API_PTR;
#else
    typedef unsigned long JET_API_PTR;
#endif

#if !defined(_NATIVE_WCHAR_T_DEFINED)
typedef unsigned short WCHAR;
#else
typedef wchar_t WCHAR;
#endif

typedef unsigned long JET_COLUMNID;
typedef double JET_DATESERIAL;
typedef unsigned long JET_DBID;
typedef long JET_ERR;
typedef unsigned long JET_GRBIT;
typedef JET_API_PTR JET_HANDLE;
typedef JET_API_PTR JET_INSTANCE;
typedef JET_API_PTR JET_LS;
typedef JET_API_PTR JET_OSSNAPID;
typedef __nullterminated const char *  JET_PCSTR;
typedef __nullterminated const WCHAR * JET_PCWSTR;
typedef __nullterminated char *  JET_PSTR;
typedef __nullterminated WCHAR * JET_PWSTR;
typedef JET_API_PTR JET_SESID;
typedef JET_API_PTR JET_TABLEID;

#define JET_API WINAPI

/* Extensible Storage Engine Structures */

typedef struct tagJET_UNICODEINDEX {
  unsigned long lcid;
  unsigned long dwMapFlags;
} JET_UNICODEINDEX;

typedef struct {
  JET_COLUMNID columnid;
  void* pvData;
  unsigned long cbData;
  unsigned long cbActual;
  JET_GRBIT grbit;
  unsigned long ibLongValue;
  unsigned long itagSequence;
  JET_COLUMNID columnidNextTagged;
  JET_ERR err;
} JET_RETRIEVECOLUMN;

typedef struct {
  unsigned long cbStruct;
  unsigned long ibLongValue;
  unsigned long itagSequence;
} JET_SETINFO;

// ??? API

JET_ERR JET_API JetInit(
  __in_out_opt  JET_INSTANCE* pinstance
);

JET_ERR JET_API JetSetSystemParameter(
  __in_out_opt  JET_INSTANCE* pinstance,
  __in          JET_SESID sesid,
  __in          unsigned long paramid,
  __in          JET_API_PTR lParam,
  __in_opt      JET_PCSTR szParam
);

JET_ERR JET_API JetOpenTable(
  __in          JET_SESID sesid,
  __in          JET_DBID dbid,
  __in          const TCHAR* szTableName,
  __in_opt      const void* pvParameters,
  __in          unsigned long cbParameters,
  __in          JET_GRBIT grbit,
  __out         JET_TABLEID* ptableid
);

JET_ERR JET_API JetSetCurrentIndex(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __in_opt      const TCHAR* szIndexName
);

JET_ERR JET_API JetRetrieveColumns(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __in_out_opt  JET_RETRIEVECOLUMN* pretrievecolumn,
  __in          unsigned long cretrievecolumn
);

JET_ERR JET_API JetMove(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __in          long cRow,
  __in          JET_GRBIT grbit
);

JET_ERR JET_API JetBeginTransaction(
  __in          JET_SESID sesid
);

JET_ERR JET_API JetPrepareUpdate(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __in          unsigned long prep
);

JET_ERR JET_API JetSetColumn(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __in          JET_COLUMNID columnid,
  __in_opt      const void* pvData,
  __in          unsigned long cbData,
  __in          JET_GRBIT grbit,
  __in_opt      JET_SETINFO* psetinfo
);

JET_ERR JET_API JetUpdate(
  __in          JET_SESID sesid,
  __in          JET_TABLEID tableid,
  __out_opt     void* pvBookmark,
  __in          unsigned long cbBookmark,
  __out_opt     unsigned long* pcbActual
);

JET_ERR JET_API JetCommitTransaction(
  __in          JET_SESID sesid,
  __in          JET_GRBIT grbit
);

JET_ERR JET_API JetCloseDatabase(
  __in          JET_SESID sesid,
  __in          JET_DBID dbid,
  __in          JET_GRBIT grbit
);

JET_ERR JET_API JetEndSession(
  __in          JET_SESID sesid,
  __in          JET_GRBIT grbit
);

JET_ERR JET_API JetDetachDatabase(
  __in          JET_SESID sesid,
  __in          const TCHAR* szFilename
);

JET_ERR JET_API JetTerm(
  __in          JET_INSTANCE instance
);

JET_ERR JET_API JetGetSystemParameter(
  __in          JET_INSTANCE instance,
  __in          JET_SESID sesid,
  __in          unsigned long paramid,
  __in_out_opt  JET_API_PTR* plParam,
  __out_opt     JET_PSTR szParam,
  __in          unsigned long cbMax
);

#if 0

/* Extensible Storage Engine Callback Functions */

JET_ERR JET_API* JET_CALLBACK(
  [in]                 JET_SESID sesid,
  [in]                 JET_DBID dbid,
  [in]                 JET_TABLEID tableid,
  [in]                 JET_CBTYP cbtyp,
  [in, out]            void* pvArg1,
  [in, out]            void* pvArg2,
  [in]                 void* pvContext,
  [in]                 JET_API_PTR ulUnused
);

void * JET_API JET_PFNREALLOC(
  [in]                 void* pvContext,
  [in]                 void* pv,
  [in]                 unsigned long cb
);

JET_ERR JET_API JET_PFNSTATUS(
                       JET_SESID  sesid,
                       JET_SNP snp,
                       JET_SNT snt,
                       void* pv
);

#endif

#endif
