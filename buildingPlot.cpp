#include "buildingPlot.h"

BuildingPlot::BuildingPlot()
{

}
BuildingPlot::BuildingPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, double seed) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
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