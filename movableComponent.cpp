#include "movableComponent.h"

MovableComponent::MovableComponent() : Movable()
{
    ownerCenter = location;
}
MovableComponent::MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity) :
Movable(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    ownerCenter = location;
}
MovableComponent::MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) :
        Movable(inputLocation, inputLookingAt, inputSpeed, inputVelocity)
{
    ownerCenter = inputOwnerCenter;
}

Point MovableComponent::getOwnerCenter() const
{
    return ownerCenter;
}

void MovableComponent::setOwnerCenter(Point inputOwnerCenter)
{
    ownerCenter = inputOwnerCenter;
}