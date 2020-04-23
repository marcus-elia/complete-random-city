#ifndef COMPLETE_RANDOM_CITY_MULTIPLOT_H
#define COMPLETE_RANDOM_CITY_MULTIPLOT_H

#include "plot.h"
#include "building.h"

// A MultiPlot is an empty plot when another plot is using this one.

class MultiPlot : public Plot
{
public:
    MultiPlot();
    MultiPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength);

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_MULTIPLOT_H
