#include "mathHelper.h"



int mod(int a, int m)
{
    int x = a % m;
    if(x < 0)
    {
        return x + m;
    }
    return x;
}



double distanceFormula(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double distance2d(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.z - p2.z)*(p1.z - p2.z));
}

double directedDistance(double A, double B, double C, double x, double y)
{
    return (A*x + B*y + C) / sqrt(A*A + B*B);
}