#ifndef COMPLETE_RANDOM_CITY_CAR_H
#define COMPLETE_RANDOM_CITY_CAR_H

#include "vehicle.h"
#include "roadPlot.h"

class Car : public Vehicle
{
private:
    RGBAcolor bodyColor;
    double length;
    double height;
    double width;
    RoadPlot* currentRoad;
public:
    Car();
    Car(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            double inputLength, double inputHeight, double inputWidth, RGBAcolor inputBodyColor,
            RoadPlot* inputCurrentRoad);

    void initializeSolids();

    // Getters
    double getLength() const;
    double getWidth() const;
    double getHeight() const;
    RoadPlot* getCurrentRoad() const;

    // Setters
    void setLength(double inputLength);
    void setHeight(double inputHeight);
    void setWidth(double inputWidth);
    void setCurrentRoad(RoadPlot* inputCurrentRoad);

    void draw() const;

    void tick();
};

#endif //COMPLETE_RANDOM_CITY_CAR_H
