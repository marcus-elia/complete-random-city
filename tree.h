#ifndef COMPLETE_RANDOM_CITY_TREE_H
#define COMPLETE_RANDOM_CITY_TREE_H

#include "cylinder.h"
#include "ellipsoid.h"

class Tree
{
private:
    Point location;
    Ellipsoid leaves;
    Cylinder trunk;
    int trunkHeight;
    double trunkRadius;
    double leavesHeight;
    double leavesXZRadius;
    RGBAcolor trunkColor;
    RGBAcolor leavesColor;
public:
    Tree();
    Tree(Point inputLocation, int inputTrunkHeight, double inputTrunkRadius, double inputLeavesHeight, double inputLeavesXZRadius);

    void initializeSolids();

    // Returns true if this tree's trunk intersects the other trunk
    bool conflictsWithTree(Point otherLocation, double otherRadius) const;

    Point getLocation() const;
    double getTrunkRadius() const;

    void draw() const;
};

#endif //COMPLETE_RANDOM_CITY_TREE_H
