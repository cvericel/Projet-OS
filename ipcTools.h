#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

int semalloc(key_t key, int valInit);

void P(int semid);

void V(int semid);

int semfree(int semid);

void* shmalloc(key_t key, int size);

int shmfree(key_t key);