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
    double topXWidth;
    double topZWidth;

    const static int distanceBetweenHighLines = 8;
    const static int distanceBetweenMediumLines = 32;
    const static int distanceBetweenLowLines = 56;

    std::vector<std::vector<Point>> linePoints;
public:
    Cylinder();
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             int inputSmoothness=25,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             double inputTopXWidth, double inputTopZWidth, int inputSmoothness=25,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
            double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
            Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            Point inputOwnerCenter, int inputSmoothness=25,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             double inputTopXWidth, double inputTopZWidth,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point inputOwnerCenter, int inputSmoothness=25,
             linesDrawnEnum inputLinesDrawn=Normal);

    void initializeCorners();

    // Make points for drawing gridlines, if applicable
    void initializeLinePoints();

    // Getters
    double getTopXWidth() const;
    double getTopZWidth() const;

    // Setters
    void setTopXWidth(double inputTopXWidth);
    void setTopZWidth(double inputTopZWidth);

    // Geometry
    // Returns the x or z radius at a height of y above the base
    double getXRadiusAtHeight(double y) const;
    double getZRadiusAtHeight(double y) const;
    // Get the coordinates on circumference at given height and angle
    Point getPointAtHeight(double y, double theta) const;


    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;
    void drawGridLines() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

#endif //COMPLETE_RANDOM_CITY_CYLINDER_H
