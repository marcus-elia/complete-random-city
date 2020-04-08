#ifndef COMPLETE_RANDOM_CITY_EMPTYPLOT_H
#define COMPLETE_RANDOM_CITY_EMPTYPLOT_H

#include "plot.h"

class EmptyPlot : public Plot
{
public:
    EmptyPlot();
    EmptyPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength);

    void draw();

};

#endif //COMPLETE_RANDOM_CITY_EMPTYPLOT_H
