#include "crossroad.h"
#include "car.h"
#include "ipcTools.h"

/*
-------------------------------------------------------------------------
-------------------------------SHARED VAR--------------------------------
-------------------------------------------------------------------------
*/
int nbCarCreated;
int roadLigthsColor[NB_ROADLIGHTS];
int nbCarWaitingRoad[NB_ROADLIGHTS];
_Bool isFinish;
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
        roadLigthsColor[PRIMARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", PRIMARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", nbCarWaitingRoad[SECONDARY_ROAD]);
        if (nbCarWaitingRoad[PRIMARY_ROAD] > 0) V(greenLight[PRIMARY_ROAD]);
        usleep(timeToWait * 20000);//TODO change that
        roadLigthsColor[PRIMARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", PRIMARY_ROAD);
        V(roadLigths[SECONDARY_ROAD]);
    } else {
        P(roadLigths[SECONDARY_ROAD]);
        roadLigthsColor[SECONDARY_ROAD] = GREEN;
        printf("CARREFOUR: Le feu %d passe au vert\n", PRIMARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", nbCarWaitingRoad[SECONDARY_ROAD]);
        if(nbCarWaitingRoad[SECONDARY_ROAD] > 0) V(greenLight[SECONDARY_ROAD]);
        usleep(timeToWait * 10000);
        roadLigthsColor[SECONDARY_ROAD] = RED;
        printf("CARREFOUR: Le feu %d passe au rouge\n", PRIMARY_ROAD);
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
        nbCarWaitingRoadMutex[i] = semalloc(SEM_NB_CAR_CREATED_KEY + i, 0);
    
    /*------------------------------SHARED MEMORY------------------------------*/
    nbCarCreated = shmalloc(SHARED_NB_CAR_CREATED, sizeof(int));
    for (i = 0; i < NB_ROADLIGHTS; i++)
    {
        roadLigthsColor[NB_ROADLIGHTS] = shmalloc(SHARED_ROADLIGHT_COLOR + i, sizeof(int));
        nbCarWaitingRoad[NB_ROADLIGHTS] = shmalloc(SHARED_NB_CAR_WAITING + i, sizeof(int));
    }
    isFinish = shmalloc(SHARED_IS_FINISH, sizeof(_Bool));

    nbCarCreated = 0;
    roadLigthsColor[PRIMARY_ROAD] = GREEN;
    roadLigthsColor[SECONDARY_ROAD] = RED;
    nbCarWaitingRoad[PRIMARY_ROAD], nbCarWaitingRoad[SECONDARY_ROAD] = 0;
    isFinish = 0;

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
            while(!isFinish)
            {
                changeRoadLightColor(PRIMARY_ROAD, 1000);//TODO time
            }
            break;
        case 2:
            while(!isFinish)
            {
                changeRoadLightColor(SECONDARY_ROAD, 1000);//TODO time
            }
        default: 
            break;
    }

    return 0;
}

int pere()
{
    int i;
    int nbCarMax = 20;

    while (nbCarCreated < nbCarMax)
    {
        generateCarAutomatic();
        usleep(100000);
    }
    isFinish = 1;

    semfree(nbCarCreatedMutex);
    semfree(nbCarWaitingRoadMutex);
    for(i = 0; i < NB_ROADLIGHTS; i++)
    {
        semfree(roadLigths[i]);
        semfree(greenLight[i]);
    }

    shmfree(nbCarCreated);
    for (i = 0; i < NB_ROADLIGHTS; i++)
    {
        shmfree(roadLigthsColor[i]);
        shmfree(nbCarWaitingRoad[i]);
    }
    shmfree(isFinish);
    waitpid(0, 0, 0);
    printf("Arret du programme\n");
}