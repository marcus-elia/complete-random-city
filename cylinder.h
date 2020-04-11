#ifndef COMPLETE_RANDOM_CITY_CYLINDER_H
#define COMPLETE_RANDOM_CITY_CYLINDER_H

#include "solid.h"
#include <iostream>

// Creates an elliptical cylinder whose bases are parallel to the xz plane
// NOTE: in order for collision to work, must be a circular ellipse

class Cylinder : public Solid
{
private:
    int smoothness;
public:
    Cylinder();
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
            double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
            Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            Point* inputOwnerCenter);

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

#endif //COMPLETE_RANDOM_CITY_CYLINDER_H
