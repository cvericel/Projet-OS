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
 * \fn void genereCar(int road)
 * \param road, route sur laquelle la voiture se situe
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures
 */
void genereCar(int road);


/*
 * \fn void manageCarOnCrossroad(Car car)
 * \param car, La voiture a gerer
 * \author {cvericel, tdarcour}
 * 
 * \brief Gere une voiture sur le carrefour
 */
void manageCarOnCrossroad(Car car);