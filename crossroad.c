#include "crossroad.h"
#include "main.h"

/**
 * \fn void manageRoadLightColor (int road)
 * \author cvericel
 * \brief Fonction qui gere le carrefour, elle s'occupe du changement des feux
 *        Necessite un processus par voie, sinon il ne fonctionnera pas
 * 
 * \param road, La route a gerer
 */
void manageRoadLightColor (int road)
{   
    // On attend que le feu puisse etre vert
    P(roadLigths[road]);
    shared->roadLigthsColor[road] = GREEN; // On passe le feu au vert
    printf("CARREFOUR: Le feu %d passe au vert\n", road);
    printf("CARREFOUR: On libere %d voiture(s)\n", shared->nbCarWaitingRoad[road]);
    V(greenLight[road]); // On debloque la voie
    if (road == PRIMARY_ROAD) usleep(shared->timeToWait * 2000); else usleep(shared->timeToWait * 1000);
    // On bloque la voie, une fois toute les voitures passees
    P(greenLight[road]);
    shared->roadLigthsColor[road] = RED; // On passe le feu au rouge
    printf("CARREFOUR: Le feu %d passe au rouge\n", road);
    // On libere l'autre feu pour qu'il puisse passer au vert
    if (road == PRIMARY_ROAD) V(roadLigths[SECONDARY_ROAD]); else V(roadLigths[PRIMARY_ROAD]);
}