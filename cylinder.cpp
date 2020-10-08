#include "cylinder.h"

Cylinder::Cylinder() : Solid()
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = 25;
    linesDrawn = Normal;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   int inputSmoothness, linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = inputSmoothness;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                double inputTopXWidth, double inputTopZWidth, int inputSmoothness,
                   linesDrawnEnum inputLinesDrawn) :
                Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    smoothness = inputSmoothness;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
        double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
        Point inputOwnerCenter, int inputSmoothness, linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                         inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = inputSmoothness;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
        double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
        double inputTopXWidth, double inputTopZWidth,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
        Point inputOwnerCenter, int inputSmoothness,
                   linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
              inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    smoothness = inputSmoothness;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
    initializeLinePoints();
}


void Cylinder::initializeCorners()
{
    double x, z, xTop, zTop;
    for(int i = 0; i < smoothness; i++)
    {
        x = xWidth/2 * cos(2*PI* i / smoothness);
        z = zWidth/2 * sin(2*PI* i / smoothness);
        xTop = topXWidth/2 * cos(2*PI* i / smoothness);
        zTop = topZWidth/2 * sin(2*PI* i / smoothness);
        corners.push_back({center.x + xTop, center.y + yWidth/2, center.z + zTop}); // upper face
        corners.push_back({center.x + x, center.y - yWidth/2, center.z + z}); // lower face
    }
}

void Cylinder::initializeLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(yWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(yWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(yWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    double distanceBetweenPoints = yWidth / numPoints;

    // Iterate through the bottom of the ellipse and add points above it
    for(int i = 1; i < corners.size(); i += 2)
    {
        y = 0;

        double theta = 2*PI* (i+1)/2 / smoothness;

        linePoints.emplace_back();
        for(int j = 0; j < numPoints - 1; j++)
        {
            y += distanceBetweenPoints;
            linePoints.back().push_back(getPointAtHeight(y, theta));
        }
    }
}

double Cylinder::getXRadiusAtHeight(double y) const
{
    double slope = (topXWidth - xWidth) / 2 / yWidth;
    return xWidth/2 + slope*y;
}

double Cylinder::getZRadiusAtHeight(double y) const
{
    double slope = (topZWidth - zWidth) / 2 / yWidth;
    return zWidth/2 + slope*y;
}

Point Cylinder::getPointAtHeight(double y, double theta) const
{
    double xRad = getXRadiusAtHeight(y);
    double zRad = getZRadiusAtHeight(y);
    return {center.x + xRad*cos(theta), center.y - yWidth/2 + y, center.z + zRad*sin(theta)};
}




void Cylinder::lookAt(Point &p)
{

}

void Cylinder::draw() const
{
    drawLines();
    drawFaces();
}

void Cylinder::drawLines() const
{
    if(linesDrawn == NoLines)
    {
        return;
    }

    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);
    for(int i = 0; i < 2*smoothness - 3; i += 2)
    {
        // Upper face
        drawPoint(corners[i]);
        drawPoint(corners[i + 2]);

        // Lower face
        drawPoint(corners[i + 1]);
        drawPoint(corners[i + 3]);

        // Verticals
        if(((linesDrawn == Low || linesDrawn == Normal) && i % 12 == 0) || (linesDrawn == Medium && i % 6 == 0) ||
           linesDrawn == High)
        {
            drawPoint(corners[i]);
            drawPoint(corners[i+1]);
        }
    }
    // Connect the end to the start
    drawPoint(corners[2*smoothness-2]);
    drawPoint(corners[0]);
    drawPoint(corners[2*smoothness-1]);
    drawPoint(corners[1]);

    // Vertical
    if(linesDrawn == High)
    {
        drawPoint(corners[2*smoothness-2]);
        drawPoint(corners[2*smoothness-1]);
    }

    glEnd();

    if(linesDrawn != Normal)
    {
        drawGridLines();
    }
}

void Cylinder::drawFaces() const
{
    glColor4f(color.r, color.g, color.b, color.a);
    glDisable(GL_CULL_FACE);

    // Draw the top and bottom circles
    glBegin(GL_TRIANGLE_FAN);
    // center
    drawPoint({center.x, center.y + yWidth/2, center.z});
    // top circumference
    for(int i = 0; i < smoothness; i++)
    {
        drawPoint(corners[2*i]);
    }
    drawPoint(corners[0]);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    // center
    drawPoint({center.x, center.y - yWidth/2, center.z});
    // bottom circumference
    for(int i = 0; i < smoothness; i++)
    {
        drawPoint(corners[2*i + 1]);
    }
    drawPoint(corners[1]);
    glEnd();

    // Draw the cylinder part

    glBegin(GL_TRIANGLE_STRIP);

    for(int i = 0; i < 2*smoothness; i++)
    {
        drawPoint(corners[i]);
    }
    // Connect the end to the start
    drawPoint(corners[0]);
    drawPoint(corners[1]);
    glEnd();

    glEnable(GL_CULL_FACE);
}

void Cylinder::drawGridLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);

    int pointsPerSide;

    pointsPerSide = linePoints[0].size();
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(linePoints[0][i]);
        for(int j = 1; j < smoothness; j++)
        {
            drawPoint(linePoints[j][i]);
            drawPoint(linePoints[j][i]);
        }
        drawPoint(linePoints[0][i]);
    }

    glEnd();
}





std::experimental::optional<Point> Cylinder::correctCollision(Point p, int buffer)
{
    if(distance2d(p, center) >= xWidth + buffer || p.y < center.y - yWidth/2 - buffer ||
    p.y > center.y + yWidth/2 + buffer)
    {
        return std::experimental::nullopt;
    }
    // If it's near the top
    if(abs(p.y - center.y - yWidth/2) < buffer)
    {
        return std::experimental::optional<Point>({p.x, center.y + yWidth/2 + buffer, p.z});
    }
    // Bottom
    else if(abs(p.y - center.y + yWidth/2) < buffer)
    {
        return std::experimental::optional<Point>({p.x, center.y - yWidth/2 - buffer, p.z});
    }
    else // Move out of the circle
    {
        double theta = atan2(p.z - center.z, p.x - center.x);
        return std::experimental::optional<Point>({center.x + (xWidth/2 + buffer)*cos(theta), p.y,
                                                   center.z + (zWidth/2 + buffer)*sin(theta)});
    }
}

void Cylinder::printDebugStats()
{
    std::cout<<"Cylinder at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
}
