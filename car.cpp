#include "car.h"

Car::Car() : Vehicle()
{
    length = 20;
    height = 10;
    width = 10;
}
Car::Car(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
RoadPlot* inputCurrentRoad) : Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    bodyColor = inputBodyColor;
    currentRoad = inputCurrentRoad;
    initializeSolids();
}

void Car::initializeSolids()
{
    solids.push_back(std::make_shared<RecPrism>(RecPrism(location, bodyColor, width, height, length,
            {1,1,1,1})));
}

// Getters
double Car::getLength() const
{
    return length;
}
double Car::getWidth() const
{
    return width;
}
double Car::getHeight() const
{
    return height;
}
RoadPlot* Car::getCurrentRoad() const
{
    return currentRoad;
}

// Setters
void Car::setLength(double inputLength)
{
    length = inputLength;
}
void Car::setHeight(double inputHeight)
{
    height = inputHeight;
}
void Car::setWidth(double inputWidth)
{
    width = inputWidth;
}
void Car::setCurrentRoad(RoadPlot* inputCurrentRoad)
{
    currentRoad = inputCurrentRoad;
}

void Car::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

void Car::tick()
{
    move(velocity.x, velocity.y, velocity.z);
}