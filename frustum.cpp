#include "frustum.h"

Frustum::Frustum() : Solid()
{
    upperXWidth = 15;
    upperZWidth = 15;
    initializeCorners();
}
Frustum::Frustum(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
double inputUpperXWidth, double inputUpperZWidth,
                 linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    upperXWidth = inputUpperXWidth;
    upperZWidth = inputUpperZWidth;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
}
Frustum::Frustum(Point inputCenter, RGBAcolor inputColor,
        double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
        double inputUpperXWidth, double inputUpperZWidth,
        Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
        Point inputOwnerCenter,
        linesDrawnEnum inputLinesDrawn) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                         inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    upperXWidth = inputUpperXWidth;
    upperZWidth = inputUpperZWidth;
    linesDrawn = inputLinesDrawn;
    initializeCorners();
}

void Frustum::initializeCorners()
{
    corners.push_back({center.x + upperXWidth/2, center.y + yWidth/2, center.z + upperZWidth/2});
    corners.push_back({center.x - upperXWidth/2, center.y + yWidth/2, center.z + upperZWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + upperXWidth/2, center.y + yWidth/2, center.z - upperZWidth/2});
    corners.push_back({center.x - upperXWidth/2, center.y + yWidth/2, center.z - upperZWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}


void Frustum::lookAt(Point &p)
{

}

void Frustum::draw() const
{
    if(linesDrawn != NoLines)
    {
        drawLines();
    }
    drawFaces();
}

void Frustum::drawLines() const
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

void Frustum::drawFaces() const
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

// TODO need to actually fill this in. For now, it's pretending it's a rectangular prism
std::experimental::optional<Point> Frustum::correctCollision(Point p, int buffer)
{
    return correctRectangularPrismCollision(p, buffer, center, xWidth, yWidth, zWidth);
}

void Frustum::printDebugStats()
{
    std::cout<<"Frustum at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
    std::cout << "upperxWidth: " << upperXWidth << std::endl;
    std::cout << "upperyWidth: " << upperZWidth << std::endl;
}