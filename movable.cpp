#include "movable.h"

Movable::Movable()
{
    location = {0,0,0};
    lookingAt = {0,0,0};
    speed = 0;
    velocity = {0,0,0};
}
Movable::Movable(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity)
{
    location = inputLocation;
    lookingAt = inputLookingAt;
    speed = inputSpeed;
    velocity = inputVelocity;
    updateVelocity();
}

void Movable::updateVelocity()
{
    double magnitude = sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
    if(magnitude > 0)
    {
        velocity = {velocity.x/magnitude*speed, velocity.y/magnitude*speed, velocity.z/magnitude*speed};
    }
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

// Setters
void Movable::setLocation(Point inputLocation)
{
    location = inputLocation;
}
void Movable::setLookingAt(Point inputLookingAt)
{
    lookingAt = inputLookingAt;
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

void Movable::move(double deltaX, double deltaY, double deltaZ)
{
    location.x += deltaX;
    lookingAt.x += deltaX;
    location.y += deltaY;
    lookingAt.y += deltaY;
    location.z += deltaZ;
    lookingAt.z += deltaZ;
}
void Movable::rotate(double thetaX, double thetaY, double thetaZ)
{
    rotatePointAroundPoint(lookingAt, location, thetaX, thetaY, thetaZ);
}


void rotatePointAroundPoint(Point &p, const Point &pBase, double thetaX, double thetaY, double thetaZ)
{
    // Store the previous coordinates during calculations
    double prevX = 0, prevY = 0, prevZ = 0;

    // Translate pBase to the origin by moving p
    p.x -= pBase.x;
    p.y -= pBase.y;
    p.z -= pBase.z;

    // Rotate around x-axis
    prevY = p.y;
    prevZ = p.z;
    p.y = prevY * cos(thetaX) - prevZ * sin(thetaX);
    p.z = prevY * sin(thetaX) + prevZ * cos(thetaX);

    // Rotate around y-axis
    prevX = p.x;
    prevZ = p.z;
    p.x = prevX * cos(thetaY) - prevZ * sin(thetaY);
    p.z = prevX * sin(thetaY) + prevZ * cos(thetaY);

    // Rotate around z-axis
    prevX = p.x;
    prevY = p.y;
    p.x = prevX * cos(thetaZ) - prevY * sin(thetaZ);
    p.y = prevX * sin(thetaZ) + prevY * cos(thetaZ);

    // Translate back
    p.x += pBase.x;
    p.y += pBase.y;
    p.z += pBase.z;
}