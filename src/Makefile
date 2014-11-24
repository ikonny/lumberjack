all:	ljack.exe

ljack.exe:	lumberjack.c libesent.a

	gcc -o ljack.exe lumberjack.c -ladvapi32 libesent.a

libesent.a: libesent.def

	dlltool -d libesent.def -l libesent.a -k
