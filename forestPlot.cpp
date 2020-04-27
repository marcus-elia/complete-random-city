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
    trees = std::vector<Tree>();
    int numTreeAttempts = 2*level*level + (rand() % 3);
    for(int i = 0; i < numTreeAttempts; i++)
    {
        Point2D p = getRandomPoint();
        double r = 3 + rand() % 2;
        if(isValidTreeLocation(p, r))
        {
            Point location = {static_cast<double>(p.x), 0, static_cast<double>(p.z)};
            double trunkHeight = 6 + (rand() % 10);
            double leavesHeight = 5 + (rand() % 9);
            double leavesXZRadius = r + 2 + (rand() % 6);
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