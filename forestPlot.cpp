#include "forestPlot.h"

ForestPlot::ForestPlot() : Plot()
{
    level = 0;
}
ForestPlot::ForestPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    level = 0;
}

void ForestPlot::initializeTrees()
{

}

int ForestPlot::getLevel() const
{
    return level;
}
void ForestPlot::setLevel(int inputLevel)
{
    level = inputLevel;
}

void ForestPlot::draw()
{
    
}