#include "airport.h"

Airport::Airport()
{
    topLeft = {0,0};
    plotSize = 32;
}
Airport::Airport(Point inputTopLeft, double inputPlotSize)
{
    topLeft = inputTopLeft;
    plotSize = inputPlotSize;
    initializeRunwayPoints();
    initializeControlTower();
}


void Airport::initializeRunwayPoints()
{
    runwayStart = {topLeft.x + plotSize/2, 0, topLeft.z + plotSize/2};
    runwayEnd = {topLeft.x + plotSize*4 + plotSize/2, 0, topLeft.z + plotSize*4 + plotSize/2};
}
void Airport::initializeControlTower()
{
    Point2D towerTopLeft = {static_cast<int>(topLeft.x + 2*plotSize), static_cast<int>(topLeft.z + 4*plotSize)};
    controlTower = std::make_shared<Building>(Building(towerTopLeft, plotSize, 170, {1,1,1,1}, {1,1,1,1}, ControlTower));
}


void Airport::createAirplane()
{
    airplanes.push_back(std::make_shared<Airplane>(Airplane(runwayStart, runwayEnd, 1, {0,0,0}, 300, runwayStart, runwayEnd, 30, 15, 20)));
}



// Getters
Point Airport::getRunwayStart() const
{
    return runwayStart;
}
Point Airport::getRunwayEnd() const
{
    return runwayEnd;
}

void Airport::draw() const
{
    controlTower->draw();

    // Draw the runways
    glColor4f(0.3, 0.3, 0.3, 1);
    glBegin(GL_QUADS);
    // Main runway from northwest to southeast
    glVertex3f(topLeft.x + plotSize, 1, topLeft.z);
    glVertex3f(topLeft.x, 1, topLeft.z + plotSize);
    glVertex3f(topLeft.x + 5*plotSize, 1, topLeft.z + 6*plotSize);
    glVertex3f(topLeft.x + 6*plotSize, 1, topLeft.z + 5*plotSize);
    // Small vertical runway
    glVertex3f(topLeft.x + 4*plotSize - 5, 1, topLeft.z + plotSize + 5);
    glVertex3f(topLeft.x + 3*plotSize + 5, 1, topLeft.z + plotSize + 5);
    glVertex3f(topLeft.x + 3*plotSize + 5, 1, topLeft.z + 6*plotSize - 5);
    glVertex3f(topLeft.x + 4*plotSize - 5, 1, topLeft.z + 6*plotSize - 5);
    glEnd();
}
void Airport::tick()
{

}