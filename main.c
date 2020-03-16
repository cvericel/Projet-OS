/*
 * \file main.c
 * \author {cvericel, tdarcour}
 * 
 * \brief Programme main
 */
#include "main.h"

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

/*
 * \fn int main (int argc, char * argv[])
 * \author {cvericel, tdarcour}
 * 
 * \brief Fonction main, met en commun tous les processus.
 */
int main (int argc, char * argv[])
{
    /*---------------------------------VARIABLE---------------------------------*/
    int i; // index
    int nbCarMax = MAX_CAR_DEFAULT; // Nombre de voitures maximum(par defaut: 10).
    _Bool activeMod = INTERACTIF_MOD; // Mode active, interactif(par defaut) ou automatique.
    int randomTime; // Variable utilise pour le mode automatique, temp maximum entre l'apparition de deux voitures.
    /*-------------------------------------------------------------------------*/

    /*--------------------------INIT SEMAPHORE / MUTEX--------------------------*/
    for (i = 0; i < NB_ROADLIGHTS; i++)
    {
        roadLigths[i] = semalloc(SEM_ROAD_LIGTH_KEY + i, 0);
        greenLight[i] = semalloc(SEM_GREEN_LIGHT_KEY + i, 0);
    }
    nbCarCreatedMutex = semalloc(SEM_NB_CAR_CREATED_KEY, 1);
    for(i = 0; i < NB_ROADLIGHTS; i++) 
        nbCarWaitingRoadMutex[i] = semalloc(SEM_NB_CAR_WAITING_KEY + i, 1);
    /*-------------------------------------------------------------------------*/

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

    /*---------------------Creation de deux processus fils---------------------*/

    int pidPR, pidSR;
    switch(pidPR = fork()) // Processus qui va gerer la voie principal du carrefour (le feu)
    {
        case -1:
            perror("Fork failed in PR\n");
            exit(1);
            break;
        case 0:
            V(roadLigths[PRIMARY_ROAD]); // On libere le feu de la voie 0 pour qu'il passe au vert
            while(!shared->isFinish)
            {
                changeRoadLightColor(PRIMARY_ROAD);
            }
            exit(0);
        default: break;
    }

    switch(pidSR = fork()) // Processus qui va gere la voie secondaire du carrefour (le feu)
    {
        case -1:
            perror("Fork failed in SR\n");
            exit(1);
            break;
        case 0:
            while(!shared->isFinish)
            {
                changeRoadLightColor(SECONDARY_ROAD);
            }
            exit(0);
        default: break;
    }
    /*-------------------------------------------------------------------------*/

    /*--------------------------------ARGUMENTS--------------------------------*/
    char opt;
    while ((opt = getopt(argc, argv, "a:n:t:")) != -1) {
        switch (opt) {
            case 'a':
                activeMod = AUTOMATIC_MOD;
                randomTime = atoi(optarg) * 1000; // On recupere le parametre en ms de l'argument et on le met en seconde
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
    /*-------------------------------------------------------------------------*/

    /*--------------------------------TRAITEMENTS--------------------------------*/

    if (activeMod == AUTOMATIC_MOD) { // Mode automatique
        // Si aucun parametre n'a etais donné avec l'argument -a
        if (randomTime == 0) randomTime = DEFAULT_TIME_BETWEEN_CAR; 

        srandom(getpid());
        // On genere les voitures avec un temp aleatoire allant jusqu'a randomTime
        while(shared->nbCarCreated <= nbCarMax) {
            generateCarAutomatic();
            usleep(rand() % randomTime);  
        }
        // On attend que les voitures soit passés avant de mettre fin au programme
        while (shared->nbCarWaitingRoad[PRIMARY_ROAD] > 0 || shared->nbCarWaitingRoad[SECONDARY_ROAD] > 0) sleep(3);

        shared->isFinish = 1;
    } else {
        int charInASCIIValue; // Valeur du characteur en code ASCII

        while (!shared->isFinish)
        {
            // a = 97, z = 122, q = 113 en code ASCII.
            charInASCIIValue = readChar();

            switch (charInASCIIValue)
            {
                case 97:
                    // Ajoute une nouvelle voiture sur la voie 0.
                    genereCarInteractif(PRIMARY_ROAD);
                    break;
                case 113:
                    // On attend que les voitures soit passés avant de mettre fin au programme
                    while (shared->nbCarWaitingRoad[PRIMARY_ROAD] > 0 || shared->nbCarWaitingRoad[SECONDARY_ROAD] > 0) sleep(3);
                    shared->isFinish = 1;
                    break;
                case 122:
                    // Ajoute une nouvelle voiture sur le voie 1.
                    genereCarInteractif(SECONDARY_ROAD);
                    break;
                default:
                    printf("\t\tERREUR: 'a' pour ajouter une voiture sur la voie 0, 'z' sur la voie 1 et q pour quitter\n");
            }
        }
    }
    
    // On attend que les processus de gestions du carrefour s'arrete.
    waitpid(pidPR, 0, 0);
    waitpid(pidSR, 0, 0);
    // On clean tous les semaphores ainsi que la memoire partagee
    cleanup();    
    printf("PROGRAMME: Arret du programme !\n");

    return 0;
}

/*
 * \fn void cleanup()
 * \author cvericel
 *  
 * \brief Detruie les semaphores et la memoire partagee
 */
void cleanup()
{
    semfree(nbCarCreatedMutex);
    semfree(nbCarWaitingRoadMutex[PRIMARY_ROAD]);
    semfree(nbCarWaitingRoadMutex[SECONDARY_ROAD]);

    for(int i = 0; i < NB_ROADLIGHTS; i++)
    {
        semfree(roadLigths[i]);
        semfree(greenLight[i]);
    }
    shmfree(SHARED_KEY, shared);
}