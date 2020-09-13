#ifndef COMPLETE_RANDOM_CITY_MATHHELPER_H
#define COMPLETE_RANDOM_CITY_MATHHELPER_H

#include <cmath>
#include <vector>
#include "structs.h"
#include <experimental/optional>

// This file contains general math helper functions

const double PI = 3.14159265358979323846;



// Convert between chunkID's and Point2D's
Point2D chunkIDtoPoint2D(int n);
int point2DtoChunkID(Point2D p);


// ============ Number Theory ============

// Since % can return negatives
int mod(int a, int m);

// Assuming n > 0, this returns the integer closest to n that
// is a perfect square
int nearestPerfectSquare(int n);

// Returns the square root of n, assuming that n is a perfect square
int isqrt(int n);

// ============ Chunk IDs =============

// Convert between chunkID's and Point2D's
Point2D chunkIDtoPoint2D(int n);
int point2DtoChunkID(Point2D p);

// Returns the ints corresponding to to all chunks that are within radius of this one,
// using the taxicab metric
std::vector<int> getChunkIDsAroundPoint(Point2D p, int radius);
std::vector<Point2D> getChunkTopLeftCornersAroundPoint(Point2D p, int radius);
// Wrappers
std::vector<int> getChunkIDsAroundPoint(int chunkID, int radius);
std::vector<Point2D> getChunkTopLeftCornersAroundPoint(int chunkID, int radius);

int getChunkIDContainingPoint(Point p, int chunkSize);

// ============ Geometry =============
double distanceFormula(double x1, double y1, double x2, double y2);

// Returns the distance in the xz plane
double distance2d(Point p1, Point p2);

// 2d directed distance
double directedDistance(double A, double B, double C, double x, double y);

bool isAboveLineXZPlane(Point p, double m, double b);

// =============== Points ==============

// Translates Point p
void movePoint(Point &p, double deltaX, double deltaY, double deltaZ);

// Rotates p around pBase in 3D space
void rotatePointAroundPoint(Point &p, const Point &pBase, double thetaX, double thetaY, double thetaZ);

// Returns rotated copy of p, leaving p unchanged
Point getRotatedPointAroundPoint(const Point &p, const Point &pBase, double thetaX, double thetaY, double thetaZ)

// ============= Collision =============
// Working in the xz-plane, assume there is a rectangle centered at c that is aligned with the x,z-axes
// with xw and zw as its dimensions. This returns the Point with adjusted x and z  coordinates to move
// p exactly buffer away from the rectangle.
std::experimental::optional<Point> correctAlignedRectangularCrossSection(Point p, int buffer, Point c,
                                                                         double xw, double zw);

// Performs a rotation and calls correctAlignedRectangularCrossSection(), then unrotates
std::experimental::optional<Point> correctRectangularCrossSection(Point p, int buffer, Point c,
                                                                  double xw, double zw, double xzAngle);

// This assumes x, y, and z are all aligned.
std::experimental::optional<Point> correctAlignedRectangularPrism(Point p, int buffer, Point c,
                                                                  double xw, double yw, double zw);

// This assumes the y-axis is aligned. If there was a y-rotation, this does not work.
std::experimental::optional<Point> correctRectangularPrism(Point p, int buffer, Point c,
                                                           double xw, double yw, double zw, double xzAngle);

#endif //COMPLETE_RANDOM_CITY_MATHHELPER_H
