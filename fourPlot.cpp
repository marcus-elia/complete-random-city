#include "fourPlot.h"

FourPlot::FourPlot()
{
    plotType = Four;
}
FourPlot::FourPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength,
        FourPlotLocation inputFourPlotLocation) : Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    fourPlotLocation = inputFourPlotLocation;
    plotType = Four;
}

void FourPlot::draw()
{
    glColor4f(0, 0.7, 0.1, 1);
    glBegin(GL_QUADS);
    drawPoint({center.x + sideLength/2.0, 0, center.z + sideLength/2.0});
    drawPoint({center.x + sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z + sideLength/2.0});
    glEnd();
}