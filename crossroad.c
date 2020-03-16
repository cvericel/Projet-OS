#include "crossroad.h"
#include "main.h"

/**
 * \fn void changeRoadLightColor (int road)
 * \author cvericel
 * \brief Fonction qui gere le carrefour, elle s'occupe du changement des feux
 *        Necessite un processus par voie, sinon il ne fonctionnera pas
 * 
 * \param road, La route a gerer
 */
void changeRoadLightColor (int road)
{
    if(road == PRIMARY_ROAD) {
        /*------------------VOIE 0------------------*/
        // On attend que le feu puisse etre vert
        P(roadLigths[PRIMARY_ROAD]);
        shared->roadLigthsColor[PRIMARY_ROAD] = GREEN; // On passe le feu au vert
        printf("CARREFOUR: Le feu %d passe au vert\n", PRIMARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        V(greenLight[PRIMARY_ROAD]); // On debloque la voie
        usleep(shared->timeToWait * 2000);
        // On bloque la voie, une fois toute les voitures passees
        P(greenLight[PRIMARY_ROAD]);
        shared->roadLigthsColor[PRIMARY_ROAD] = RED; // On passe le feu au rouge
        printf("CARREFOUR: Le feu %d passe au rouge\n", PRIMARY_ROAD);
        // On libere le feu 1 pour qu'il puisse passer au vert
        V(roadLigths[SECONDARY_ROAD]);

    } else {
        /*------------------VOIE 1------------------*/
        // On attend que le feu puisse etre vert
        P(roadLigths[SECONDARY_ROAD]);
        shared->roadLigthsColor[SECONDARY_ROAD] = GREEN; // On passe le feu au vert
        printf("CARREFOUR: Le feu %d passe au vert\n", SECONDARY_ROAD);
        printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[SECONDARY_ROAD]);
        V(greenLight[SECONDARY_ROAD]); // On debloque la voie
        usleep(shared->timeToWait * 1000);
        // On bloque la voie une fois toute les voitures passees
        P(greenLight[SECONDARY_ROAD]);
        shared->roadLigthsColor[SECONDARY_ROAD] = RED; // On passe le feu au rouge
        printf("CARREFOUR: Le feu %d passe au rouge\n", SECONDARY_ROAD);
        // On libere le feu 0 pour qu'il puisse passer au rouge
        V(roadLigths[PRIMARY_ROAD]);
    }
}