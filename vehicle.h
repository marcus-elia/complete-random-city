#ifndef COMPLETE_RANDOM_CITY_VEHICLE_H
#define COMPLETE_RANDOM_CITY_VEHICLE_H

// A vehicle is a movable which has a list of solids

#include "movable.h"
#include "solid.h"
#include <vector>
#include <memory>

class Vehicle : public Movable
{
protected:
    std::vector<std::shared_ptr<Solid>> solids;
public:
    Vehicle();
    Vehicle(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity);

    virtual void initializeSolids() = 0;

    std::vector<std::shared_ptr<Solid>> getSolids() const;

    void move(double deltaX, double deltaY, double deltaZ);
    void rotate(double thetaX, double thetaY, double thetaZ);
};

#endif //COMPLETE_RANDOM_CITY_VEHICLE_H
