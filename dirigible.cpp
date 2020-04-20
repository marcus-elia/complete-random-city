#include "dirigible.h"

Dirigible::Dirigible() : Vehicle()
{
    length = 20;
    height = 10;
    width = 10;
    target = location;
    turnSpeed = 0.01;
}
Dirigible::Dirigible(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
typeOfAirship inputAirshipType) : Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    bodyColor = inputBodyColor;
    airshipType = inputAirshipType;
    initializeSolids();
    target = location;
    turnSpeed = 0.01;
}

void Dirigible::initializeSolids()
{
    if(airshipType == Blimp)
    {
        solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(location, bodyColor, width, height, length,
                {1,1,1,1},
                location, lookingAt, 0, {0,0,0}, location)));
    }
}

// Getters
double Dirigible::getLength() const
{
    return length;
}
double Dirigible::getWidth() const
{
    return width;
}
double Dirigible::getHeight() const
{
    return height;
}

// Setters
void Dirigible::setLength(double inputLength)
{
    length = inputLength;
}
void Dirigible::setHeight(double inputHeight)
{
    height = inputHeight;
}
void Dirigible::setWidth(double inputWidth)
{
    width = inputWidth;
}

void Dirigible::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

void Dirigible::tick()
{
    move(velocity.x, velocity.y, velocity.z);
    if(distance2d(target, location) < 10)
    {
        setRandomTarget();
    }
    double angleToTurn = atan2(target.z - location.z, target.x - location.x) - xzAngle;
    while(angleToTurn < -PI)
    {
        angleToTurn += 2*PI;
    }
    while(angleToTurn > 2*PI)
    {
        angleToTurn -= 2*PI;
    }
    if(abs(angleToTurn) < turnSpeed)
    {
        setXZAngle(atan2(target.z - location.z, target.x - location.x));
        updateVelocity();
    }
    else if(angleToTurn < 0)
    {
        setXZAngle(xzAngle + turnSpeed);
        updateVelocity();
    }
    else if(angleToTurn > 0)
    {
        setXZAngle(xzAngle - turnSpeed);
        updateVelocity();
    }
}

void Dirigible::setRandomTarget()
{
    double randAngle = (rand() % 100) * (2*PI / 100);
    double randDistance = (rand() % 50) + 100;
    target = {location.x + cos(randAngle)*randDistance, location.y, location.z + sin(randAngle)*randDistance};
}