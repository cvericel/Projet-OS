/*
 * CONSTANTES
 * \def PRIMARY_ROAD
 * \def SECONDARY_ROAD
 * \def RED
 * \def GREEN
 * \def NB_ROADLIGHTS
 */
#define PRIMARY_ROAD 0
#define SECONDARY_ROAD 1
#define RED 0
#define GREEN 1
#define NB_ROADLIGHTS 2


/**
 * \fn void changeRoadLightColor (int road)
 * \author cvericel
 * \brief Fonction qui gere le carrefour, elle s'occupe du changement des feux
 *        Necessite un processus par voie, sinon il ne fonctionnera pas
 * 
 * \param road, La route a gerer
 */
void changeRoadLightColor(int road);