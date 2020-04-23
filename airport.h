#ifndef COMPLETE_RANDOM_CITY_AIRPORT_H
#define COMPLETE_RANDOM_CITY_AIRPORT_H

#include "structs.h"
#include "building.h"
#include <memory>

class Airport
{
private:
    Point runwayStart;
    Point runwayEnd;
    int curNumPlanes;
    double runwayWidth;
    double plotSize;
    Point topLeft;
    std::shared_ptr<Building> controlTower;
public:
    Airport();
    Airport(Point inputTopLeft, double inputPlotSize);

    void initializeRunwayPoints();
    void initializeControlTower();

    // Getters
    Point getRunwayStart() const;
    Point getRunwayEnd() const;

    void draw() const;
    void tick();
};

#endif //COMPLETE_RANDOM_CITY_AIRPORT_H
