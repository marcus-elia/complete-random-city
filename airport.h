#ifndef COMPLETE_RANDOM_CITY_AIRPORT_H
#define COMPLETE_RANDOM_CITY_AIRPORT_H

#include "structs.h"
#include "building.h"
#include "airplane.h"
#include <memory>
#include <vector>

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
    std::vector<std::shared_ptr<Airplane>> airplanes;
public:
    Airport();
    Airport(Point inputTopLeft, double inputPlotSize);

    void initializeRunwayPoints();
    void initializeControlTower();

    // Planes
    void createAirplane(RGBAcolor startingHitboxColor);

    // Getters
    Point getRunwayStart() const;
    Point getRunwayEnd() const;

    void draw() const;
    void tick();
};

#endif //COMPLETE_RANDOM_CITY_AIRPORT_H
