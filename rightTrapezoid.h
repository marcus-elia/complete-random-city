#ifndef COMPLETE_RANDOM_CITY_RIGHTTRAPEZOID_H
#define COMPLETE_RANDOM_CITY_RIGHTTRAPEZOID_H

#include "solid.h"

class RightTrapezoid : public Solid
{
private:
    double upperZWidth;
public:
    RightTrapezoid();
    RightTrapezoid(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
    double inputUpperZWidth);
    RightTrapezoid(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
    double inputUpperZWidth,
            Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
    Point inputOwnerCenter);

    void initializeCorners();

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};
#endif //COMPLETE_RANDOM_CITY_RIGHTTRAPEZOID_H
