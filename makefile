all : main

car.o : car.h car.c main.h
	gcc -Wall -c -g car.c

crossroad.o : crossroad.h crossroad.c main.h
	gcc -Wall -c -g crossroad.c

ipcTools.o : ipcTools.h ipcTools.c
	gcc -Wall -c -g ipcTools.c

readChar.o : readChar.h readChar.c
	gcc -Wall -c -g readChar.c

main.o : main.c main.h car.h ipcTools.h readChar.h
	gcc -Wall -c -g main.c

main : car.o crossroad.o ipcTools.o readChar.o main.o
	gcc -o main car.o crossroad.o ipcTools.o readChar.o main.o

documentation:
	doxygen doc

clean :
	rm -f *.o main
	rm -f /html
	rm -f /latex