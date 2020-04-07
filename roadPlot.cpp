#include "roadPlot.h"

RoadPlot::RoadPlot() : Plot()
{

}
RoadPlot::RoadPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength,
                   bool inputLeft, bool inputRight, bool inputUp, bool inputDown) :
                   Plot(inputTopLeftChunkCoords, inputCenter, inputSideLength)
{
    left = inputLeft;
    right = inputRight;
    up = inputUp;
    down = inputDown;
    initializeRoadCorners();
    initializeYellowLines();
}

void RoadPlot::initializeRoadCorners()
{
    // Center intersection
    roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + sideLength/4.0});
    roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + sideLength/4.0});
    roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - sideLength/4.0});
    roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - sideLength/4.0});

    if(left)
    {
        roadCorners.push_back({center.x - 3.0*sideLength/8 + sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 - sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 - sideLength/8.0, 0, center.z - sideLength/4.0});
        roadCorners.push_back({center.x - 3.0*sideLength/8 + sideLength/8.0, 0, center.z - sideLength/4.0});
    }
    if(right)
    {
        roadCorners.push_back({center.x + 3.0*sideLength/8 + sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 - sideLength/8.0, 0, center.z + sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 - sideLength/8.0, 0, center.z - sideLength/4.0});
        roadCorners.push_back({center.x + 3.0*sideLength/8 + sideLength/8.0, 0, center.z - sideLength/4.0});
    }
    if(up)
    {
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z - 3.0*sideLength/8 - sideLength/8.0});
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z - 3.0*sideLength/8 - sideLength/8.0});
    }
    if(down)
    {
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + 3.0*sideLength/8 + sideLength/8.0});
        roadCorners.push_back({center.x - sideLength/4.0, 0, center.z + 3.0*sideLength/8 - sideLength/8.0});
        roadCorners.push_back({center.x + sideLength/4.0, 0, center.z + 3.0*sideLength/8 - sideLength/8.0});
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
    if(!up && !down)
    {
        yellowLines.push_back({{center.x - sideLength/4.0, 0, (double)center.z},
                               {center.x + sideLength/4.0, 0, (double)center.z}});
    }
    if(!left && !right)
    {
        yellowLines.push_back({{(double)center.x, 0, center.z - sideLength/4.0},
                               {(double)center.x, 0, center.z + sideLength/4.0}});
    }
}

// Getters
bool RoadPlot::getLeft() const;
bool RoadPlot::getRight() const;
bool RoadPlot::getUp() const;
bool RoadPlot::getDown() const;
std::experimental::optional<std::shared_ptr<RoadPlot>> RoadPlot::getLeftRoad() const;
std::experimental::optional<std::shared_ptr<RoadPlot>> RoadPlot::getRightRoad() const;
std::experimental::optional<std::shared_ptr<RoadPlot>> RoadPlot::getUpRoad() const;
std::experimental::optional<std::shared_ptr<RoadPlot>> RoadPlot::getDownRoad() const;

// Setters
void RoadPlot::setLeft(bool inputLeft);
void RoadPlot::setRight(bool inputRight);
void RoadPlot::setUp(bool inputUp);
void RoadPlot::setDown(bool inputDown);
void RoadPlot::setLeftRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputLeftRoad);
void RoadPlot::setRightRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputRightRoad);
void RoadPlot::setUpRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputUpRoad);
void RoadPlot::setDownRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputDownRoad);

void draw();