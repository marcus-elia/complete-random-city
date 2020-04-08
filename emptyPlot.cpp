#include "emptyPlot.h"

EmptyPlot::EmptyPlot()
{

}
EmptyPlot::EmptyPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength) :
Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{

}

void EmptyPlot::draw()
{
    glColor4f(0, 0.7, 0.1, 0);
    glBegin(GL_QUADS);
    drawPoint({center.x + sideLength/2.0, 0, center.z + sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z + sideLength/2.0});
    drawPoint({center.x - sideLength/2.0, 0, center.z - sideLength/2.0});
    drawPoint({center.x + sideLength/2.0, 0, center.z - sideLength/2.0});
    glEnd();
}