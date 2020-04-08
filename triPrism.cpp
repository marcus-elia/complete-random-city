#include "triPrism.h"

TriPrism::TriPrism() : Solid()
{
    initializeCorners();
}
TriPrism::TriPrism(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    initializeCorners();
}

void TriPrism::initializeCorners()
{
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x, center.y + yWidth/2, center.z - zWidth/2});
}


void TriPrism::move(double deltaX, double deltaY, double deltaZ)
{

}

void TriPrism::rotate(double thetaX, double thetaY, double thetaZ)
{

}

void TriPrism::lookAt(Point &p)
{

}

void TriPrism::draw() const
{
    drawLines();
    drawFaces();
}

void TriPrism::drawLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[1]);
    drawPoint(corners[2]);

    drawPoint(corners[2]);
    drawPoint(corners[0]);

    drawPoint(corners[1]);
    drawPoint(corners[4]);

    drawPoint(corners[2]);
    drawPoint(corners[5]);

    drawPoint(corners[0]);
    drawPoint(corners[3]);

    drawPoint(corners[3]);
    drawPoint(corners[4]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);

    drawPoint(corners[5]);
    drawPoint(corners[3]);

    glEnd();
}

void TriPrism::drawFaces() const
{

    glColor4f(color.r, color.g, color.b, color.a);

    glBegin(GL_TRIANGLES);

    drawPoint(corners[0]);
    drawPoint(corners[2]);
    drawPoint(corners[1]);

    drawPoint(corners[3]);
    drawPoint(corners[4]);
    drawPoint(corners[5]);

    glEnd();

    glBegin(GL_QUADS);

    drawPoint(corners[5]);
    drawPoint(corners[4]);
    drawPoint(corners[1]);
    drawPoint(corners[2]);

    drawPoint(corners[5]);
    drawPoint(corners[2]);
    drawPoint(corners[0]);
    drawPoint(corners[3]);

    drawPoint(corners[1]);
    drawPoint(corners[4]);
    drawPoint(corners[3]);
    drawPoint(corners[0]);

    glEnd();
}

std::experimental::optional<Point> TriPrism::correctCollision(Point p, int buffer)
{
    // If not on the property
    if(p.x < center.x - xWidth/2 - buffer || p.x > center.x + xWidth/2 + buffer ||
          p.z < center.z - zWidth/2 - buffer || p.z > center.z + zWidth/2 + buffer ||
          p.y > center.y + yWidth/2 + buffer || p.y < center.y - yWidth/2 - buffer)
    {
        return std::experimental::nullopt;
    }
    if(p.x <= center.x) // Left half
    {
        double A = 2*yWidth/xWidth;
        double B = -1;
        double C = center.y - yWidth/2 - 2*center.x*yWidth/xWidth;
        if(abs(directedDistance(A,B,C,p.x,p.y)) < buffer)
        {
            double m = 2*yWidth/xWidth; // slope of the left side of the triangle in xy plane
            double b1 = center.y - yWidth/2 - 2*center.x*yWidth/xWidth;  // y-intercept
            double b2 = p.y + xWidth*p.x / (2*yWidth);   // y-intercept of perpendicular line through p

            // Where the perpendicular through p intersects the triangle's side
            double intersectionX = (b2 - b1) / (m + 1/m);
            double intersectionY = m*intersectionX + b1;

            double theta = atan(-1/m); // the angle of the perpendicular

            // How far to move from the intersection point
            double deltaX = buffer*cos(theta);
            double deltaY = buffer*sin(theta);
            return std::experimental::optional<Point>({intersectionX + deltaX, intersectionY + deltaY, p.z});
        }
    }
    else // Right half
    {
        double A = -2*yWidth/xWidth;
        double B = -1;
        double C = center.y - yWidth/2 + 2*center.x*yWidth/xWidth;
        if(abs(directedDistance(A,B,C,p.x,p.y)) < buffer)
        {
            double m = -2*yWidth/xWidth; // slope of the right side of the triangle in xy plane
            double b1 = center.y - yWidth/2 + 2*center.x*yWidth/xWidth;  // y-intercept
            double b2 = p.y - xWidth/(2*yWidth)*p.x;  // y-intercept of perpendicular line through p

            // Where the perpendicular through p intersects the triangle's side
            double intersectionX = (b2 - b1) / (m + 1/m);
            double intersectionY = m*intersectionX + b1;

            double theta = atan(-1/m); // the angle of the perpendicular

            // How far to move from the intersection point
            double deltaX = buffer*cos(theta);
            double deltaY = buffer*sin(theta);
            return std::experimental::optional<Point>({intersectionX + deltaX, intersectionY + deltaY, p.z});
        }
    }
    // Otherwise, assume it's near the base of the prism
    return std::experimental::optional<Point>({p.x, center.y - yWidth/2 - buffer, p.z});
}

void TriPrism::printDebugStats()
{
    std::cout<<"Triangular Prism at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
}