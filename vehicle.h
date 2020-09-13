#ifndef COMPLETE_RANDOM_CITY_VEHICLE_H
#define COMPLETE_RANDOM_CITY_VEHICLE_H

// A vehicle is a movable which has a list of solids

#include "movable.h"
#include "solid.h"
#include "recPrism.h"
#include <vector>
#include <memory>

class Vehicle : public Movable
{
protected:
    std::vector<std::shared_ptr<Solid>> solids;
    std::shared_ptr<RecPrism> hitbox;

    Point frontCollisionPoint, backCollisionPoint;
public:
    Vehicle();
    Vehicle(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity, RGBAcolor startingHitboxColor);

    virtual void initializeSolids() = 0;
    virtual void initializeHitbox(RGBAcolor startingHitboxColor) = 0;

    std::vector<std::shared_ptr<Solid>> getSolids() const;
    Point getFrontCollisionPoint() const;
    Point getBackCollisionPoint() const;

    // Rotating needs to be overridden because the Solids also need to turn when
    // the vehicle does
    void setLookingAt(Point inputLookingAt);
    void setXZAngle(double inputAngle);

    void move(double deltaX, double deltaY, double deltaZ);
    void rotate(double thetaX, double thetaY, double thetaZ);

    virtual void drawHitbox() const = 0;

    std::experimental::optional<Point> correctCollision(Point p, double buffer) const;
};

#endif //COMPLETE_RANDOM_CITY_VEHICLE_H
