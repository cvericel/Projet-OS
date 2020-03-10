#include "car.h"
#include "crossroad.h"
#include "ipcTools.h"

void generateCarAutomatic()
{

    int pid;
    int road = random()%2; // Get random road id between 1 and 2
    Car car;

    switch (pid = fork())
    {
        case -1:
            perror("Erreur dans generateCarAutomatic\n");
            exit(1);
            break;
        case 0:
            car.pid = pid;
            car.road = road;
            P(nbCarCreatedMutex);
                shared->nbCarCreated++;
            V(nbCarCreatedMutex);
            car.index = shared->nbCarCreated;
            usleep(100000);
            printf("\tVOITURE: arrivee de la voiture %d sur la voie %d\n", car.index, car.road);
            carCrossroad(car); // Car gestion in crossroad
            exit(0);
    }

}

void carCrossroad(Car car)
{
    switch (car.road)
    {
        case PRIMARY_ROAD:
            if (shared->roadLigthsColor[PRIMARY_ROAD] == RED) {
                printf("\tVOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
                    shared->nbCarWaitingRoad[PRIMARY_ROAD]++;
                V(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
                printf("CARREFOUR: il y'a %d voitures en attente sur la voie %d\n", shared->nbCarWaitingRoad[PRIMARY_ROAD], PRIMARY_ROAD);
                P(greenLight[PRIMARY_ROAD]);
                printf("DEBUG: GREENLIGTH PRIMARY ROAD\n");

            }
            
            // GREEN LIGHT
            printf("\tVOITURE: la voiture %d est passee\n", car.index);
            break;
        case SECONDARY_ROAD:
            if(shared->roadLigthsColor[SECONDARY_ROAD] == RED) {
                printf("\tVOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                    shared->nbCarWaitingRoad[SECONDARY_ROAD]++;
                V(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                printf("\tVOITURE: Il y'a %d voitures en attente sur la voie %d\n", shared->nbCarWaitingRoad[SECONDARY_ROAD], SECONDARY_ROAD);
                P(greenLight[SECONDARY_ROAD]);
                printf("DEBUG: GREENLIGTH SECONDARY ROAD\n");

            }

            // GREEN LIGHT
            printf("\tVOITURE: la voiture %d est passee\n", car.index);
            break;
    }
}