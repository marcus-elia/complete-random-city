#ifndef COMPLETE_RANDOM_CITY_MATHHELPER_H
#define COMPLETE_RANDOM_CITY_MATHHELPER_H

#include <cmath>
#include "structs.h"

// ============ Number Theory ============

// Since % can return negatives
int mod(int a, int m);

// ============ Distance =============
double distanceFormula(double x1, double y1, double x2, double y2);

// Returns the distance in the xz plane
double distance2d(Point p1, Point p2);

// 2d directed distance
double directedDistance(double A, double B, double C, double x, double y);

// =============== Points ==============

// Translates Point p
void movePoint(Point &p, double deltaX, double deltaY, double deltaZ);

// Rotates p around pBase in 3D space
void rotatePointAroundPoint(Point &p, const Point &pBase, double thetaX, double thetaY, double thetaZ);

#endif //COMPLETE_RANDOM_CITY_MATHHELPER_H
