#include "vehicle.h"

Vehicle::Vehicle() : Movable()
{

}
Vehicle::Vehicle(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity) :
Movable(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    
}


std::vector<std::shared_ptr<Solid>> Vehicle::getSolids() const
{
    return solids;
}

void Vehicle::move(double deltaX, double deltaY, double deltaZ)
{
    // Move the vehicle
    movePoint(location, deltaX, deltaY, deltaZ);
    movePoint(lookingAt, deltaX, deltaY, deltaZ);

    // Move each solid
    for(std::shared_ptr<Solid> s : solids)
    {
        s->move(deltaX, deltaY, deltaZ);
    }
}
void Vehicle::rotate(double thetaX, double thetaY, double thetaZ)
{
    // Rotate the vehicle itself
    rotatePointAroundPoint(lookingAt, location, thetaX, thetaY, thetaZ);

    // Rotate each solid around the vehicle
    for(std::shared_ptr<Solid> s : solids)
    {
        s->rotateAroundOwner(thetaX, thetaY, thetaZ);
    }
}