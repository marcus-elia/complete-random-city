#include "movable.h"

Movable::Movable()
{
    location = {0,0,0};
    setLookingAt({0,0,0});
    speed = 0;
    velocity = {0,0,0};
}
Movable::Movable(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity)
{
    location = inputLocation;
    setLookingAt(inputLookingAt);
    speed = inputSpeed;
    velocity = inputVelocity;
    updateVelocity();
}

void Movable::updateVelocity()
{
    double magnitude = sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
    if(magnitude == 0)
    {
        velocity = {lookingAt.x - location.x, lookingAt.y - location.y, lookingAt.z - location.z};
        magnitude = sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
    }
    velocity = {velocity.x/magnitude*speed, velocity.y/magnitude*speed, velocity.z/magnitude*speed};

}


// Getters
Point Movable::getLocation() const
{
    return location;
}
Point Movable::getLookingAt() const
{
    return lookingAt;
}
double Movable::getSpeed() const
{
    return speed;
}
Point Movable::getVelocity() const
{
    return velocity;
}
double Movable::getXZAngle() const
{
    return xzAngle;
}

// Setters
void Movable::setLocation(Point inputLocation)
{
    location = inputLocation;
}
void Movable::setLookingAt(Point inputLookingAt)
{
    lookingAt = inputLookingAt;
    xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);
    double prevAngle = atan2(velocity.z, velocity.x);
    rotatePointAroundPoint(velocity, {0,0,0}, 0, xzAngle - prevAngle, 0);
}
void Movable::setSpeed(double inputSpeed)
{
    speed = inputSpeed;
    updateVelocity();
}
void Movable::setVelocity(Point inputVelocity)
{
    velocity = inputVelocity;
}
void Movable::setXZAngle(double inputAngle)
{
    // Rotate the object to match the angle
    double thetaY = inputAngle - xzAngle;
    rotate(0, thetaY, 0);
    // Set the new angle
    xzAngle = inputAngle;

    double prevAngle = atan2(velocity.z, velocity.x);
    rotatePointAroundPoint(velocity, {0,0,0}, 0, xzAngle - prevAngle, 0);
}

void Movable::move(double deltaX, double deltaY, double deltaZ)
{
    movePoint(location, deltaX, deltaY, deltaZ);
    movePoint(lookingAt, deltaX, deltaY, deltaZ);
}
void Movable::rotate(double thetaX, double thetaY, double thetaZ)
{
    rotatePointAroundPoint(lookingAt, location, thetaX, thetaY, thetaZ);
}

void Movable::tick()
{

}
