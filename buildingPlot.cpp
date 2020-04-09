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

}

Building BuildingPlot::getBuilding() const
{
    return building;
}

void BuildingPlot::draw()
{
    building.draw();
}