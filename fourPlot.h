#ifndef COMPLETE_RANDOM_CITY_FOURPLOT_H
#define COMPLETE_RANDOM_CITY_FOURPLOT_H

#include "plot.h"
#include "building.h"

// A FourPlot is one of 4 plots in a 2x2 square of plots that form
// a single structure. A FourPlot is one of the 3 which does not
// contain a building (meaning it is empty)

enum FourPlotLocation {UpLeft, UpRight, DownLeft, DownRight};

class FourPlot : public Plot
{
private:
    FourPlotLocation fourPlotLocation;
public:
    FourPlot();
    FourPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength, FourPlotLocation inputFourPlotLocation);

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_FOURPLOT_H
