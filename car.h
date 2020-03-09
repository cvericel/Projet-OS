#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

/*
 * Car struct
 * pid = processus pid
 * road = road where the car is waiting
 * car index (ex: car number 4)
 */
typedef struct{
    int pid;
    int road;
    int index;
}Car;

void generateCarAutomatic();

void carCrossroad(Car car);

int pere();