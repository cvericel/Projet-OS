#include "crossroad.h"
#include "main.h"

void changeRoadLightColor (int road, int timeToWait)
{
    if(road == PRIMARY_ROAD) {
        P(roadLigths[PRIMARY_ROAD]);
        shared->roadLigthsColor[PRIMARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", PRIMARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        V(greenLight[PRIMARY_ROAD]);
        usleep(timeToWait * 2000);//TODO change that
        P(greenLight[PRIMARY_ROAD]);
        shared->roadLigthsColor[PRIMARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", PRIMARY_ROAD);
        V(roadLigths[SECONDARY_ROAD]);

    } else {
        P(roadLigths[SECONDARY_ROAD]);
        shared->roadLigthsColor[SECONDARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", SECONDARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        V(greenLight[SECONDARY_ROAD]);
        usleep(timeToWait * 1000);
        P(greenLight[SECONDARY_ROAD]);
        shared->roadLigthsColor[SECONDARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", SECONDARY_ROAD);
        V(roadLigths[PRIMARY_ROAD]);
    }
}