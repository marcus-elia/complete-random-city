#include "roadPlot.h"

RoadPlot::RoadPlot() : Plot()
{
    plotType = Road;
}
RoadPlot::RoadPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength,
                   bool inputLeft, bool inputRight, bool inputUp, bool inputDown) :
                   Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    plotType = Road;
    left = inputLeft;
    right = inputRight;
    up = inputUp;
    down = inputDown;
    initializeRoadCorners();
    initializeYellowLines();
}

void RoadPlot::initializeRoadCorners()
{
    int numDirections = 0; // for deciding if we should have a circle

    if(left)
    {
        numDirections++;
        roadCorners.push_back({center.x - 3.0*sideLength/8 + sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 - sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 - sideLength/8.0, 0, center.z - sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 + sideLength/8.0, 0, center.z - sideLength/4.0});
    }
    if(right)
    {
        numDirections++;
        roadCorners.push_back({center.x + 3.0*sideLength/8 + sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 - sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 - sideLength/8.0, 0, center.z - sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 + sideLength/8.0, 0, center.z - sideLength/4.0});
    }
    if(up)
    {
        numDirections++;
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - 3.0*sideLength/8 - sideLength/8.0});
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - 3.0*sideLength/8 - sideLength/8.0});
    }
    if(down)
    {
        numDirections++;
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + 3.0*sideLength/8 - sideLength/8.0});
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + 3.0*sideLength/8 - sideLength/8.0});
    }

    // Make circle if we need a cul de sac
    culDeSac = (numDirections == 1);
    if(culDeSac)
    {
        int smoothness = 20;
        double x, z;
        for(int i = 0; i < smoothness; i++)
        {
            x = sideLength*0.354 * cos(2*PI* i / smoothness);
            z = sideLength*0.354 * sin(2*PI* i / smoothness);
            circlePoints.push_back({center.x + x, 0, center.z + z});
        }
    }
    else // Otherwise, put a square in the center
    {
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - sideLength/4.0});
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - sideLength/4.0});
    }
}
void RoadPlot::initializeYellowLines()
{
    if(left)
    {
        yellowLines.push_back({{center.x - sideLength/2.0, 0, (double)center.z},
                               {center.x - sideLength/4.0, 0, (double)center.z}});
    }
    if(right)
    {
        yellowLines.push_back({{center.x + sideLength/2.0, 0, (double)center.z},
                               {center.x + sideLength/4.0, 0, (double)center.z}});
    }
    if(up)
    {
        yellowLines.push_back({{(double)center.x, 0, center.z - sideLength/2.0},
                               {(double)center.x, 0, center.z - sideLength/4.0}});
    }
    if(down)
    {
        yellowLines.push_back({{(double)center.x, 0, center.z + sideLength/2.0},
                               {(double)center.x, 0, center.z + sideLength/4.0}});
    }
    // For a road with no intersections, either draw horizontal or vertical line through the center
    if(!up && !down && !culDeSac)
    {
        yellowLines.push_back({{center.x - sideLength/4.0, 0, (double)center.z},
                               {center.x + sideLength/4.0, 0, (double)center.z}});
    }
    if(!left && !right && !culDeSac)
    {
        yellowLines.push_back({{(double)center.x, 0, center.z - sideLength/4.0},
                               {(double)center.x, 0, center.z + sideLength/4.0}});
    }
}

// Getters
bool RoadPlot::getLeft() const
{
    return left;
}
bool RoadPlot::getRight() const
{
    return right;
}
bool RoadPlot::getUp() const
{
    return up;
}
bool RoadPlot::getDown() const
{
    return down;
}
RoadPlot* RoadPlot::getLeftRoad() const
{
    return leftRoad;
}
RoadPlot* RoadPlot::getRightRoad() const
{
    return rightRoad;
}
RoadPlot* RoadPlot::getUpRoad() const
{
    return upRoad;
}
RoadPlot* RoadPlot::getDownRoad() const
{
    return downRoad;
}

// Setters
void RoadPlot::setLeft(bool inputLeft)
{
    left = inputLeft;
}
void RoadPlot::setRight(bool inputRight)
{
    right = inputRight;
}
void RoadPlot::setUp(bool inputUp)
{
    up = inputUp;
}
void RoadPlot::setDown(bool inputDown)
{
    down = inputDown;
}
void RoadPlot::setLeftRoad(RoadPlot* inputLeftRoad)
{
    leftRoad = inputLeftRoad;
}
void RoadPlot::setRightRoad(RoadPlot* inputRightRoad)
{
    rightRoad = inputRightRoad;
}
void RoadPlot::setUpRoad(RoadPlot* inputUpRoad)
{
    upRoad = inputUpRoad;
}
void RoadPlot::setDownRoad(RoadPlot* inputDownRoad)
{
    downRoad = inputDownRoad;
}

DrivingDirection RoadPlot::getRandomDirectionExcept(DrivingDirection input) const
{
    bool satisfied = false;
    std::vector<DrivingDirection> choices;
    if(getLeftRoad() && input != West)
    {
        choices.push_back(West);
    }
    if(getRightRoad() && input != East)
    {
        choices.push_back(East);
    }
    if(getUpRoad() && input != North)
    {
        choices.push_back(North);
    }
    if(getDownRoad() && input != South)
    {
        choices.push_back(South);
    }
    // If we can only go back the way we came, return input
    if(choices.empty())
    {
        return input;
    }
    // Otherwise, randomly pick a direction
    int r = rand() % choices.size();
    return choices[r];
}

double RoadPlot::getWestEdge() const
{
    return center.x - sideLength/2.0;
}
double RoadPlot::getNorthEdge() const
{
    return center.z - sideLength/2.0;
}
double RoadPlot::getEastEdge() const
{
    return center.x + sideLength/2.0;
}
double RoadPlot::getSouthEdge() const
{
    return center.z + sideLength/2.0;
}
double RoadPlot::getWestIntersectionEdge() const
{
    return center.x - sideLength/4.0;
}
double RoadPlot::getNorthIntersectionEdge() const
{
    return center.z - sideLength/4.0;
}
double RoadPlot::getEastIntersectionEdge() const
{
    return center.x + sideLength/4.0;
}
double RoadPlot::getSouthIntersectionEdge() const
{
    return center.z + sideLength/4.0;
}





void RoadPlot::draw()
{
    // Draw the rectangles that form the road
    glColor4f(0.5, 0.5, 0.5, 1);
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    for(int i = 0; i < roadCorners.size()/4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            drawPoint(roadCorners[4*i + j]);
        }
    }
    glEnd();

    if(culDeSac)
    {
        glBegin(GL_TRIANGLE_FAN);
        drawPoint({(double)center.x, 0, (double)center.z});
        for(Point &p : circlePoints)
        {
            drawPoint(p);
        }
        drawPoint(circlePoints[0]);
        glEnd();
    }


    // Draw the center lines
    glColor4f(1.0, 1.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for(Segment sgmt : yellowLines)
    {
        drawPoint(sgmt.p1);
        drawPoint(sgmt.p2);
    }
    glEnd();

    glEnable(GL_CULL_FACE);
}