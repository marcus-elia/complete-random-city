#ifndef COMPLETE_RANDOM_CITY_AIRPLANE_H
#define COMPLETE_RANDOM_CITY_AIRPLANE_H

#include "vehicle.h"
#include "recPrism.h"
#include "rightTrapezoid.h"
#include "ellipsoid.h"
#include "cylinder.h"
#include "frustum.h"

class Airplane : public Vehicle
{
private:
    double cruisingAltitude;
    Point runwayStart;
    Point runwayEnd;
    bool isTakingOff;
    double length;
    double height;
    double width;
public:
    Airplane();
    Airplane(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            double inputCruisingAltitude, Point inputRunwayStart, Point inputRunwayEnd,
             double inputLength, double inputHeight, double inputWidth, RGBAcolor startingHitboxColor);

    void startOnRunway();
    void initializeSolids();
    void initializeHitbox(RGBAcolor startingHitboxColor);

    void tick();

    void draw() const;
    void drawHitbox() const;
};

#endif //COMPLETE_RANDOM_CITY_AIRPLANE_H
