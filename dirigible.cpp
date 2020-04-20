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
        Point solidCenter;
        Point solidLookingAt;
        RGBAcolor gray = {0.5, 0.5, 0.5, 1};
        solidCenter = location;
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(location, bodyColor, width, height, length,
                {1,1,1,1},
                location, lookingAt, 0, {0,0,0}, location)));
        solidCenter = {location.x, location.y - height/2, location.z};
        solidLookingAt = {lookingAt.x, lookingAt.y - height/2 - height/8, lookingAt.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(solidCenter, gray,
                width/6, height/4, length/6,
                {1,1,1,1}, width/5, length/4,
                solidCenter, solidLookingAt, 0, {0,0,-1}, location)));
        // Fins
        solidCenter = {location.x, location.y + height/4, location.z + 5*length/16};
        solidLookingAt = {lookingAt.x, lookingAt.y, lookingAt.z};
        solids.push_back(std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter, gray, 3,
                5*height/8, length/4,
                {1,1,1,1}, length/8,
                solidCenter, solidLookingAt, 0, {0,0,-1}, location)));
        solidCenter = {location.x - width/4, location.y - height/4, location.z + 5*length/16};
        solidLookingAt = {lookingAt.x, lookingAt.y, lookingAt.z};
        std::shared_ptr<RightTrapezoid> leftFin = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter, gray, 2,
                                                  5*height/8, length/4,
                                                  {1,1,1,1}, length/8,
                                                  solidCenter, solidLookingAt, 0, {0,0,-1}, location));
        leftFin->rotate(0, 0, 2*PI/3);
        solids.push_back(leftFin);
        solidCenter = {location.x + width/4, location.y - height/4, location.z + 5*length/16};
        solidLookingAt = {lookingAt.x, lookingAt.y, lookingAt.z};
        std::shared_ptr<RightTrapezoid> rightFin = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter, gray, 2,
                                                   5*height/8, length/4,
                                                   {1,1,1,1}, length/8,
                                                    solidCenter, solidLookingAt, 0, {0,0,-1}, location));
        rightFin->rotate(0, 0, 4*PI/3);
        solids.push_back(rightFin);

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
    if(needsToTurn)
    {
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
            needsToTurn = false;
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

}

void Dirigible::setRandomTarget()
{
    double randAngle = (rand() % 100) * (2*PI / 100);
    double randDistance = (rand() % 50) + 100;
    target = {location.x + cos(randAngle)*randDistance, location.y, location.z + sin(randAngle)*randDistance};
    needsToTurn = true;
}