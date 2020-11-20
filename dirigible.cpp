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
typeOfAirship inputAirshipType, RGBAcolor startingHitboxColor) : Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity, startingHitboxColor)
{
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    bodyColor = inputBodyColor;
    airshipType = inputAirshipType;
    initializeSolids();
    initializeHitbox(startingHitboxColor);
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
    else if(airshipType == Balloon)
    {
        Point solidCenter;
        Point solidLookingAt;
        solidCenter = location;
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(location, bodyColor, width, height, length,
                                                     {1,1,1,1},
                                                     solidCenter, solidLookingAt, 0, {0,0,0}, location)));
        solidCenter = {location.x, location.y - 1.3*height, location.z};
        solidLookingAt = {lookingAt.x, lookingAt.y - 1.3*height, lookingAt.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter, {.3, .1, .1,1},
                width/3, height/3, length/3,
                            {1,1,1,1}, solidCenter, solidLookingAt, 0,
                            {0,0,-1}, location)));
        initializeRopes();
    }
}
void Dirigible::initializeRopes()
{
    balloonCenter = location;
    basketTopCenter = {location.x, location.y - 1.3*height + width/6, location.z};
    double cubeSize = width/3;
    double sphereRadius = width/2;
    ropePoints.push_back({basketTopCenter.x + cubeSize/2, basketTopCenter.y, basketTopCenter.z + cubeSize/2});
    ropePoints.push_back({basketTopCenter.x - cubeSize/2, basketTopCenter.y, basketTopCenter.z + cubeSize/2});
    ropePoints.push_back({basketTopCenter.x - cubeSize/2, basketTopCenter.y, basketTopCenter.z - cubeSize/2});
    ropePoints.push_back({basketTopCenter.x + cubeSize/2, basketTopCenter.y, basketTopCenter.z - cubeSize/2});
    ropePoints.push_back({balloonCenter.x + sphereRadius/sqrt(2), balloonCenter.y, balloonCenter.z + sphereRadius/sqrt(2)});
    ropePoints.push_back({balloonCenter.x - sphereRadius/sqrt(2), balloonCenter.y, balloonCenter.z + sphereRadius/sqrt(2)});
    ropePoints.push_back({balloonCenter.x - sphereRadius/sqrt(2), balloonCenter.y, balloonCenter.z - sphereRadius/sqrt(2)});
    ropePoints.push_back({balloonCenter.x + sphereRadius/sqrt(2), balloonCenter.y, balloonCenter.z - sphereRadius/sqrt(2)});
}
void Dirigible::initializeHitbox(RGBAcolor startingHitboxColor)
{
    hitbox = std::make_shared<RecPrism>(RecPrism(location, startingHitboxColor, width, height, length, {1,1,1,1}));
    hitbox->rotate(0, xzAngle - 3*PI/2, 0);

    frontCollisionPoint = {location.x, location.y, location.z - 7*length/16};
    rotatePointAroundPoint(frontCollisionPoint, location, 0, xzAngle - 3*PI/2, 0);
    backCollisionPoint = {location.x, location.y, location.z + 7*length/16};
    rotatePointAroundPoint(backCollisionPoint, location, 0, xzAngle - 3*PI/2, 0);
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

void Dirigible::setXZAngle(double inputAngle)
{
    if(airshipType == Balloon)
    {
        for(int i = 0; i < 4; i++)
        {
            rotatePointAroundPoint(ropePoints[i], basketTopCenter, 0, inputAngle-xzAngle, 0);
            rotatePointAroundPoint(ropePoints[i+4], balloonCenter, 0, inputAngle-xzAngle, 0);
        }
    }
    Vehicle::setXZAngle(inputAngle);
}

void Dirigible::draw() const
{
    glLineWidth(1.5);
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
    glLineWidth(2.0);
    if(airshipType == Balloon)
    {
        drawRopes();
    }
}
void Dirigible::drawRopes() const
{
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for(int i = 0; i < 4; i++)
    {
        drawPoint(ropePoints[i]);   // Bottom of line
        drawPoint(ropePoints[i+4]); // Top of line
    }
    glEnd();
}
void Dirigible::drawHitbox() const
{
    hitbox->draw();
}

void Dirigible::tick()
{
    move(velocity.x, velocity.y, velocity.z);
    if(airshipType == Balloon)
    {
        for(int i = 0; i < 8; i++)
        {
            movePoint(ropePoints[i], velocity.x, velocity.y, velocity.z);
        }
        movePoint(basketTopCenter, velocity.x, velocity.y, velocity.z);
        movePoint(balloonCenter, velocity.x, velocity.y, velocity.z);
    }
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