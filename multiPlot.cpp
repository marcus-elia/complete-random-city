#include "multiPlot.h"

MultiPlot::MultiPlot()
{
    plotType = Multi;
}
MultiPlot::MultiPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength)
: Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    plotType = Multi;
}

void MultiPlot::draw()
{
    glColor4f(0, 0.7, 0.1, 1);
    glBegin(GL_QUADS);
    drawPoint({center.x + sideLength/2.0, 0, center.z + sideLength/2.0});
    drawPoint({center.x + sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z + sideLength/2.0});
    glEnd();
}