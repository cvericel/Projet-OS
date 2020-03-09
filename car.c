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
                nbCarCreated++;
            V(nbCarCreatedMutex);
            car.index = nbCarCreated;
            usleep(100000);
            printf("VOITURE: arrivee de la voiture %d sur la voie %d\n", car.index, car.road);
            carCrossroad(car); // Car gestion in crossroad
            exit(0);
    }

}

void carCrossroad(Car car)
{
    switch (car.road)
    {
        case PRIMARY_ROAD:
            if (roadLigthsColor[PRIMARY_ROAD] == RED) {
                printf("VOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
                    nbCarWaitingRoad[PRIMARY_ROAD]++;
                V(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
                printf("Il y'a %d voitures en attente sur la voie %d", nbCarWaitingRoad[PRIMARY_ROAD], PRIMARY_ROAD);
                P(greenLight[PRIMARY_ROAD]);
            }
            
            // GREEN LIGHT
            printf("VOITURE: la voiture %d est passee\n", car.index);
            break;
        case SECONDARY_ROAD:
            if(roadLigthsColor[SECONDARY_ROAD] == RED) {
                printf("VOITURE: la voiture %d attend le feu\n", car.index);
                P(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                    nbCarWaitingRoad[SECONDARY_ROAD]++;
                V(nbCarWaitingRoadMutex[SECONDARY_ROAD]);
                printf("Il y'a %d voitures en attente sur la voie %d", nbCarWaitingRoad[SECONDARY_ROAD], SECONDARY_ROAD);
                P(greenLight[SECONDARY_ROAD]);
            }

            // GREEN LIGHT
            printf("VOITURE: la voiture %d est passee\n", car.index);
            break;
    }
}