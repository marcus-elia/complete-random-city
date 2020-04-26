#ifndef COMPLETE_RANDOM_CITY_FORESTPLOT_H
#define COMPLETE_RANDOM_CITY_FORESTPLOT_H

#include "plot.h"
#include <memory>

class ForestPlot : public Plot
{
private:
    int level; // how dense and tall it is
public:
    ForestPlot();
    ForestPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength);

    void initializeTrees();

    int getLevel() const;
    void setLevel(int inputLevel);

    void draw();

};

#endif //COMPLETE_RANDOM_CITY_FORESTPLOT_H
