#ifndef COMPLETE_RANDOM_CITY_TRIPRISM_H
#define COMPLETE_RANDOM_CITY_TRIPRISM_H

#include "solid.h"
#include <iostream>

// This makes an isosceles triangular prism whose
// base is facing down

class TriPrism : public Solid
{
public:
    TriPrism();
    TriPrism(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    TriPrism(Point inputCenter, RGBAcolor inputColor,
            double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
            Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            Point* inputOwnerCenter);

    void initializeCorners();

    void move(double deltaX, double deltaY, double deltaZ);

    void rotate(double thetaX, double thetaY, double thetaZ);

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};


#endif //COMPLETE_RANDOM_CITY_TRIPRISM_H
