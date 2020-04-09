#include "buildingPlot.h"

BuildingPlot::BuildingPlot()
{
    plotType = Buildings;
}
BuildingPlot::BuildingPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, double seed) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    plotType = Buildings;
    makeBuilding(seed);
}
BuildingPlot::BuildingPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, Building b) :
        Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    building = b;
}

void BuildingPlot::makeBuilding(double seed)
{
    building = Building({center.x - sideLength/2, center.z - sideLength/2}, sideLength,
            100, {1,0,0,1}, {1,1,1,1}, Plain);
}

Building BuildingPlot::getBuilding() const
{
    return building;
}

void BuildingPlot::draw()
{
    building.draw();
}