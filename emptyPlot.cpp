#include "emptyPlot.h"

EmptyPlot::EmptyPlot()
{
    plotType = Empty;
}
EmptyPlot::EmptyPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    plotType = Empty;
}

void EmptyPlot::draw()
{
    glColor4f(0, 0.7, 0.1, 1);
    glBegin(GL_QUADS);
    drawPoint({center.x + sideLength/2.0, 0, center.z + sideLength/2.0});
    drawPoint({center.x + sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z + sideLength/2.0});
    glEnd();
}