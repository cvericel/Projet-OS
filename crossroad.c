#include "crossroad.h"
#include "car.h"
#include "ipcTools.h"

/*
-------------------------------------------------------------------------
-------------------------------SHARED VAR--------------------------------
-------------------------------------------------------------------------
*/
Shared * shared;
/*
-------------------------------------------------------------------------
--------------------------------SEMAPHORE--------------------------------
-------------------------------------------------------------------------
*/
int roadLigths[NB_ROADLIGHTS];
int greenLight[NB_ROADLIGHTS];
int nbCarWaitingRoadMutex[NB_ROADLIGHTS];
int nbCarCreatedMutex;

void changeRoadLightColor (int road, int timeToWait)
{
    if(road == PRIMARY_ROAD) {
        P(roadLigths[PRIMARY_ROAD]);
        shared->roadLigthsColor[PRIMARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", PRIMARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        if (shared->nbCarWaitingRoad[PRIMARY_ROAD] > 0) V(greenLight[PRIMARY_ROAD]);
        usleep(timeToWait * 2000);//TODO change that
        shared->roadLigthsColor[PRIMARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", PRIMARY_ROAD);
        V(roadLigths[SECONDARY_ROAD]);

    } else {
        P(roadLigths[SECONDARY_ROAD]);
        shared->roadLigthsColor[SECONDARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", SECONDARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        if(shared->nbCarWaitingRoad[SECONDARY_ROAD] > 0) V(greenLight[SECONDARY_ROAD]);
        usleep(timeToWait * 1000);
        shared->roadLigthsColor[SECONDARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", SECONDARY_ROAD);
        V(roadLigths[PRIMARY_ROAD]);
    }
}

int main (int argc, char * argv[])
{
    /*--------------------------INIT SEMAPHORE / MUTEX--------------------------*/
    int i;
    
    for(i = 0; i < NB_ROADLIGHTS; i++)
    {
        roadLigths[i] = semalloc(SEM_ROAD_LIGTH_KEY + i, 0);
        greenLight[i] = semalloc(SEM_GREEN_LIGHT_KEY + i, 0);
    }
    // Init mutex
    nbCarCreatedMutex = semalloc(SEM_NB_CAR_CREATED_KEY, 1);
    for(i = 0; i < NB_ROADLIGHTS; i++) 
        nbCarWaitingRoadMutex[i] = semalloc(SEM_NB_CAR_CREATED_KEY + i, 1);
    
    /*------------------------------SHARED MEMORY------------------------------*/
    shared = (Shared*)shmalloc(SHARED_KEY, sizeof(Shared));
    shared->nbCarCreated = 0;
    shared->roadLigthsColor[PRIMARY_ROAD] = GREEN;
    shared->roadLigthsColor[SECONDARY_ROAD] = RED;
    shared->nbCarWaitingRoad[PRIMARY_ROAD] = 0;
    shared->nbCarWaitingRoad[SECONDARY_ROAD] = 0;
    shared->isFinish = 0;

    switch (nFork(2))
    {
        case -1:
            perror("Fork failed primaryRoad\n");
            exit(1);
            break;
        case 0:
            /*--------------------------------AUTOMATIC--------------------------------*/
            pere();
        case 1:
            V(roadLigths[PRIMARY_ROAD]);
            while(!shared->isFinish)
            {
                changeRoadLightColor(PRIMARY_ROAD, 4000);//TODO time
            }
            break;
        case 2:
            while(!shared->isFinish)
            {
                changeRoadLightColor(SECONDARY_ROAD, 4000);//TODO time
            }
        default: 
            break;
    }

    return 0;
}

int pere()
{
    int i;
    int nbCarMax = 10;
    while (shared->nbCarCreated < nbCarMax)
    {
        generateCarAutomatic();
        usleep(4000000);
    }
    shared->isFinish = 1;

    semfree(nbCarCreatedMutex);
    semfree(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
    semfree(nbCarWaitingRoadMutex[SECONDARY_ROAD]);

    for(i = 0; i < NB_ROADLIGHTS; i++)
    {
        semfree(roadLigths[i]);
        semfree(greenLight[i]);
    }

    shmfree(shared->nbCarCreated);
    for (i = 0; i < NB_ROADLIGHTS; i++)
    {
        shmfree(shared->roadLigthsColor[i]);
        shmfree(shared->nbCarWaitingRoad[i]);
    }
    shmfree(shared->isFinish);
    waitpid(0, 0, 0);
    printf("Arret du programme\n");
    return 0;
}