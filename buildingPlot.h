#ifndef COMPLETE_RANDOM_CITY_BUILDINGPLOT_H
#define COMPLETE_RANDOM_CITY_BUILDINGPLOT_H

#include "plot.h"
#include "building.h"

class BuildingPlot : public Plot
{
private:
    Building building;
public:
    BuildingPlot();
    BuildingPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, double seed);
    BuildingPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, Building b);

    void makeBuilding(double seed);

    Building getBuilding() const;

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_BUILDINGPLOT_H
