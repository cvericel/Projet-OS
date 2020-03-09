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

#define SHARED_NB_CAR_CREATED 60
#define SHARED_ROADLIGHT_COLOR 70
#define SHARED_NB_CAR_WAITING 80
#define SHARED_IS_FINISH 90

/*-----------------------------------------------------------------------
-------------------------------SHARED VAR--------------------------------
-------------------------------------------------------------------------*/
extern int nbCarCreated;
// Roadligths color
extern int roadLigthsColor[NB_ROADLIGHTS];
// Number of car waiting on the road
extern int nbCarWaitingRoad[NB_ROADLIGHTS];
extern _Bool isFinish;

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