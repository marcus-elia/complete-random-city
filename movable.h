#ifndef COMPLETE_RANDOM_CITY_MOVABLE_H
#define COMPLETE_RANDOM_CITY_MOVABLE_H

#include "structs.h"
#include <vector>
#include <memory>
#include <cmath>

class Movable
{
protected:
    Point location;
    Point lookingAt;
    double speed;
    Point velocity;
    double xzAngle; // angle in the xz-plane
public:
    Movable();
    Movable(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity);

    // Set the magnitude of the velocity to be the speed
    void updateVelocity();

    // Getters
    Point getLocation() const;
    Point getLookingAt() const;
    double getSpeed() const;
    Point getVelocity() const;
    double getXZAngle() const;

    // Setters
    void setLocation(Point inputLocation);
    virtual void setLookingAt(Point inputLookingAt);
    void setSpeed(double inputSpeed);
    void setVelocity(Point inputVelocity);
    virtual void setXZAngle(double inputAngle);

    virtual void move(double deltaX, double deltaY, double deltaZ);
    virtual void rotate(double thetaX, double thetaY, double thetaZ);

    virtual void tick();

    virtual void draw() const = 0;
};

// Translates Point p
void movePoint(Point &p, double deltaX, double deltaY, double deltaZ);

// Rotates p around pBase in 3D space
void rotatePointAroundPoint(Point &p, const Point &pBase, double thetaX, double thetaY, double thetaZ);

#endif //COMPLETE_RANDOM_CITY_MOVABLE_H
