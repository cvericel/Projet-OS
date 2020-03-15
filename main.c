#include "main.h"

 // TODO FINIR LE PROGRAMME PROPREMENT
 

/************************************************************************
-------------------------------SHARED VAR--------------------------------
************************************************************************/
Shared * shared;
/************************************************************************
--------------------------------SEMAPHORE--------------------------------
************************************************************************/
int roadLigths[NB_ROADLIGHTS];
int greenLight[NB_ROADLIGHTS];
int nbCarWaitingRoadMutex[NB_ROADLIGHTS];
int nbCarCreatedMutex;
int carOnCrossroadMutex;

int main (int argc, char * argv[])
{
    /*--------------------------INIT SEMAPHORE / MUTEX--------------------------*/
    int i;
    int nbCarMax = MAX_CAR_DEFAULT;
    _Bool activeMod = INTERACTIF_MOD;
    // Variable utiliser pour le mode automatique, le temp entre la géneration de deux voitures
    int randomTime;

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
    shared->timeToWait = DEFAULT_TIME_TO_WAIT;
    shared->isFinish = 0;
    /*-------------------------------------------------------------------------*/

    switch (nFork(2))
    {  
        case -1:
            perror("Fork failed in main\n");
            exit(1);
            break;
        case 1:
            V(roadLigths[PRIMARY_ROAD]);
            while(!shared->isFinish)
            {
                changeRoadLightColor(PRIMARY_ROAD);
            }

            return 0;
        
        case 2:
            while(!shared->isFinish)
            {
                changeRoadLightColor(SECONDARY_ROAD);
            }

            return 0;
    }

    /*--------------------------------ARGUMENTS--------------------------------*/
    char opt;
    while ((opt = getopt(argc, argv, "a:n:t:")) != -1) {
        switch (opt) {
            case 'a':
                activeMod = AUTOMATIC_MOD;
                randomTime = atoi(optarg) * 1000;
                printf("ARGUMENT: Mode automatique activé, temp maximum entre deux voitures: %d!\n", randomTime);
                break;
            case 'n':
                nbCarMax = atoi(optarg);
                printf("ARGUMENT: Bornage du nombre maximale de voiture à %d voitures !\n", nbCarMax);
                break;
            case 't':
                shared->timeToWait = atoi(optarg);
                printf("ARGUMENT: Temp des feux definis a %dms pour la voie 0 et %dms pour la voie 1!\n", shared->timeToWait * 2, shared->timeToWait);
                break;
        }
    }

    /*--------------------------------TRAITEMENTS--------------------------------*/
    if (activeMod == AUTOMATIC_MOD) {
        if (randomTime == 0) randomTime = DEFAULT_TIME_BETWEEN_CAR;

        srandom(getpid());

        while(shared->nbCarCreated <= nbCarMax) {
            generateCarAutomatic();
            usleep(rand() % randomTime);  
        }
        shared->isFinish = 1;
        // Stop jusqu'a que toute les voitures soient passe
        usleep(shared->timeToWait * 4);
    } else {
        int charInASCIIValue;

        while (!shared->isFinish)
        {
            // a = 97, z = 122, q = 113 en code ASCII
            charInASCIIValue = readChar();

            switch (charInASCIIValue)
            {
                case 97:
                    // Ajoute une nouvelle voiture sur la voie 0
                    genereCarInteractif(PRIMARY_ROAD);
                    break;
                case 113:
                    // Met fin au programme
                    shared->isFinish = 1;
                    break;
                case 122:
                    // Ajoute une nouvelle voiture sur le voie 1
                    genereCarInteractif(SECONDARY_ROAD);
                    break;
                default:
                    printf("\t\tERREUR: 'a' pour ajouter une voiture sur la voie 0, 'z' sur la voie 1 et q pour quitter\n");
            }
        }
    }

    cleanup();    
    printf("Arret du programme\n");
    return 0;
}

void cleanup()
{
    int i;

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
    shmfree(shared->timeToWait);
    shmfree(shared->isFinish);
    waitpid(0, 0, 0);
}