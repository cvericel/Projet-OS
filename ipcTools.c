#include <stdio.h> 
#include "ipcTools.h" 
#include <sys/shm.h> 
#include <sys/sem.h> 
#include <stdlib.h> 
#include <string.h>

static struct sembuf s = {0, 0, 0};

/*
 * \fn int semalloc (key_t key, int valInit) 
 * \brief Creation de semaphore / mutex
 * \author cvericel
 * 
 * \param key, clef du semaphore / mutex
 * \param valInit, valeur du semaphore / mutex a la creation
 * \return semid, id du semaphore / mutex
 */
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
			perror("semctl\n");
			return -1;
		}
	}
		
    
    return semid;
}

/*
 * \fn void P(int semid, char * buf)
 * \brief Bloque sur un semaphore / mutex
 * \author cvericel
 * 
 * \param semid, id du semaphore / mutex
 */
void P(int semid)
{
	s.sem_op= -1;
	if (semop(semid, &s, 1) == -1){
		perror("semop P\n");
		exit(2);
	}
}

/*
 * \fn void V(int semid)
 * \brief Debloque un semaphore / mutex
 * \author cvericel
 * 
 * \param semid, id du semaphore / mutex
 */
void V(int semid)
{
	s.sem_op = +1;
	if (semop(semid, &s, 1) == -1){
		perror("semop V\n");
		exit(2);
	}
}

/*
 * \fn int semfree (int semid, char * buf)
 * \brief Detruie le semaphore associe a semid
 * \author cvericel
 * 
 * \param semid, id du semaphore / mutex
 * 
 * \return 0, si succees 
 * \return -1 si erreur
 */
int semfree (int semid)
{
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		perror("semctl\n");
		return -1;
	}
	else {
		return 0;
	}
}

/*
 * \fn void * shmalloc(key_t key, int size)
 * \brief Creer une memoire partagee
 * 
 * \param key, la clef de la memoire partage
 * \param size
 * 
 * \return l'adresse de la memoire partagee
 * \return 0, s'il y'a une erreur
 */
void * shmalloc(key_t key, int size) 
{
	void * res;
	int owner = 0; 
	int shmid = shmget(key, 1, 0600);
	if (shmid == -1) {
		owner = 1;
		shmid = shmget(key,size,IPC_CREAT|IPC_EXCL|0600); 
	}
	if (shmid == -1) return 0; 

	res = shmat(shmid, 0, 0); 

	if (res == (void*)-1) {
		if (owner) shmctl(shmid, IPC_RMID, 0);
		return 0; 
	}

	return res; 
}

/*
 * \fn int shmfree(key_t key, void * addr)
 * \brief Detruie la memoire partagees
 * \author cvericel
 * 
 * \param key, clef de la memoire partagee
 * \param addr, l'adresse de la memoire partagee
 * 
 * \return 0, si succee
 * \return -1, sinon
 */
int shmfree(key_t key, void * addr)
{
	int shmid = shmget(key,  0, 0600);
	if (shmid == -1) {
		perror("Shared memory space already free\n");
		return 0;
	}

	if (shmdt(addr) == -1) {
		perror("shmdt failed\n");
		return -1;
	}

	struct shmid_ds stats;
	if (shmctl(shmid, IPC_STAT, &stats) == -1) {
		perror("stats failed\n");
		return -1;
	}

	if (stats.shm_nattch > 0) {
		printf("shared memory still attached (%d links), destroy aborted\n", stats.shm_nattch);
		return -1;
	} else {
		return shmctl(shmid, IPC_RMID, 0);
	}
}
