#ifndef COMPLETE_RANDOM_CITY_FRUSTUM_H
#define COMPLETE_RANDOM_CITY_FRUSTUM_H

#include "solid.h"
#include "recPrism.h"

class Frustum : public Solid
{
private:
    double upperXWidth;
    double upperZWidth;
public:
    Frustum();
    Frustum(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             double inputUpperXWidth, double inputUpperZWidth);

    void initializeCorners();

    void move(double deltaX, double deltaY, double deltaZ);

    void rotate(double thetaX, double thetaY, double thetaZ);

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

#endif //COMPLETE_RANDOM_CITY_FRUSTUM_H
