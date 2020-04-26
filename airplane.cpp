#include "airplane.h"

Airplane::Airplane() : Vehicle()
{
    cruisingAltitude = 300;
    runwayStart = {0,0, 0};
    runwayEnd = {0, 0, -100};
    length = 20;
    height = 10;
    width = 10;
    isTakingOff = true;
    startOnRunway();
    initializeSolids();
}
Airplane::Airplane(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
double inputCruisingAltitude, Point inputRunwayStart, Point inputRunwayEnd,
double inputLength, double inputHeight, double inputWidth)
: Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    cruisingAltitude = inputCruisingAltitude;
    runwayStart = inputRunwayStart;
    runwayEnd = inputRunwayEnd;
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    isTakingOff = true;
    startOnRunway();
    initializeSolids();
}

void Airplane::startOnRunway()
{
    location = runwayStart;
    double runwayAngle = atan2(runwayEnd.z - runwayStart.z, runwayEnd.x - runwayStart.x);
    setXZAngle(runwayAngle);
}

void Airplane::initializeSolids()
{
    solids.push_back(std::make_shared<RecPrism>(RecPrism(location,
                                                         {1,0,0,0}, width, height, length,{1,1,1,1},
                                                         location, lookingAt,  0,
                                                         {0,0,0},  location)));
}

void Airplane::tick()
{
    if(isTakingOff)
    {
        move(velocity.x, velocity.y, velocity.z);
        velocity.y += 0.1;
        speed += 0.1;
        updateVelocity();

        if(distance2d(location, runwayEnd) < speed)
        {
            isTakingOff = false;
            velocity.y = 0;
        }
    }
    else
    {
        move(velocity.x, velocity.y, velocity.z);
        if(distance2d(location, runwayStart) > 512)
        {
            rotate(0, 0.05, 0);
        }
    }
}