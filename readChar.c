#include "readChar.h"

/*
 * \fn int readChar()
 * \brief Bloque l'entree standard
 * \return c, le code ASCII correspondant au caractere
 */
int readChar()
{
    int c=0;
    struct termios org_opts, new_opts; 

    //----- store old settings ----------- 
    tcgetattr(STDIN_FILENO, &org_opts);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ICRNL | ECHO | ECHOE | ECHOK | ECHONL| ECHOPRT | ECHOKE);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    //------ restore old settings --------- 
    tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    return(c); 
}