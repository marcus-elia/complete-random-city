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

    // Put it in the right place
    Point2D start;
    if(exitDirection == North)
    {
        start = currentRoad->getUpExitPoint();
    }
    else if(exitDirection == East)
    {
        start = currentRoad->getRightExitPoint();
    }
    else if(exitDirection == South)
    {
        start = currentRoad->getDownExitPoint();
    }
    else if(exitDirection == West)
    {
        start = currentRoad->getLeftExitPoint();
    }
    move(start.x - location.x, 0, start.z - location.z);
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

void Car::updateTurnPoints()
{
    if(intersectionDirection == LeftTurn)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsLeftNorth(speed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsLeftEast(speed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsLeftSouth(speed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsLeftWest(speed);
        }
    }
    else if(intersectionDirection == RightTurn)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsRightNorth(speed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsRightEast(speed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsRightSouth(speed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsRightWest(speed);
        }
    }
    else if(intersectionDirection == Circle)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsCircleNorth(speed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsCircleEast(speed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsCircleSouth(speed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsCircleWest(speed);
        }
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

    glBegin(GL_QUADS);
    glColor4f(0,1,1,1);
    Point2D loc = currentRoad->getCenter();
    drawPoint({loc.x + 8.0, 10, loc.z + 8.0});
    drawPoint({loc.x - 8.0, 10, loc.z + 8.0});
    drawPoint({loc.x - 8.0, 10, loc.z - 8.0});
    drawPoint({loc.x + 8.0, 10, loc.z - 8.0});
    glEnd();
    glEnable(GL_CULL_FACE);
}


void Car::checkStatusExiting()
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
        currentRoad = currentRoad->getLeftRoad().value();
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
void Car::checkStatusIntersection()
{
    if(turnPoints.empty())
    {
        currentStatus = Exiting;
        alignWithDirection(exitDirection);
        updateVelocity();
    }
    else
    {
        Point2D nextLocation = turnPoints.back();
        turnPoints.pop_back();
        double theta = atan2(nextLocation.z - location.z, nextLocation.x - location.x);
        velocity = {nextLocation.x - location.x, 0, nextLocation.z - location.z};
        rotate(0, theta, 0);
    }
}
void Car::checkStatusApproaching()
{
    if((approachDirection == North && location.z <= currentRoad->getSouthIntersectionEdge()) ||
       (approachDirection == West && location.x <= currentRoad->getEastIntersectionEdge()) ||
       (approachDirection == South && location.z >= currentRoad->getNorthIntersectionEdge()) ||
       (approachDirection == East && location.x >= currentRoad->getWestIntersectionEdge()))
    {
        currentStatus = Intersection;
        updateTurnPoints();
    }
}




void Car::tick()
{
    move(velocity.x, velocity.y, velocity.z);

    // If we are leaving the RoadPlot, check if we've reached the border
    if(currentStatus == Exiting)
    {
        checkStatusExiting();
    }
    else if(currentStatus == Approaching)
    {
        checkStatusApproaching();
    }
    else if(currentStatus == Intersection)
    {
        checkStatusIntersection();
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