#include "ipcTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct sembuf s = {0, 0, 0};

int semalloc (key_t key, int valInit)
{
    int semid;
	semid = semget(key, 1, 0);

	if (semid == -1) {
		semid = semget(key, 1, IPC_CREAT|0600);
		if (semid == -1) {
			perror("semget\n");
			return -1;
		}
		if (semctl(semid, 0, SETVAL, valInit) == -1) {
			// On détruie le semaphore
			semfree(semid);
			perror("semctl\n");
			return -1;
		}
	}
		
    
    return semid;
}

void P(int semid)
{
	s.sem_op= -1;
	if (semop(semid, &s, 1) == -1){
		perror("semop P\n");
		exit(2);
	}
}

void V(int semid)
{
	s.sem_op = +1;
	if (semop(semid, &s, 1) == -1){
		perror("semop V\n");
		exit(2);
	}
}

int semfree (int semid)
{
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		perror("semfctl\n");
		return -1;
	}
	else {
		return 0;
	}
}

void * shmalloc(key_t key, int size) {
    void * res;
	int shmid = shmget(key, size, IPC_CREAT|IPC_EXCL|0600);
	if (shmid == -1)
		return 0;
	res = shmat(shmid,0,0);
	if (res == (void*)-1) {
		shmfree(shmid);
		return 0;
	}
	return res;
}

int shmfree(key_t key)
{
	int shmid = shmget(key,  1, 0);
	return shmctl(shmid, IPC_RMID, 0);
}
