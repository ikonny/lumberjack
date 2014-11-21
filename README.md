lumberjack
==========
Active Directory forest security testing

lumberjack implements well-known attacks against flaws in Active Directory forest and domain design:

* SIDHistory: the lack of SID filtering amongst all domains in a forest allows a rogue domain administrator to impersonate an admin in any of the other domains
* Modify membership of a universal group stored in the Global Catalog configuration partition
* Add a trust between domains through the manipulation of forest configuration data that is copied to every domain controller
* Manipulate forest schema by impersonating the Schema Master or other FSMO role

SIDHistory
----------
First described by [Aelita][AELITA] in 2002, a rogue administrator of a domain can modify the SIDHistory of any user in a controlled domain to impersonate a user form any other domain inside the forest. The control that disables this attack (SID filtering, described in [MS02-001][MS02-001]) is not effective within a forest, basically allowing any domain admin to control any of the data in the forest.

The implementation of the attack is heavily based on the [SHEdit][SHEDIT] work by Tibor Biro, however as far as we know this is the first open-source 

TODO
----
* Provide pre-compiled binaries
* Test in Windows 2012
* Test in Windows 2008
* Test in Windows 2003
* Test in Windows 2000
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

References
==========
SHEdit
http://www.tbiro.com/projects/SHEdit/index.htm

Online SidHistory attack
http://cosmoskey.blogspot.com.es/2010/08/online-sidhistory-edit-sid-injection.html

AD Partitions
http://www.selfadsi.org/adsdb.htm

Achieving Autonomy and Isolation with Forests, Domains, and Organizational Units
http://technet.microsoft.com/en-us/library/bb727032.aspx

Acknowledgements
================
The project was inspired by SHEdit - a really clean and simple implementation of the SIDHistory attack. SHEdit is only available for Windows 2000 and 2003, so our focus was to make it portable and support current versions of Windows, up to 2012.

Thanks to Robert Rowan, who provided the original victim Windows test environments to verify correct operation of the tools and without whom this project would not have been possible.

[AELITA]: http://www.decuslib.com/decus/vmslt02a/sec/nt-sidescalation-attack.txt
[MS02-001]: https://technet.microsoft.com/en-us/library/security/ms02-001.aspx
[SHEDIT]: http://www.tbiro.com/projects/SHEdit/
