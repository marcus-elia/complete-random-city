#include "solid.h"

Solid::Solid()
{
    xWidth = 1;
    yWidth = 1;
    zWidth = 1;
    lineColor = {1,1,1,1};
    initializeCorners();
}

Solid::Solid(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor)
{
    center = inputCenter;
    color = inputColor;
    xWidth = inputXWidth;
    yWidth = inputYWidth;
    zWidth = inputZWidth;
    lineColor = inputLineColor;
    initializeCorners();
    location = center;
    lookingAt = {0,0,0};
    speed = 0;
    velocity = {0,0,0};
    ownerCenter = center;
}

Solid::Solid(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point inputOwnerCenter) : MovableComponent(inputLocation, inputLookingAt, inputSpeed,
                     inputVelocity, inputOwnerCenter)
{
    center = inputCenter;
    color = inputColor;
    xWidth = inputXWidth;
    yWidth = inputYWidth;
    zWidth = inputZWidth;
    lineColor = inputLineColor;
    initializeCorners();
}

void Solid::initializeCorners()
{

}

Point Solid::getCenter() const
{
    return center;
}
RGBAcolor Solid::getColor() const
{
    return color;
}
std::vector<Point> Solid::getCorners() const
{
    return corners;
}
double Solid::getXWidth() const
{
    return xWidth;
}
double Solid::getYWidth() const
{
    return yWidth;
}
double Solid::getZWidth() const
{
    return zWidth;
}
RGBAcolor Solid::getLineColor() const
{
    return lineColor;
}


void Solid::setCenter(Point inputCenter)
{
    center = inputCenter;
}
void Solid::setColor(RGBAcolor inputColor)
{
    color = inputColor;
}
void Solid::setCorners(std::vector<Point> inputCorners)
{
    corners = inputCorners;
}
void Solid::setXWidth(double inputXWidth)
{
    xWidth = inputXWidth;
}
void Solid::setYWidth(double inputYWidth)
{
    yWidth = inputYWidth;
}
void Solid::setZWidth(double inputZWidth)
{
    zWidth = inputZWidth;
}
void Solid::setLineColor(RGBAcolor inputLineColor)
{
    lineColor = inputLineColor;
}




void Solid::move(double deltaX, double deltaY, double deltaZ)
{
    movePoint(center, deltaX, deltaY, deltaZ);
    movePoint(location, deltaX, deltaY, deltaZ);
    movePoint(lookingAt, deltaX, deltaY, deltaZ);
    for(Point &p : corners)
    {
        movePoint(p, deltaX, deltaY, deltaZ);
    }
}

void Solid::rotate(double thetaX, double thetaY, double thetaZ)
{
    for(Point &p : corners)
    {
        rotatePointAroundPoint(p, center, thetaX, thetaY, thetaZ);
    }
}

void Solid::rotateAroundOwner(double thetaX, double thetaY, double thetaZ)
{
    rotatePointAroundPoint(center, ownerCenter, thetaX, thetaY, thetaZ);
    rotatePointAroundPoint(location, ownerCenter, thetaX, thetaY, thetaZ);
    rotatePointAroundPoint(lookingAt, ownerCenter, thetaX, thetaY, thetaZ);
    for(Point &p : corners)
    {
        rotatePointAroundPoint(p, ownerCenter, thetaX, thetaY, thetaZ);
    }
    //rotate(thetaX, thetaY, thetaZ);
}