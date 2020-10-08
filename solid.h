#ifndef COMPLETE_RANDOM_CITY_SOLID_H
#define COMPLETE_RANDOM_CITY_SOLID_H

#include "structs.h"
#include "graphics.h"
#include "movableComponent.h"
#include <vector>
#include <experimental/optional>
#include <cmath>

// How many lines are drawn on the solid
enum linesDrawnEnum {NoLines, Normal, Low, Medium, High};

class Solid : public MovableComponent
{
protected:
    Point center;
    RGBAcolor color;
    std::vector<Point> corners;
    double xWidth;
    double yWidth;
    double zWidth;
    RGBAcolor lineColor;
    linesDrawnEnum linesDrawn;

    // When drawing lines on a plane, move the line a little in front of the plane so it shows up
    constexpr const static double lineOffset = 0.5;
public:
    // Constructors
    Solid();
    Solid(Point inputCenter, RGBAcolor inputColor,
          double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
          linesDrawnEnum inputLinesDrawn=Normal);
    Solid(Point inputCenter, RGBAcolor inputColor,
          double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
          Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
          Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn=Normal);

    // Initialization
    virtual void initializeCorners();

    // Getters
    Point getCenter() const;
    RGBAcolor getColor() const;
    std::vector<Point> getCorners() const;
    double getXWidth() const;
    double getYWidth() const;
    double getZWidth() const;
    RGBAcolor getLineColor() const;

    // Setter
    void setCenter(Point inputCenter);
    void setColor(RGBAcolor inputColor);
    void setCorners(std::vector<Point> inputCorners);
    void setXWidth(double inputXWidth);
    void setYWidth(double inputYWidth);
    void setZWidth(double inputZWidth);
    void setLineColor(RGBAcolor inputLineColor);


    // Moving
    void move(double deltaX, double deltaY, double deltaZ);
    void rotate(double thetaX, double thetaY, double thetaZ);
    void rotateAroundOwner(double thetaX, double thetaY, double thetaZ);

    virtual void draw() const = 0;

    // If the p is within buffer of this shape, a corrected position
    // is returned. returns nullopt otherwise.
    virtual std::experimental::optional<Point> correctCollision(Point p, int buffer)=0;

    virtual void printDebugStats()=0;
};

#endif //COMPLETE_RANDOM_CITY_SOLID_H
