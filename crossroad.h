#include <unistd.h>
#include <stdio.h>
#include "nFork.h"


/* CONSTANT */
#define PRIMARY_ROAD 0
#define SECONDARY_ROAD 1

#define RED 0
#define GREEN 1

#define NB_ROADLIGHTS 2

#define MAX_CAR_DEFAULT 10

#define SEM_ROAD_LIGTH_KEY 40
#define SEM_GREEN_LIGHT_KEY 45
#define SEM_NB_CAR_WAITING_KEY 50
#define SEM_NB_CAR_CREATED_KEY 55

#define SHARED_KEY 100


/*-----------------------------------------------------------------------
-------------------------------SHARED VAR--------------------------------
-------------------------------------------------------------------------*/
typedef struct{
    int nbCarCreated;
    // Roadligths color
    int roadLigthsColor[NB_ROADLIGHTS];
    // Number of car waiting on the road
    int nbCarWaitingRoad[NB_ROADLIGHTS];
    _Bool isFinish;
} Shared;
extern Shared * shared;

/*-----------------------------------------------------------------------
---------------------------------SEMAPHORE-------------------------------
-------------------------------------------------------------------------*/
extern int roadLigths[NB_ROADLIGHTS];
extern int greenLight[NB_ROADLIGHTS];

/*-----------------------------------------------------------------------
-----------------------------------MUTEX---------------------------------
-------------------------------------------------------------------------*/
extern int nbCarWaitingRoadMutex[NB_ROADLIGHTS];
extern int nbCarCreatedMutex;


void changeRoadLightColor(int road, int timeToWait);