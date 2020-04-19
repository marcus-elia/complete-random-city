#include "ellipsoid.h"

Ellipsoid::Ellipsoid() : Solid()
{
    pointsPerRing = 16;
    numRings = 8;
    initializeCorners();
}
Ellipsoid::Ellipsoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    pointsPerRing = 16;
    numRings = 8;
    initializeCorners();
}
Ellipsoid::Ellipsoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    pointsPerRing = 16;
    numRings = 8;
    initializeCorners();
}

void Ellipsoid::initializeCorners()
{
    // corners[0] = top
    // corners[1] = bottom
    // corners[(i-1)*pointsPerRing + 2] = start of ith ring (starting at top)
    // corners has pointsPerRing*numRings + 2 entries
    corners.push_back({center.x, center.y + yWidth/2, center.z});
    corners.push_back({center.x, center.y - yWidth/2, center.z});

    double curY = center.y + yWidth/2;
    double deltaY = yWidth / (numRings + 1);
    double rx, rz;  // the x and z radii at the given height
    for(int ring = 0; ring < numRings; ring++)
    {
        curY -= deltaY;
        rx = (abs(curY - center.y) / yWidth) * xWidth;
        rz = (abs(curY - center.y) / yWidth) * zWidth;
        double theta;
        for(int i = 0; i < pointsPerRing; i++)
        {
            theta = i / pointsPerRing * 2 * PI;
            corners.push_back({center.x + rx*cos(theta), curY, center.z + rz*sin(theta)});
        }
    }
}

void Ellipsoid::lookAt(Point &p)
{

}

void Ellipsoid::draw() const
{
    glDisable(GL_CULL_FACE);
    drawLines();
    drawFaces();
    glEnable(GL_CULL_FACE);
}
void Ellipsoid::drawLines() const
{

}
void Ellipsoid::drawFaces() const
{
    glColor4f(color.r, color.g, color.b, color.a);

    // Top
    glBegin(GL_TRIANGLE_FAN);
    drawPoint(corners[0]);
    for(int i = 2; i < pointsPerRing + 2; i++)
    {
        drawPoint(corners[i]);
    }
    drawPoint(corners[2]);
    glEnd();

    // Bottom
    glBegin(GL_TRIANGLE_FAN);
    drawPoint(corners[1]);
    for(int i = 2; i < pointsPerRing + 2; i++)
    {
        drawPoint(corners[(numRings-1)*pointsPerRing + i]);
    }
    drawPoint(corners[(numRings-1)*pointsPerRing + 2]);
    glEnd();

    // Connect consecutive rings
    for(int ring = 1; ring < numRings; ring++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i < pointsPerRing; i++)
        {
            drawPoint(corners[(ring-1)*pointsPerRing + 2 + i]); // the higher (previous) ring
            drawPoint(corners[ring*pointsPerRing + 2 + i]);     // the lower (current) ring
        }
        // Connect to the start points
        drawPoint(corners[(ring-1)*pointsPerRing + 2]);
        drawPoint(corners[ring*pointsPerRing + 2]);
        glEnd();
    }
}

std::experimental::optional<Point> Ellipsoid::correctCollision(Point p, int buffer)
{
    return std::experimental::nullopt;
}

void Ellipsoid::printDebugStats()
{

}