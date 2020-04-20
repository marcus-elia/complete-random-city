#ifndef COMPLETE_RANDOM_CITY_DIRIGIBLE_H
#define COMPLETE_RANDOM_CITY_DIRIGIBLE_H

#include "vehicle.h"
#include "ellipsoid.h"
#include "cylinder.h"
#include "rightTrapezoid.h"
#include "recPrism.h"

enum typeOfAirship {Blimp, Balloon};

class Dirigible : public Vehicle
{
private:
    RGBAcolor bodyColor;
    double length;
    double height;
    double width;
    typeOfAirship airshipType;

    // For movement
    Point target;
    double turnSpeed;
    bool needsToTurn;
public:
    Dirigible();
    Dirigible(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
    double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
    typeOfAirship inputAirshipType);

    void initializeSolids();

    // Getters
    double getLength() const;
    double getWidth() const;
    double getHeight() const;

    // Setters
    void setLength(double inputLength);
    void setHeight(double inputHeight);
    void setWidth(double inputWidth);

    void draw() const;

    void tick();

    void setRandomTarget();
};

#endif //COMPLETE_RANDOM_CITY_DIRIGIBLE_H
