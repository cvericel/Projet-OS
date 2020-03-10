CC = gcc
	CFLAGS = -Wall
DEPS = car.h ipcTools.h crossroad.h nFork.h readChar.h main.h
OBJ = car.o ipcTools.o crossroad.o nFork.o readChar.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	gcc $(CFLAGS) -o $@ $^