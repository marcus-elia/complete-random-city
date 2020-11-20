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

    // If it is a hot air balloon, it has 4 ropes
    std::vector<Point> ropePoints;
    // For rotating
    Point balloonCenter;
    Point basketTopCenter;
public:
    Dirigible();
    Dirigible(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
    double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
    typeOfAirship inputAirshipType, RGBAcolor startingHitboxColor);

    void initializeSolids();
    void initializeRopes();
    void initializeHitbox(RGBAcolor startingHitboxColor);

    // Getters
    double getLength() const;
    double getWidth() const;
    double getHeight() const;

    // Setters
    void setLength(double inputLength);
    void setHeight(double inputHeight);
    void setWidth(double inputWidth);
    void setXZAngle(double inputAngle);

    void draw() const;
    void drawRopes() const;
    void drawHitbox() const;

    void tick();

    void setRandomTarget();
};

#endif //COMPLETE_RANDOM_CITY_DIRIGIBLE_H
