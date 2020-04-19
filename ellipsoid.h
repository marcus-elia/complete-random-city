#ifndef COMPLETE_RANDOM_CITY_ELLIPSOID_H
#define COMPLETE_RANDOM_CITY_ELLIPSOID_H

#include "solid.h"

class Ellipsoid : public Solid
{
private:
    // The smoothness
    int pointsPerRing;
    int numRings;
public:
    Ellipsoid();
    Ellipsoid(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    Ellipsoid(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
    Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
    Point inputOwnerCenter);

    void initializeCorners();

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

#endif //COMPLETE_RANDOM_CITY_ELLIPSOID_H
