#include "tree.h"

Tree::Tree()
{
    location = {0,0,0};
    trunkHeight = 30;
    trunkRadius = 6;
    leavesHeight = 25;
    leavesXZRadius = 15;
    trunkColor = {0.2, 0.1, 0.05, 1};
    leavesColor = {0, 0.7, 0, 1};
    initializeSolids();
}
Tree::Tree(Point inputLocation, int inputTrunkHeight, double inputTrunkRadius, double inputLeavesHeight, double inputLeavesXZRadius)
{
    location = inputLocation;
    trunkHeight = inputTrunkHeight;
    trunkRadius = inputTrunkRadius;
    leavesHeight = inputLeavesHeight;
    leavesXZRadius = inputLeavesXZRadius;
    double r = (rand() % 100 ) / 100.0;
    leavesColor = {r/4, 0.5 + r/2, r/4, 1};
    trunkColor = {0.1 + r/8, 0.05 + r/12, r/12, 1};
    initializeSolids();
}

void Tree::initializeSolids()
{
    Point center = {location.x, trunkHeight/2.0, location.z};
    trunk = Cylinder(location, trunkColor, trunkRadius, trunkHeight, trunkRadius, {1,1,1,1});
    center = {location.x, trunkHeight + leavesHeight/2.0, location.z};
    leaves = Ellipsoid(location, leavesColor, leavesXZRadius, leavesHeight, leavesXZRadius, {1,1,1,1});
}

bool Tree::conflictsWithTree(Point otherLocation, double otherRadius) const
{
    return distance2d(location, otherLocation) < trunkRadius + otherRadius;
}



Point Tree::getLocation() const
{
    return location;
}
double Tree::getTrunkRadius() const
{
    return trunkRadius;
}

void Tree::draw() const
{
    trunk.draw();
    leaves.draw();
}