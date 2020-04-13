#include "recPrism.h"

RecPrism::RecPrism() : Solid()
{
    initializeCorners();
}
RecPrism::RecPrism(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    initializeCorners();
}

RecPrism::RecPrism(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
        inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    initializeCorners();
}

void RecPrism::initializeCorners()
{
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}



void RecPrism::lookAt(Point &p)
{

}

void RecPrism::draw() const
{
    glDisable(GL_CULL_FACE);
    drawLines();
    drawFaces();
    glEnable(GL_CULL_FACE);

    // Debugging: print a ring around the middle of each rectangle to
    // prove that it knows where it is.
    /*glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0,1,0,1);
    drawPoint(center);
    double x, z;
    for(int i = 0; i < 7; i++)
    {
        x = center.x + xWidth*cos(i*2*PI/6);
        z = center.z + zWidth*sin(i*2*PI/6);
        drawPoint({x, center.y, z});
    }
    glEnd();
    glEnable(GL_CULL_FACE);*/
}

void RecPrism::drawLines() const
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

void RecPrism::drawFaces() const
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

// Wrapper function
std::experimental::optional<Point> RecPrism::correctCollision(Point p, int buffer)
{
    return correctRectangularPrismCollision(p, buffer, center,
                                            xWidth, yWidth, zWidth);
}

void RecPrism::printDebugStats()
{
    std::cout<<"Rectangular Prism at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
}

// Static
std::experimental::optional<Point> correctRectangularPrismCollision(Point p, int buffer, Point c,
                                                                    double xw, double yw, double zw)
{
    // If it is ouside the prism, just return nullopt
    if(p.x < c.x - xw/2 - buffer || p.x > c.x + xw/2 + buffer ||
       p.z < c.z - zw/2 - buffer || p.z > c.z + zw/2 + buffer ||
       p.y > c.y + yw/2 + buffer || p.y < c.y - yw/2 - buffer)
    {
        return std::experimental::nullopt;
    }
    // Get the distance from each face
    double left = abs(p.x - c.x + xw/2);
    double right = abs(p.x - c.x - xw/2);
    double up = abs(p.y - c.y - yw/2);
    double down = abs(p.y - c.y + yw/2);
    double front = abs(p.z - c.z - zw/2);
    double back = abs(p.z - c.z + zw/2);
    // If we're closest to the left side
    if(left <= right && left < up && left < down && left < front && left < back)
    {
        return std::experimental::optional<Point>({c.x - xw/2 - buffer, p.y, p.z});
    }
        // Right side
    else if(right <= up && right < down && right < front && right < back)
    {
        return std::experimental::optional<Point>({c.x + xw/2 + buffer, p.y, p.z});
    }
        // Up
    else if(up <= down && up < front && up < back)
    {
        return std::experimental::optional<Point>({p.x, c.y + yw/2 + buffer, p.z});
    }
        // Down
    else if(down <= front && down < back)
    {
        return std::experimental::optional<Point>({p.x, c.y - yw/2 - buffer, p.z});
    }
        // Back side
    else if(back < front)
    {
        return std::experimental::optional<Point>({p.x, p.y, c.z - zw/2 - buffer});
    }
    else // Front
    {
        return std::experimental::optional<Point>({p.x, p.y, c.z + zw/2 + buffer});
    }
}