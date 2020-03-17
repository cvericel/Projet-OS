/* 
 * \file car.c
 * \author {cvericel, tdarcour}
 * 
 * \brief Regroupe les fonctions qui geres les voitures
 */

#include "main.h"

/*
 * \fn void genereCar(int road)
 * \param road, route sur laquelle la voiture se situe
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures
 */
void genereCar(int road)
{
    int pid;
    Car car;
    switch (pid = fork()) // On creer un processus pour la voiture
    {
    case -1:
        perror("Erreur dans generateCarAutomatic\n");
        exit(1);
        break; 
    case 0:
        car.pid = pid;
        car.road = road;
        P(nbCarCreatedMutex); // On attend de pouvoir rentrer dans la sections critique
        shared->nbCarCreated++;
        V(nbCarCreatedMutex); // On libere la section critique
        car.index = shared->nbCarCreated;
        usleep(100000); // Temp d'arriver de la voiture dans le carrefour
        printf("\tVOITURE: arrivee de la voiture %d sur la voie %d\n", car.index, car.road);
        manageCarOnCrossroad(car);
        exit(0);
    }
}

/*
 * \fn void carCrossroad(Car car)
 * \param car, La voiture a gerer
 * \author {cvericel, tdarcour}
 * 
 * \brief Gere les voitures sur le carrefour
 */
void manageCarOnCrossroad(Car car)
{
    int road = car.road;
    /** La voiture est sur la voie primaire. **/
    if (shared->roadLigthsColor[road] == RED) {
        printf("\tVOITURE: la voiture %d attend le feu\n", car.index);
        P(nbCarWaitingRoadMutex[road]); // On attend l'acces la section critique
            shared->nbCarWaitingRoad[road]++;
        V(nbCarWaitingRoadMutex[road]); // On debloque l'acces a la section critique
        printf("\tCARREFOUR: il y'a %d voitures en attente sur la voie %d\n", shared->nbCarWaitingRoad[road], road);
        // On attend que la voie soit libre, c'est a dire feu vert + aucune voiture sur le carrefour
        P(greenLight[road]);
            P(nbCarWaitingRoadMutex[road]);
                shared->nbCarWaitingRoad[road]--;
            V(nbCarWaitingRoadMutex[road]); 
            V(greenLight[road]); // On libere la voie, les voitures de derriere peuvent desormais avancer
    } else {
        // Si le feu est vert et qu'il y'a des voitures en attentes devant nous on endort la voiture pour 2sec
        if (shared->nbCarWaitingRoad[road] > 0) {
            printf("\tVOITURE: la voiture %d attend que les voitures devant elles avancent sur la voie %d", car.index, road);
            usleep(200000);
            manageCarOnCrossroad(car); // Appelle "recursif" pour retourner au debut du programme
        }
    }
    printf("\tVOITURE: la voiture %d est passee\n", car.index);
}