#ifndef COMPLETE_RANDOM_CITY_RECPRISM_H
#define COMPLETE_RANDOM_CITY_RECPRISM_H

#include "solid.h"
#include <iostream>

class RecPrism : public Solid
{
public:
    RecPrism();
    RecPrism(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    RecPrism(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point* inputOwnerCenter);

    void initializeCorners();

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

std::experimental::optional<Point> correctRectangularPrismCollision(Point p, int buffer, Point c,
                                                                    double xw, double yw, double zw);


#endif //COMPLETE_RANDOM_CITY_RECPRISM_H
