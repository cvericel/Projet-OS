#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

/*
 * \fn int semalloc (key_t key, int valInit) 
 * \brief Creation de semaphore / mutex
 * \author cvericel
 * 
 * \param key, clef du semaphore / mutex
 * \param valInit, valeur du semaphore / mutex a la creation
 * \return semid, id du semaphore / mutex
 */
int semalloc(key_t key, int valInit);


/*
 * \fn void P(int semid, char * buf)
 * \brief Bloque sur un semaphore / mutex
 * \author cvericel
 * 
 * \param semid, id du semaphore / mutex
 */
void P(int semid);


/*
 * \fn void V(int semid)
 * \brief Debloque un semaphore / mutex
 * \author cvericel
 * 
 * \param semid, id du semaphore / mutex
 */
void V(int semid);

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
int semfree(int semid);

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
void * shmalloc(key_t key, int size);

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
int shmfree(key_t key, void * addr);