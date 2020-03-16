/* 
 * \file car.c
 * \author {cvericel, tdarcour}
 * 
 * \brief Regroupe les fonctions qui geres les voitures
 */

#include "main.h"

/*
 * \fn void genereCarInteractif(int road)
 * \param road, route sur laquelle la voiture se situe
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures pour le mode interactif
 */
void genereCarInteractif(int road)
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
        carCrossroad(car);
        exit(0);
    }
}

/*
 * \fn void generateCarAutomatic()
 * \author {cvericel, tdarcour}
 * 
 * \brief Genere les voitures pour le mode automatique
 */
void generateCarAutomatic()
{

    int pid;
    int road = rand() % 2;
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
            carCrossroad(car); // Car gestion in crossroad
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
void carCrossroad(Car car)
{
    switch (car.road)
    {
        case PRIMARY_ROAD:
            /** La voiture est sur la voie primaire. **/
            if (shared->roadLigthsColor[PRIMARY_ROAD] == RED) {
                printf("\tVOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[PRIMARY_ROAD]); // On attend l'acces la section critique
                    shared->nbCarWaitingRoad[PRIMARY_ROAD]++;
                V(nbCarWaitingRoadMutex[PRIMARY_ROAD]); // On debloque l'acces a la section critique
                printf("CARREFOUR: il y'a %d voitures en attente sur la voie %d\n", shared->nbCarWaitingRoad[PRIMARY_ROAD], PRIMARY_ROAD);
                // On attend que la voie soit libre, c'est a dire feu vert + aucune voiture sur le carrefour
                P(greenLight[PRIMARY_ROAD]);
                    P(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
                        shared->nbCarWaitingRoad[PRIMARY_ROAD]--;
                    V(nbCarWaitingRoadMutex[PRIMARY_ROAD]); 
                    V(greenLight[PRIMARY_ROAD]); // On libere la voie, les voitures de derriere peuvent desormais avancer
            } else {
                // Si le feu est vert et qu'il y'a des voitures en attentes devant nous on endort la voiture pour 2sec
                if (shared->nbCarWaitingRoad[PRIMARY_ROAD] > 0) {
                    printf("\tVOITURE: la voiture %d attend que les voitures devant elles avancent sur la voie %d", car.index, PRIMARY_ROAD);
                    usleep(200000);
                    carCrossroad(car); // Appelle "recursif" pour retourner au debut du programme
                }
            }
            printf("\tVOITURE: la voiture %d est passee\n", car.index);
            break;
        case SECONDARY_ROAD:
            /** La voiture est sur la voie secondaire. **/ 
            if(shared->roadLigthsColor[SECONDARY_ROAD] == RED) {
                printf("\tVOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                    shared->nbCarWaitingRoad[SECONDARY_ROAD]++;
                V(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                printf("\tVOITURE: Il y'a %d voitures en attente sur la voie %d\n", shared->nbCarWaitingRoad[SECONDARY_ROAD], SECONDARY_ROAD);
                P(greenLight[SECONDARY_ROAD]);
                    P(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                        shared->nbCarWaitingRoad[SECONDARY_ROAD]--;
                    V(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                    V(greenLight[SECONDARY_ROAD]);

            } else{
                if (shared->nbCarWaitingRoad[SECONDARY_ROAD] > 0) {
                    printf("\tVOITURE: la voiture %d attend que les voitures devant elles avancent sur la voie %d\n", car.index, SECONDARY_ROAD);
                    usleep(200000);
                    carCrossroad(car);
                }
            }

            printf("\tVOITURE: la voiture %d est passee\n", car.index);
            break;
    }
}