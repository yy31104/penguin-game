#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*A structure which helps to send and sends several coordinates in a singular form
 * X - first value (ex. x value)
 * Y - second value (ex. y value)*/
typedef struct {
    int X;
    int Y;
} CoordXY;

/*A function of type CoordXY which divides the CoordXY variable into two integer values*/
CoordXY functionReturnsTwoInts();

/*A function which counts the sum of absolute values of differences between new and old coordinates
 * curCoords - variable of type CoordXY, representing current coordinates
 * newCoords - variable of type CoordXY, representing new coordinates*/
int find_sum_of_differences(CoordXY curCoords, CoordXY newCoords);

#endif

