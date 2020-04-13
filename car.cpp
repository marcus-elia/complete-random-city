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
    initializeDirections();
}

void Car::initializeSolids()
{
    solids.push_back(std::make_shared<RecPrism>(RecPrism(location, bodyColor, width, height, length,
            {1,1,1,1}, location, lookingAt,  0,
            {0,0,0},  location)));
}
void Car::initializeDirections()
{
    exitDirection = currentRoad->getRandomDirection();
    alignWithDirection(exitDirection);
    currentStatus = Exiting;
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
IntersectionDirection Car::getIntersectionDirection() const
{
    return intersectionDirection;
}
DrivingDirection Car::getApproachDirection() const
{
    return approachDirection;
}
DrivingDirection Car::getExitDirection() const
{
    return exitDirection;
}
RoadStatus Car::getCurrentStatus() const
{
    return currentStatus;
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
void Car::setIntersectionDirection(IntersectionDirection input)
{
    intersectionDirection = input;
}
void Car::setApproachDirection(DrivingDirection input)
{
    approachDirection = input;
}
void Car::setExitDirection(DrivingDirection input)
{
    exitDirection = input;
}
void Car::setCurrentStatus(RoadStatus input)
{
    currentStatus = input;
}


IntersectionDirection Car::determineIntersectionDirection(DrivingDirection approach, DrivingDirection exit) const
{
    switch(approach)
    {
        case North:
            switch(exit)
            {
                case North:
                    return Straight;
                case East:
                    return RightTurn;
                case South:
                    return Circle;
                default:
                    return LeftTurn;
            }
        case East:
            switch(exit)
            {
                case North:
                    return LeftTurn;
                case East:
                    return Straight;
                case South:
                    return RightTurn;
                default:
                    return Circle;
            }
        case South:
            switch(exit)
            {
                case North:
                    return Circle;
                case East:
                    return LeftTurn;
                case South:
                    return Straight;
                default:
                    return RightTurn;
            }
        default:
            switch(exit)
            {
                case North:
                    return RightTurn;
                case East:
                    return Circle;
                case South:
                    return LeftTurn;
                default:
                    return Straight;
            }
    }
}

void Car::alignWithDirection(DrivingDirection input)
{
    if(input == North)
    {
        setXZAngle(3*PI/2);
    }
    else if(input == East)
    {
        setXZAngle(0);
    }
    else if(input == South)
    {
        setXZAngle(PI/2);
    }
    else
    {
        setXZAngle(PI);
    }
}



void Car::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
    // Debug
    glDisable(GL_CULL_FACE);
    if(currentStatus == Approaching)
    {
        glColor4f(1,0,0,1);
    }
    else
    {
        glColor4f(0,0,1,1);
    }
    glBegin(GL_QUADS);
    drawPoint({location.x + 8, 30, location.z + 8});
    drawPoint({location.x - 8, 30, location.z + 8});
    drawPoint({location.x - 8, 30, location.z - 8});
    drawPoint({location.x + 8, 30, location.z - 8});
    glEnd();
    glEnable(GL_CULL_FACE);
}

void Car::tick()
{
    move(velocity.x, velocity.y, velocity.z);

    // If we are leaving the RoadPlot, check if we've reached the border
    if(currentStatus == Exiting)
    {
        bool enteredNewRoad = false;
        if(exitDirection == North && location.z < currentRoad->getNorthEdge())
        {
            currentRoad = currentRoad->getUpRoad().value();
            enteredNewRoad = true;
        }
        else if(exitDirection == East && location.x > currentRoad->getEastEdge())
        {
            currentRoad = currentRoad->getRightRoad().value();
            enteredNewRoad = true;
        }
        else if(exitDirection == South && location.y > currentRoad->getSouthEdge())
        {
            currentRoad = currentRoad->getDownRoad().value();
            enteredNewRoad = true;
        }
        else if(exitDirection == West && location.x < currentRoad->getWestEdge())
        {
            currentRoad = currentRoad->getRightRoad().value();
            enteredNewRoad = true;
        }

        if(enteredNewRoad)
        {
            currentStatus = Approaching;
            approachDirection = exitDirection;
            exitDirection = currentRoad->getRandomDirectionExcept(oppositeDirection(approachDirection));
            intersectionDirection = determineIntersectionDirection(approachDirection, exitDirection);
        }
    }
    else if(currentStatus == Approaching)
    {
        bool reachedCenter = false;
        if((approachDirection == North && location.z <= currentRoad->getCenter().z) ||
                (approachDirection == West && location.x <= currentRoad->getCenter().x) ||
                (approachDirection == South && location.z >= currentRoad->getCenter().z) ||
                (approachDirection == East && location.x >= currentRoad->getCenter().x))
        {
            currentStatus = Exiting;
            alignWithDirection(exitDirection);
        }
    }
}

DrivingDirection oppositeDirection(DrivingDirection input)
{
    if(input == North)
    {
        return South;
    }
    else if(input == West)
    {
        return East;
    }
    else if(input == South)
    {
        return North;
    }
    else
    {
        return West;
    }
}