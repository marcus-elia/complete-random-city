#include "plot.h"

Plot::Plot()
{

}
Plot::Plot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength)
{
    topLeftChunkCoords = inputTopLeftChunkCoords;
    center = inputCenter;
    sideLength = inputSideLength;
}

// Getters
Point2D Plot::getTopLeftChunkCoords() const
{
    return topLeftChunkCoords;
}
Point2D Plot::getCenter() const
{
    return center;
}
int Plot::getSideLength() const
{
    return sideLength;
}
PlotType Plot::getPlotType() const
{
    return plotType;
}

// Setters
void Plot::setTopLeftChunkCoords(Point2D inputTopLeftChunkCoords)
{
    topLeftChunkCoords = inputTopLeftChunkCoords;
}
void Plot::setCenter(Point2D inputCenter)
{
    center = inputCenter;
}
void Plot::setSideLength(int inputSideLength)
{
    sideLength = inputSideLength;
}
void Plot::draw()
{
    glDisable(GL_CULL_FACE);
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    glVertex3f(center.x + 5, 0, center.z + 5);
    glVertex3f(center.x - 5, 0, center.z + 5);
    glVertex3f(center.x- 5, 0, center.z - 5);
    glVertex3f(center.x + 5, 0, center.z - 5);
    glEnd();
    glEnable(GL_CULL_FACE);
}

