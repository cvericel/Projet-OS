CC = gcc
	CFLAGS = -Wall
DEPS = car.h ipcTools.h crossroad.h nFork.h
OBJ = car.o ipcTools.o crossroad.o nFork.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

crossroad: $(OBJ)
	gcc $(CFLAGS) -o $@ $^