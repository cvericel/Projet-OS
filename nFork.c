#include "nFork.h"
#include <unistd.h>

int nFork(int nbProcs)
{
    int a;
    for (int i = 1; i <= nbProcs; i++)
    {
        switch (a = fork())
        {
        case -1:
            return -1;
        case 0:
            return i;
        }
    }
    return 0;
}