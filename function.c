#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "function.h"

CoordXY functionReturnsTwoInts(void) {
    CoordXY c; c.X = 0; c.Y = 0; return c;
}

int find_sum_of_differences(CoordXY curCoords, CoordXY newCoords)
{
    int X_diff, Y_diff;
    int Diff_sum;

    X_diff = abs(newCoords.X - curCoords.X);
    Y_diff = abs(newCoords.Y - curCoords.Y);

    Diff_sum = X_diff + Y_diff;

    return Diff_sum;
}

