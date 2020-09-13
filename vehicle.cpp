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

void Vehicle::setLookingAt(Point inputLookingAt)
{
    lookingAt = inputLookingAt;
    xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);
    double prevAngle = atan2(velocity.z, velocity.x);
    rotatePointAroundPoint(velocity, {0,0,0}, 0, xzAngle - prevAngle, 0);
    for(std::shared_ptr<Solid> s : solids)
    {
        s->setLookingAt(inputLookingAt);
    }
}

void Vehicle::setXZAngle(double inputAngle)
{
    // Rotate the object to match the angle
    double thetaY = inputAngle - xzAngle;
    //rotate(0, thetaY, 0);
    rotatePointAroundPoint(lookingAt, location, 0, thetaY, 0);
    // Set the new angle
    xzAngle = inputAngle;

    double prevAngle = atan2(velocity.z, velocity.x);
    rotatePointAroundPoint(velocity, {0,0,0}, 0, xzAngle - prevAngle, 0);

    for(std::shared_ptr<Solid> s : solids)
    {
        //s->setXZAngle(inputAngle);
        s->rotateAroundOwner(0, thetaY, 0);
    }
    hitbox->rotate(0, thetaY, 0);
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
        s->moveOwnerCenter(deltaX, deltaY, deltaZ);
    }
    hitbox->move(deltaX, deltaY, deltaZ);
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
    hitbox->rotate(thetaX, thetaY, thetaZ);
}


std::experimental::optional<Point> Vehicle::correctCollision(Point p, double buffer) const
{
    return correctRectangularPrism(p, buffer, hitbox->getCenter(), hitbox->getXWidth(), hitbox->getYWidth(), hitbox->getZWidth(), hitbox->getXZAngle());
}