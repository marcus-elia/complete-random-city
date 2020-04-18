#ifndef COMPLETE_RANDOM_CITY_CAR_H
#define COMPLETE_RANDOM_CITY_CAR_H

#include "vehicle.h"
#include "roadPlot.h"
#include "structs.h"
#include "cylinder.h"
#include "frustum.h"

enum typeOfCar {Sedan, SUV, PickupTruck};

class Car : public Vehicle
{
private:
    RGBAcolor bodyColor;
    double length;
    double height;
    double width;
    typeOfCar carType;

    // Driving things
    RoadPlot* currentRoad;
    // Which way the car has to go in the middle of the current roadplot
    IntersectionDirection intersectionDirection;
    // Which way the car enters/leaves an intersection
    DrivingDirection approachDirection; // example: North means car is facing north as it approaches
    DrivingDirection exitDirection;     // example: North means car is facing north as it leaves
    // What phase of the RoadPlot is the car in
    RoadStatus currentStatus;

    // If the car is turning, we need points on the curve to guide it
    std::vector<Point2D> turnPoints;
    // How much to rotate by each frame when turning
    double deltaTheta;
public:
    Car();
    Car(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
            typeOfCar inputCarType, RoadPlot* inputCurrentRoad);

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
    // Make the car turn to be facing the given cardinal direction
    void alignWithDirection(DrivingDirection input);
    // Gets a vector of turn points from the RoadPlot
    void updateTurnPoints();
    // Sets how much we turn per tick depending on what kind of turn it is
    void updateDeltaTheta();

    // Check for when the Car needs to change status, and update things accordingly
    void checkStatusExiting();
    void checkStatusIntersection();
    void checkStatusApproaching();

    void draw() const;

    void tick();
};

DrivingDirection oppositeDirection(DrivingDirection input);

#endif //COMPLETE_RANDOM_CITY_CAR_H
