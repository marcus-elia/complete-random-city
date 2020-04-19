#include "rightTrapezoid.h"

RightTrapezoid::RightTrapezoid() : Solid()
{
    upperZWidth = 15;
    initializeCorners();
}
RightTrapezoid::RightTrapezoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
 double inputUpperZWidth) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    upperZWidth = inputUpperZWidth;
    initializeCorners();
}
RightTrapezoid::RightTrapezoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
 double inputUpperZWidth,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    upperZWidth = inputUpperZWidth;
    initializeCorners();
}

void RightTrapezoid::initializeCorners()
{
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z + xWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z + zWidth/2 - upperZWidth});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z + zWidth/2 - upperZWidth});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}

void RightTrapezoid::lookAt(Point &p)
{

}

void RightTrapezoid::draw() const
{
    drawLines();
    drawFaces();
}

void RightTrapezoid::drawLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[1]);
    drawPoint(corners[3]);

    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[2]);
    drawPoint(corners[0]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);

    drawPoint(corners[5]);
    drawPoint(corners[7]);

    drawPoint(corners[7]);
    drawPoint(corners[6]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);

    drawPoint(corners[0]);
    drawPoint(corners[4]);

    drawPoint(corners[2]);
    drawPoint(corners[6]);

    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[1]);
    drawPoint(corners[5]);

    glEnd();
}

void RightTrapezoid::drawFaces() const
{
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);

    drawPoint(corners[0]);
    drawPoint(corners[1]);
    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[5]);
    drawPoint(corners[4]);
    drawPoint(corners[6]);
    drawPoint(corners[7]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);
    drawPoint(corners[0]);
    drawPoint(corners[2]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[6]);
    drawPoint(corners[2]);
    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[3]);
    drawPoint(corners[1]);
    drawPoint(corners[5]);
    drawPoint(corners[7]);
    glEnd();
}


std::experimental::optional<Point> RightTrapezoid::correctCollision(Point p, int buffer)
{
    return std::experimental::nullopt;
}

void RightTrapezoid::printDebugStats()
{
    
}