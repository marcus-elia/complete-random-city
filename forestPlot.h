#ifndef COMPLETE_RANDOM_CITY_FORESTPLOT_H
#define COMPLETE_RANDOM_CITY_FORESTPLOT_H

#include "plot.h"
#include "tree.h"
#include <memory>

class ForestPlot : public Plot
{
private:
    int level; // how dense and tall it is
    std::vector<Tree> trees;
public:
    ForestPlot();
    ForestPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength);

    // Creates a bunch of random trees based on the value of level
    void initializeTrees();

    // Returns a random point inside the plot
    Point2D getRandomPoint() const;

    bool isValidTreeLocation(Point2D potentialLocation, double radius) const;

    int getLevel() const;
    void setLevel(int inputLevel);

    void draw();

};

#endif //COMPLETE_RANDOM_CITY_FORESTPLOT_H
