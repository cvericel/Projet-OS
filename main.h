/*
 * \file main.c
 * \author {cvericel, tdarcour}
 * 
 * \brief Programme main
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "crossroad.h"
#include "car.h"
#include "ipcTools.h"
#include "readChar.h"

/*
 * Constantes.
 * \def MAX_CAR_DEFAULT
 * \def AUTOMATIC_MOD
 * \def INTERACTIF_MOD
 * \def DEFAULT_TIME_TO_WAITE
 * \def DEFAULT_TIME_BETWEEN_CAR
 * 
 * Clefs des semaphores.
 * \def SEM_ROAD_LIGTH_KEY
 * \def SEM_GREEN_LIGHT_KEY
 * \def SEM_NB_CAR_WAITING_KEY
 * \def SEM_NB_CAR_CREATED_KEY
 * \def SEM_CAR_ON_CROSSROAD
 * 
 * Clef de la memoire partagee.
 * \def SHARED_KEY
 */
#define MAX_CAR_DEFAULT 10
#define AUTOMATIC_MOD 0
#define INTERACTIF_MOD 1
#define DEFAULT_TIME_TO_WAIT 2000
#define DEFAULT_TIME_BETWEEN_CAR 1000

#define SEM_ROAD_LIGTH_KEY 40
#define SEM_GREEN_LIGHT_KEY 45
#define SEM_NB_CAR_WAITING_KEY 50
#define SEM_NB_CAR_CREATED_KEY 55
#define SEM_CAR_ON_CROSSROAD 60

#define SHARED_KEY 100


/*
 * ----------------------------------------------------------------------
 * --------------------------MEMOIRE PARTAGEE----------------------------
 * ----------------------------------------------------------------------
 * \struct Shared
 * \brief structure qui contient toute les variables partagees
 */
typedef struct{
    int nbCarCreated; // Nombre de voiture cree
    int roadLigthsColor[NB_ROADLIGHTS]; // Couleur du feu
    int nbCarWaitingRoad[NB_ROADLIGHTS]; // Nombre de voitures en attentes sur une voie
     int timeToWait; // Temp d'attente pour le feu de la voie secondaire
    _Bool isFinish; // Boolean pour savoir quand toutes les voitures sont passees
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
extern int carOnCrossroadMutex;

/*
 * \fn void cleanup()
 * \author cvericel
 *  
 * \brief Detruie les semaphores et la memoire partagee
 */
void cleanup();