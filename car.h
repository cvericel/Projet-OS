#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

/*
 * Voiture
 * \struct Car
 * \brief Structure d'une voiture
 */
typedef struct{
    int pid; // Pid du processus qui gere la voiture
    int road; // Route sur laquelle la voiture se situe
    int index;
}Car;

/*
 * \fn void genereCarInteractif(int road)
 * \param road, route sur laquelle la voiture se situe
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures pour le mode interactif
 */
void genereCarInteractif(int road);
 
/*
 * \fn void generateCarAutomatic()
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures pour le mode automatique
 */
void generateCarAutomatic();

/*
 * \fn void carCrossroad(Car car)
 * \param car, La voiture a gerer
 * \author {cvericel, tdarcour}
 * 
 * \brief Gere les voitures sur le carrefour
 */
void carCrossroad(Car car);