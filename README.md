lumberjack
==========
Active Directory forest security testing

TODO
----
* JetGetDatabaseFileInfo before opening
* Check log format for pending to apply logs, e.g. to see if there are any issues with the version or similar

ESE database compatibility
==========================
ESE databases are typically non-transportable from the systems they were created on and doing so can lead to all kinds of problems and incompatibilities.

There is a different version of the ESENT.DLL library for each system: some only support certain page sizes, some only certain functionalities, some work with dfferent incompatble database versions. To be on the safe side we recommend that you use this tool on the system where the ESE database was generated; short of that try to replicate the environment as close as possible and be prepared for errors.

No matter what you do, always take a backup of the database and its supporting logs - this is not just the standard disclaimer but a must when working with ESE databases. Merely opening an ESE database file will make permanent non-recoverable changes to the database, not always in the right direction. The initial backup of the database and its logfiles is the only way to recover from unintented changed, do a backup now please.

Troubleshooting
===============
There is a wide variety of errors that the application may encounter during its operation, the application tries to provide information on how to recover from most errors, however the following are typical errors you will find and how to correct them.

The ESENTUTL tool (shipped with Windows) is an indispensable tool to troubleshoot most of the common corruption errors, a good start when encountering an error is checking the status of the database using the following command:

	ESENTUTL /mh <database_name>

This command will show several details on the database, including the status of the database and any logs associated with changes pending commiting.
	
JET_errDatabaseDirtyShutdown: Database was not shutdown cleanly
---------------------------------------------------------------
The ESE database you are working with was not shutdown properly, this means there are pending logged changes waiting to be applied to the database. To recover from this error you will need to apply the logs to the database through a recovery process using the ESENTUTL tool:

	ESENTUTL /r <three-letter prefix of the logfiles, e.b. 'edb' for instance> /o

The recovery operation may or may not be successful. If it is not successful there may be various reasons, check that:

    * You have all the logs required for a database recovery, e.g. ESENTUTL /mh <database_name> will list the logs required and committed for a recovery

	* Even if you have all the logfiles, these may be corrupt or not compatible with the local ESE library version. ESENTUTL /ml <three-letter prefix of the logfiles> will analyze the logfiles and usually assist with troubleshooting

If you are not able to recover the database you are probably only left with the option of repairing the database. Repairing the database will attempt to repair the integrity of the database file without applying the non-commited logfiles; this means that at the very least the changes in the logfiles will be ignored and in the worst case (which in our experience happens quite often) your database will remain corrupt and unusable. It is still worth a try, to perform a repair use the ESENTUTL:

	ESENTUTL /p /o ntds.dit

If you are able to repair or recover the database, please take a backiup of the resulting files. You do not want the results of all your efforts lost.
