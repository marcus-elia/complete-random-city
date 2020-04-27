#include "ellipsoid.h"

Ellipsoid::Ellipsoid() : Solid()
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
    hideLines = false;
}
Ellipsoid::Ellipsoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
    hideLines = false;
}
Ellipsoid::Ellipsoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
bool inputHideLines, int inputNumRings, int inputPointsPerRing) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    pointsPerRing = inputPointsPerRing;
    numRings = inputNumRings;
    initializeCorners();
    hideLines = inputHideLines;
}
Ellipsoid::Ellipsoid(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
    hideLines = false;
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
    double phi = -PI/2;
    double deltaPhi = (PI) / (numRings + 1);
    double rx, rz;  // the x and z radii at the given height
    for(int ring = 0; ring < numRings; ring++)
    {
        //curY -= deltaY;
        phi += deltaPhi;
        if(phi < 0)
        {
            curY = center.y + 1 / sqrt(4 / (tan(phi)*tan(phi)*xWidth*xWidth) + 4 / (yWidth*yWidth));
        }
        else if(phi > 0)
        {
            curY = center.y - 1 / sqrt(4 / (tan(phi)*tan(phi)*xWidth*xWidth) + 4 / (yWidth*yWidth));
        }
        else
        {
            curY = center.y;
        }
        rx = xWidth/2 * sqrt(1 - 4*(curY-center.y)*(curY-center.y)/yWidth/yWidth);
        rz = zWidth/2 * sqrt(1 - 4*(curY-center.y)*(curY-center.y)/yWidth/yWidth);
        double theta;
        for(int i = 0; i < pointsPerRing; i++)
        {
            theta = (double)i / pointsPerRing * 2 * PI;
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
    if(!hideLines)
    {
        drawLines();
    }
    drawFaces();
    glEnable(GL_CULL_FACE);
}

void Ellipsoid::drawLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);

    // The top
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[0]);
        drawPoint(corners[i+2]);
    }
    // The Bottom
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[1]);
        drawPoint(corners[(numRings - 1)*pointsPerRing + 2 + i]);
    }
    // First ring, horiztonal lines
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[i + 2]);
        drawPoint(corners[2 + (i + 1) % pointsPerRing]);
    }
    //drawPoint(corners[pointsPerRing + 3]);
    //drawPoint(corners[2]);

    // Iterate through the other rings
    for(int ring = 1; ring < numRings; ring++)
    {
        for(int i = 0; i < pointsPerRing; i++)
        {
            // Vertical segment
            drawPoint(corners[(ring-1)*pointsPerRing + 2 + i]); // the higher (previous) ring
            drawPoint(corners[ring*pointsPerRing + 2 + i]);     // the lower (current) ring

            // Horizontal segment
            drawPoint(corners[ring*pointsPerRing + 2 + i]);
            drawPoint(corners[ring*pointsPerRing + 2 + (i + 1) % pointsPerRing]);
        }
        // Connect to the start points
        //drawPoint(corners[ring*pointsPerRing + 2]);
        //drawPoint(corners[(ring + 1)*pointsPerRing + 1]);
    }
    glEnd();
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