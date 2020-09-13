#include "car.h"

Car::Car() : Vehicle()
{
    length = 20;
    height = 10;
    width = 10;
}
Car::Car(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor, typeOfCar inputCarType,
RoadPlot* inputCurrentRoad) : Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    bodyColor = inputBodyColor;
    carType = inputCarType;
    currentRoad = inputCurrentRoad;
    initializeSolids();
    initializeHitbox();
    initializeDirections();
}

void Car::initializeSolids()
{
    Point solidCenter;
    Point solidLookingAt;
    if(carType == Sedan)
    {
        // Body
        solidCenter = {location.x, height/4 + 2, location.z};
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter,
                bodyColor, width, height/2, length,{1,1,1,1},
                 solidCenter, solidLookingAt,  0,
                {0,0,0},  location)));
        // Cabin
        solidCenter = {location.x, height/2 + 2 + height/4, location.z};
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<Frustum>(Frustum(solidCenter,
                                                             bodyColor, width, height/2, width,{1,1,1,1},
                                                             width-1, width-3,
                                                             solidCenter, solidLookingAt,  0,
                                                             {0,0,0},  location)));
    }
    else if(carType == SUV)
    {
        // Body
        solidCenter = {location.x, height/4 + 3, location.z};
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter,
                                                             bodyColor, width, height/2, length,{1,1,1,1},
                                                             solidCenter, solidLookingAt,  0,
                                                             {0,0,0},  location)));
        // Cabin
        solidCenter = {location.x, 3 + height/2 + height/4, location.z + length/8};
        solidLookingAt = {lookingAt.x, lookingAt.y + height/2 + height/4, lookingAt.z};
        solids.push_back(std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
                bodyColor, width, height/2, 3*length/4, {1,1,1,1}, 5*length/8,
                solidCenter, solidLookingAt, 0, {0,0,0}, location)));

    }
    else if(carType == PickupTruck)
    {
        // Body
        solidCenter = {location.x, height/4 + 3, location.z};
        solidLookingAt = lookingAt;
        solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter,
                                                             bodyColor, width, height/2, length,{1,1,1,1},
                                                             solidCenter, solidLookingAt,  0,
                                                             {0,0,0},  location)));
        // Cabin
        solidCenter = {location.x, 3 + height/2 + height/4, location.z - length/6};
        solidLookingAt = {lookingAt.x, lookingAt.y + height/2 + height/4, lookingAt.z};
        solids.push_back(std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
                                                                         bodyColor, width, height/2, length/3, {1,1,1,1}, length/6,
                                                                         solidCenter, solidLookingAt, 0, {0,0,0}, location)));
    }



    // Add in the wheels
    // Front Left Wheel
    solidCenter = {location.x - width/2 - 1, 3, location.z - length/2 + 4};
    solidLookingAt = {lookingAt.x + solidCenter.x - location.x, 3, lookingAt.z + solidCenter.z - location.z};
    std::shared_ptr<Cylinder> frontLeftWheel = std::make_shared<Cylinder>(Cylinder(solidCenter, {0,0,0,1},
                                                                                   6, 2, 6, {1,1,1,1},
                                                                                   solidCenter, solidLookingAt, 0, {0,0,0},
                                                                                   location));
    frontLeftWheel->rotate(0, 0, PI/2);
    solids.push_back(frontLeftWheel);
    // Front Right Wheel
    solidCenter = {location.x + width/2 + 1, 3, location.z - length/2 + 4};
    solidLookingAt = {lookingAt.x + solidCenter.x - location.x, 3, lookingAt.z + solidCenter.z - location.z};
    std::shared_ptr<Cylinder> frontRightWheel = std::make_shared<Cylinder>(Cylinder(solidCenter, {0,0,0,1},
                                                                                    6, 2, 6, {1,1,1,1},
                                                                                    solidCenter, solidLookingAt, 0, {0,0,0},
                                                                                    location));
    frontRightWheel->rotate(0, 0, PI/2);
    solids.push_back(frontRightWheel);
    // Back Left Wheel
    solidCenter = {location.x - width/2 - 1, 3, location.z + length/2 - 4};
    solidLookingAt = {lookingAt.x + solidCenter.x - location.x, 3, lookingAt.z + solidCenter.z - location.z};
    std::shared_ptr<Cylinder> backLeftWheel = std::make_shared<Cylinder>(Cylinder(solidCenter, {0,0,0,1},
                                                                                  6, 2, 6, {1,1,1,1},
                                                                                  solidCenter, solidLookingAt, 0, {0,0,0},
                                                                                  location));
    backLeftWheel->rotate(0, 0, PI/2);
    solids.push_back(backLeftWheel);
    // Back Right Wheel
    solidCenter = {location.x + width/2 + 1, 3, location.z + length/2 - 4};
    solidLookingAt = {lookingAt.x + solidCenter.x - location.x, 3, lookingAt.z + solidCenter.z - location.z};
    std::shared_ptr<Cylinder> backRightWheel = std::make_shared<Cylinder>(Cylinder(solidCenter, {0,0,0,1},
                                                                                   6, 2, 6, {1,1,1,1},
                                                                                   solidCenter, solidLookingAt, 0, {0,0,0},
                                                                                   location));
    backRightWheel->rotate(0, 0, PI/2);
    solids.push_back(backRightWheel);

}
void Car::initializeHitbox()
{
    hitbox = RecPrism(location, {1,0.9,0.9,0.5}, width, height, length, {1,1,1,1});
    hitbox.rotate(0, xzAngle - 3*PI/2, 0);
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
    double turnSpeed = speed/2;
    if(intersectionDirection == LeftTurn)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsLeftNorth(turnSpeed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsLeftEast(turnSpeed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsLeftSouth(turnSpeed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsLeftWest(turnSpeed);
        }
    }
    else if(intersectionDirection == RightTurn)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsRightNorth(turnSpeed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsRightEast(turnSpeed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsRightSouth(turnSpeed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsRightWest(turnSpeed);
        }
    }
    else if(intersectionDirection == Circle)
    {
        if(approachDirection == North)
        {
            turnPoints = currentRoad->getTurnPointsCircleNorth(turnSpeed);
        }
        else if(approachDirection == East)
        {
            turnPoints = currentRoad->getTurnPointsCircleEast(turnSpeed);
        }
        else if(approachDirection == South)
        {
            turnPoints = currentRoad->getTurnPointsCircleSouth(turnSpeed);
        }
        else if(approachDirection == West)
        {
            turnPoints = currentRoad->getTurnPointsCircleWest(turnSpeed);
        }
    }
}

void Car::updateDeltaTheta()
{
    if(turnPoints.empty())
    {
        deltaTheta = 0;
        return;
    }
    int n = turnPoints.size();
    double angleAmount; // how much the car turns in total through the intersection
    if(approachDirection == North)
    {
        if(exitDirection == East)
        {
            angleAmount = PI/2;
        }
        else if(exitDirection == South)
        {
            angleAmount = -5.3556;
        }
        else if(exitDirection == West)
        {
            angleAmount = - PI/2;
        }
    }
    else if(approachDirection == East)
    {
        if(exitDirection == North)
        {
            angleAmount = -PI/2;
        }
        else if(exitDirection == South)
        {
            angleAmount = PI/2;
        }
        else if(exitDirection == West)
        {
            angleAmount = -5.3556;
        }
    }
    else if(approachDirection == South)
    {
        if(exitDirection == North)
        {
            angleAmount = -5.3556;
        }
        else if(exitDirection == East)
        {
            angleAmount = -PI/2;
        }
        else if(exitDirection == West)
        {
            angleAmount = PI/2;
        }
    }
    else if(approachDirection == West)
    {
        if(exitDirection == North)
        {
            angleAmount = PI/2;
        }
        else if(exitDirection == East)
        {
            angleAmount = -5.3556;
        }
        else if(exitDirection == South)
        {
            angleAmount = -PI/2;
        }
    }
    deltaTheta = angleAmount / n;
}

void Car::draw() const
{
    glLineWidth(1.0);
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
    glLineWidth(2.0);
    glEnable(GL_CULL_FACE);
}

void Car::drawHitbox() const
{
    hitbox.draw();
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
    else if(exitDirection == South && location.z > currentRoad->getSouthEdge())
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
        //double theta = atan2(nextLocation.z - location.z, nextLocation.x - location.x);
        //rotate(0, theta, 0);
        setXZAngle(xzAngle + deltaTheta);
        velocity = {nextLocation.x - location.x, 0, nextLocation.z - location.z};
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
        updateDeltaTheta();
        if(intersectionDirection == Circle)
        {
            setXZAngle(xzAngle + PI/2  - atan(PI/4*0.23));
        }
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