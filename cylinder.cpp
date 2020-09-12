#include "cylinder.h"

Cylinder::Cylinder() : Solid()
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = 25;
    initializeCorners();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor, int inputSmoothness) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = inputSmoothness;
    initializeCorners();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                double inputTopXWidth, double inputTopZWidth) :
                Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    smoothness = 25;
    initializeCorners();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
        double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
        Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                         inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    smoothness = 25;
    initializeCorners();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
        double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
        double inputTopXWidth, double inputTopZWidth,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
        Point inputOwnerCenter) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
              inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    smoothness = 25;
    initializeCorners();
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
    }
    // Connect the end to the start
    drawPoint(corners[2*smoothness-2]);
    drawPoint(corners[0]);
    drawPoint(corners[2*smoothness-1]);
    drawPoint(corners[1]);

    glEnd();
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
