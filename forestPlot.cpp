#include "forestPlot.h"

ForestPlot::ForestPlot() : Plot()
{
    plotType = Forest;
    level = 0;
    initializeTrees();
}
ForestPlot::ForestPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, int inputLevel) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    plotType = Forest;
    level = inputLevel;
    initializeTrees();
}

void ForestPlot::initializeTrees()
{
    trees = std::vector<Tree>();
    //int numTreeAttempts = 2*level + (rand() % 3);
    int numTreeAttempts = level*1.25;
    for(int i = 0; i < numTreeAttempts; i++)
    {
        Point2D p = getRandomPoint();
        double r = 8 + rand() % 2;
        if(isValidTreeLocation(p, r) && (rand() % 100 > 80))
        {
            Point location = {static_cast<double>(p.x), 0, static_cast<double>(p.z)};
            double trunkHeight = 25 + (rand() % 20) + 4*level;
            double leavesHeight = 15 + (rand() % 14) + 3*level;
            double leavesXZRadius = 3*r + 2 + (rand() % 20) + level;
            trees.emplace_back(location, trunkHeight, r, leavesHeight, leavesXZRadius);
        }
    }
}


Point2D ForestPlot::getRandomPoint() const
{
    return {center.x + (rand() % sideLength) - sideLength/2, center.z + (rand() % sideLength) - sideLength/2};
}
bool ForestPlot::isValidTreeLocation(Point2D potentialLocation, double radius) const
{
    // Check edges
    int x = potentialLocation.x;
    int z = potentialLocation.z;
    if(x - (center.x - sideLength/2) < radius || (center.x + sideLength/2) - x < radius ||
       z - (center.z - sideLength/2) < radius || (center.z + sideLength/2) - z < radius)
    {
        return false;
    }

    // Check other trees
    for(const Tree &t : trees)
    {
        Point p = {static_cast<double>(potentialLocation.x), 0, static_cast<double>(potentialLocation.z)};
        if(t.conflictsWithTree(p, radius))
        {
            return false;
        }
    }
    return true;
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
    for(Tree &t : trees)
    {
        t.draw();
    }
}