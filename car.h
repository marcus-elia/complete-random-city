#ifndef COMPLETE_RANDOM_CITY_CAR_H
#define COMPLETE_RANDOM_CITY_CAR_H

#include "vehicle.h"
#include "roadPlot.h"
#include "structs.h"


class Car : public Vehicle
{
private:
    RGBAcolor bodyColor;
    double length;
    double height;
    double width;

    // Driving things
    RoadPlot* currentRoad;
    // Which way the car has to go in the middle of the current roadplot
    IntersectionDirection intersectionDirection;
    // Which way the car enters/leaves an intersection
    DrivingDirection approachDirection; // example: North means car is facing north as it approaches
    DrivingDirection exitDirection;     // example: North means car is facing north as it leaves
    // What phase of the RoadPlot is the car in
    RoadStatus currentStatus;
public:
    Car();
    Car(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
            RoadPlot* inputCurrentRoad);

    void initializeSolids();
    void initializeDirections();

    // Getters
    double getLength() const;
    double getWidth() const;
    double getHeight() const;
    RoadPlot* getCurrentRoad() const;
    IntersectionDirection getIntersectionDirection() const;
    DrivingDirection getApproachDirection() const;
    DrivingDirection getExitDirection() const;
    RoadStatus getCurrentStatus() const;

    // Setters
    void setLength(double inputLength);
    void setHeight(double inputHeight);
    void setWidth(double inputWidth);
    void setCurrentRoad(RoadPlot* inputCurrentRoad);
    void setIntersectionDirection(IntersectionDirection input);
    void setApproachDirection(DrivingDirection input);
    void setExitDirection(DrivingDirection input);
    void setCurrentStatus(RoadStatus input);

    // Helper functions for driving
    IntersectionDirection determineIntersectionDirection(DrivingDirection approach, DrivingDirection exit) const;

    void draw() const;

    void tick();
};

#endif //COMPLETE_RANDOM_CITY_CAR_H
