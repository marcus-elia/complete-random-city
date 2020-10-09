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
    initializeStreetLamp();
    leftRoad = std::experimental::nullopt;
    rightRoad = std::experimental::nullopt;
    upRoad = std::experimental::nullopt;
    downRoad = std::experimental::nullopt;
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
void RoadPlot::initializeStreetLamp()
{
    if((rand() % 100) < 8)
    {
        Point location = {center.x + sideLength/4.0 + 2, 10, center.z + sideLength/4.0 + 2}; // location.y is halfway up the pole
        double poleRadius = 0.75;
        double poleHeight = 20;
        double lightRadius = 0.25;
        double lightHeight = 5;
        double baseHeight = 5;
        RGBAcolor poleColor = {0, 0.5, 0, 1};
        streetLamp = StreetLamp(location, poleRadius, poleHeight, lightRadius, lightHeight, poleColor, {1,1,1,1});
    }
    else
    {
        streetLamp = std::experimental::nullopt;
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
std::experimental::optional<RoadPlot*> RoadPlot::getLeftRoad() const
{
    return leftRoad;
}
std::experimental::optional<RoadPlot*> RoadPlot::getRightRoad() const
{
    return rightRoad;
}
std::experimental::optional<RoadPlot*> RoadPlot::getUpRoad() const
{
    return upRoad;
}
std::experimental::optional<RoadPlot*> RoadPlot::getDownRoad() const
{
    return downRoad;
}
int RoadPlot::getNumRoads() const
{
    int count = 0;
    if(getUpRoad())
    {
        count++;
    }
    if(getDownRoad())
    {
        count++;
    }
    if(getLeftRoad())
    {
        count++;
    }
    if(getRightRoad())
    {
        count++;
    }
    return count;
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
void RoadPlot::setLeftRoad(std::experimental::optional<RoadPlot*> inputLeftRoad)
{
    leftRoad = inputLeftRoad;
}
void RoadPlot::setRightRoad(std::experimental::optional<RoadPlot*> inputRightRoad)
{
    rightRoad = inputRightRoad;
}
void RoadPlot::setUpRoad(std::experimental::optional<RoadPlot*> inputUpRoad)
{
    upRoad = inputUpRoad;
}
void RoadPlot::setDownRoad(std::experimental::optional<RoadPlot*> inputDownRoad)
{
    downRoad = inputDownRoad;
}




DrivingDirection RoadPlot::getRandomDirection() const
{
    std::vector<DrivingDirection> choices;
    if(getLeftRoad())
    {
        choices.push_back(West);
    }
    if(getRightRoad())
    {
        choices.push_back(East);
    }
    if(getUpRoad())
    {
        choices.push_back(North);
    }
    if(getDownRoad())
    {
        choices.push_back(South);
    }
    if(choices.empty())
    {
        return North;
    }
    // randomly pick a direction
    int r = rand() % choices.size();
    return choices[r];
}


DrivingDirection RoadPlot::getRandomDirectionExcept(DrivingDirection input) const
{
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


double RoadPlot::getLaneWidth() const
{
    return sideLength/4.0;
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
Point2D RoadPlot::getUpExitPoint() const
{
    return {center.x + sideLength/8, center.z - sideLength/4};
}
Point2D RoadPlot::getUpEntrancePoint() const
{
    return {center.x - sideLength/8, center.z - sideLength/4};
}
Point2D RoadPlot::getRightExitPoint() const
{
    return {center.x + sideLength/4, center.z + sideLength/8};
}
Point2D RoadPlot::getRightEntrancePoint() const
{
    return {center.x + sideLength/4, center.z - sideLength/8};
}
Point2D RoadPlot::getDownExitPoint() const
{
    return {center.x - sideLength/8, center.z + sideLength/4};
}
Point2D RoadPlot::getDownEntrancePoint() const
{
    return {center.x + sideLength/8, center.z + sideLength/4};
}
Point2D RoadPlot::getLeftExitPoint() const
{
    return {center.x - sideLength/4, center.z - sideLength/8};
}
Point2D RoadPlot::getLeftEntrancePoint() const
{
    return {center.x - sideLength/4, center.z + sideLength/8};
}
// Points for cars turning
std::vector<Point2D> RoadPlot::getTurnPointsLeftNorth(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getWestIntersectionEdge()), static_cast<int>(getSouthIntersectionEdge())};
    // r = the radius of the curve
    double r = 3.0*sideLength/8;

    // If car is too fast, pretend it is going slower so we can calculate points
    // d = Euclidean distance between the points on the curve
    double d = carSpeed < 2*r ? carSpeed : 2*r;

    // n = number of points inside the curve (not counting endpoints) to make the car move at the
    // correct speed. (this formula came from law of cosines)
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);

    double deltaTheta = (PI/2) / (n+1);  // the angle swept out between each two points

    std::vector<Point2D> turnPoints;
    double theta = 3*PI/2; // Start at the end of the intersection and go clockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsRightNorth(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getEastIntersectionEdge()), static_cast<int>(getSouthIntersectionEdge())};
    double r = sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = 3*PI/2; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta -= deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsCircleNorth(double carSpeed) const
{
    double r = sideLength*0.2795;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(5.3556 / (acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (5.3556) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI/2 + 0.46365; // Start at the end of the intersection and go clockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(center.x + deltaX), static_cast<int>(center.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsLeftEast(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getWestIntersectionEdge()), static_cast<int>(getNorthIntersectionEdge())};
    double r = 3*sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = 0; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsRightEast(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getWestIntersectionEdge()), static_cast<int>(getSouthIntersectionEdge())};
    double r = sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = 0; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta -= deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsCircleEast(double carSpeed) const
{
    double r = sideLength*0.2795;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(5.3556 / (acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (5.3556) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI + 0.46365; // Start at the end of the intersection and go clockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(center.x + deltaX), static_cast<int>(center.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsLeftSouth(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getEastIntersectionEdge()), static_cast<int>(getNorthIntersectionEdge())};
    double r = 3*sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI/2; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsRightSouth(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getWestIntersectionEdge()), static_cast<int>(getNorthIntersectionEdge())};
    double r = sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI/2; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta -= deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsCircleSouth(double carSpeed) const
{
    double r = sideLength*0.2795;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(5.3556 / (acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (5.3556) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = 3*PI/2 + 0.46365; // Start at the end of the intersection and go clockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(center.x + deltaX), static_cast<int>(center.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsLeftWest(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getEastIntersectionEdge()), static_cast<int>(getSouthIntersectionEdge())};
    double r = 3*sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsRightWest(double carSpeed) const
{
    Point2D turnCenter = {static_cast<int>(getEastIntersectionEdge()), static_cast<int>(getNorthIntersectionEdge())};
    double r = sideLength/8.0;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(PI / (2*acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (PI/2) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = PI; // Start at the end of the intersection and go counterclockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta -= deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(turnCenter.x + deltaX), static_cast<int>(turnCenter.z + deltaZ)});
    }
    return turnPoints;
}
std::vector<Point2D> RoadPlot::getTurnPointsCircleWest(double carSpeed) const
{
    double r = sideLength*0.2795;
    double d = carSpeed < 2*r ? carSpeed : 2*r;
    int n = ceil(5.3556 / (acos(1 - (d*d)/(r*r))) - 1);
    double deltaTheta = (5.3556) / (n+1);
    std::vector<Point2D> turnPoints;
    double theta = 0.46365; // Start at the end of the intersection and go clockwise
    double deltaX, deltaZ;
    for(int i = 1; i < n+1; i++)
    {
        theta += deltaTheta;
        deltaX = r*cos(theta);
        deltaZ = r*sin(theta);
        turnPoints.push_back({static_cast<int>(center.x + deltaX), static_cast<int>(center.z + deltaZ)});
    }
    return turnPoints;
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

    if(streetLamp)
    {
        streetLamp->draw();
    }


    // Draw the center lines
    glLineWidth(3.0);
    glColor4f(1.0, 1.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for(Segment sgmt : yellowLines)
    {
        drawPoint(sgmt.p1);
        drawPoint(sgmt.p2);
    }
    glEnd();
    glLineWidth(2.0);



    glEnable(GL_CULL_FACE);
}