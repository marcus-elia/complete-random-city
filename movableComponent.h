#ifndef COMPLETE_RANDOM_CITY_MOVABLECOMPONENT_H
#define COMPLETE_RANDOM_CITY_MOVABLECOMPONENT_H

#include "movable.h"

class MovableComponent : public Movable
{
protected:
    // The center point that this will rotate around
    Point* ownerCenter;
public:
    MovableComponent();
    MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity);
    MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            Point* inputOwnerCenter);

    Point* getOwnerCenter() const;

    void setOwnerCenter(Point* inputOwnerCenter);

    virtual void rotateAroundOwner(double thetaX, double thetaY, double thetaZ) = 0;
};

#endif //COMPLETE_RANDOM_CITY_MOVABLECOMPONENT_H
