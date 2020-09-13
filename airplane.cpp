#include "airplane.h"

Airplane::Airplane() : Vehicle()
{
    cruisingAltitude = 300;
    runwayStart = {0,0, 0};
    runwayEnd = {0, 0, -100};
    length = 20;
    height = 10;
    width = 10;
    isTakingOff = true;
    startOnRunway();
    initializeSolids();
    double runwayAngle = atan2(runwayEnd.z - runwayStart.z, runwayEnd.x - runwayStart.x);
    setXZAngle(runwayAngle);
}
Airplane::Airplane(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
double inputCruisingAltitude, Point inputRunwayStart, Point inputRunwayEnd,
double inputLength, double inputHeight, double inputWidth, RGBAcolor startingHitboxColor)
: Vehicle(inputLocation, inputLookingAt, inputSpeed, inputVelocity, startingHitboxColor)
{
    cruisingAltitude = inputCruisingAltitude;
    runwayStart = inputRunwayStart;
    runwayEnd = inputRunwayEnd;
    length = inputLength;
    height = inputHeight;
    width = inputWidth;
    isTakingOff = true;
    startOnRunway();
    initializeSolids();
    initializeHitbox(startingHitboxColor);
    double runwayAngle = atan2(runwayEnd.z - runwayStart.z, runwayEnd.x - runwayStart.x);
    setXZAngle(runwayAngle);
}

void Airplane::startOnRunway()
{
    location = {runwayStart.x, height/2, runwayStart.z};
}

void Airplane::initializeSolids()
{
    Point solidCenter;
    RGBAcolor navy = {0, 0, 0.4, 1};
    RGBAcolor yellow = {1, 0.9, 0, 0};
    // Main body
    solidCenter = location;
    double bodyWidth = width/4;
    double bodyThickness = height/4;
    solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter,
                                                         navy, bodyWidth, bodyThickness, length/2,{1,1,1,1},
                                                         solidCenter, lookingAt,  0,
                                                         {0,0,0},  location)));
    double wingLength = width;
    double innerWingWidth = length/3;
    double outerWingWidth = length/6;
    // Left wing
    solidCenter = {location.x - width/2 - bodyWidth/2, location.y, location.z};
    std::shared_ptr<RightTrapezoid> leftWing = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
            navy, 2, wingLength, innerWingWidth, {1,1,1,1}, outerWingWidth,
            solidCenter, lookingAt,  0, {0,0,0},  location));
    leftWing->rotate(0, 0, PI/2);
    solids.push_back(leftWing);
    // Right wing
    solidCenter = {location.x + width/2 + bodyWidth/2, location.y, location.z};
    std::shared_ptr<RightTrapezoid> rightWing = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
                                   navy, 2, wingLength, innerWingWidth, {1,1,1,1}, outerWingWidth,
                                   solidCenter, lookingAt,  0, {0,0,0},  location));
    rightWing->rotate(0, 0, -PI/2);
    solids.push_back(rightWing);

    // Nose
    solidCenter = {location.x, location.y, location.z - length/2};
    std::shared_ptr<Frustum> nose = std::make_shared<Frustum>(Frustum(solidCenter, navy, bodyWidth, length/2,
                                                                         bodyThickness, {1,1,1,1}, 2, 2,
            solidCenter, lookingAt, 0, {0,0,0}, location));
    nose->rotate(-PI/2, 0, 0);
    solids.push_back(nose);

    // Wing tips
    solidCenter = {location.x - width - bodyWidth/2 - 1, location.y, location.z + outerWingWidth/2};
    solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter, yellow, 2, 2,
            outerWingWidth + 2, {1,1,1,1},
            solidCenter, lookingAt, 0, {0,0,0},
            location)));
    solidCenter = {location.x + width + bodyWidth/2 + 1, location.y, location.z + outerWingWidth/2};
    solids.push_back(std::make_shared<RecPrism>(RecPrism(solidCenter, yellow, 2, 2,
                                                         outerWingWidth + 2, {1,1,1,1},
                                                         solidCenter, lookingAt, 0, {0,0,0},
                                                         location)));

    // Rear body
    solidCenter = {location.x, location.y, location.z + length/4 + length/8};
    std::shared_ptr<Frustum> rearBody = std::make_shared<Frustum>(Frustum(solidCenter, navy, bodyWidth, length/4,
                                         bodyThickness, {1,1,1,1}, 6, 6,
                                         solidCenter, lookingAt, 0, {0,0,0}, location));
    rearBody->rotate(PI/2, 0, 0);
    solids.push_back(rearBody);

    double rearWingLength = width/2;
    double innerRearWingWidth = length/5;
    double outerRearWingWidth = length/8;
    // Left rear wing
    solidCenter = {location.x - rearWingLength/2 - bodyWidth/2, location.y, location.z + length/4 + innerRearWingWidth/2};
    std::shared_ptr<RightTrapezoid> leftRearWing = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
                                  navy, 2, rearWingLength, innerRearWingWidth, {1,1,1,1}, outerRearWingWidth,
                                  solidCenter, lookingAt,  0, {0,0,0},  location));
    leftRearWing->rotate(0, 0, PI/2);
    solids.push_back(leftRearWing);
    // Right Rear wing
    solidCenter = {location.x + rearWingLength/2 + bodyWidth/2, location.y, location.z + length/4 + innerRearWingWidth/2};
    std::shared_ptr<RightTrapezoid> rightRearWing = std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter,
                                          navy, 2, rearWingLength, innerRearWingWidth, {1,1,1,1}, outerRearWingWidth,
                                          solidCenter, lookingAt,  0, {0,0,0},  location));
    rightRearWing->rotate(0, 0, -PI/2);
    solids.push_back(rightRearWing);

    // Left fin
    double finWidth = 2;
    double finHeight = bodyWidth;
    double finLength = length/4;
    solidCenter = {location.x - bodyWidth/2 + finWidth/2, location.y + finHeight/2, location.z + length/4 + finLength/4};
    solids.push_back(std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter, navy, finWidth, finHeight, finLength,
                                                                     {1,1,1,1}, finLength/2,
                                                                     solidCenter, lookingAt, 0, {0,0,0}, location)));
    // Right fin
    solidCenter = {location.x + bodyWidth/2 - finWidth/2, location.y + finHeight/2, location.z + length/4 + finLength/4};
    solids.push_back(std::make_shared<RightTrapezoid>(RightTrapezoid(solidCenter, navy, finWidth, finHeight, finLength,
                                                                     {1,1,1,1}, finLength/2,
                                                                     solidCenter, lookingAt, 0, {0,0,0}, location)));

    // Cabin
    solidCenter = {location.x, location.y + bodyThickness/2, location.z - length/3};
    solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(solidCenter, {.2,.2,1,0}, bodyWidth/2, bodyThickness-2, length/4,
                                                           {1,1,1,1}, solidCenter, lookingAt, 0, {0,0,0}, location)));

}
void Airplane::initializeHitbox(RGBAcolor startingHitboxColor)
{
    hitbox = std::make_shared<RecPrism>(RecPrism(location, startingHitboxColor, width, height, length, {1,1,1,1}));
    hitbox->rotate(0, xzAngle - 3*PI/2, 0);

    frontCollisionPoint = {location.x, location.y, location.z - 7*length/16};
    rotatePointAroundPoint(frontCollisionPoint, location, 0, xzAngle - 3*PI/2, 0);
    backCollisionPoint = {location.x, location.y, location.z + 7*length/16};
    rotatePointAroundPoint(backCollisionPoint, location, 0, xzAngle - 3*PI/2, 0);
}

void Airplane::tick()
{
    if(isTakingOff)
    {
        move(velocity.x, velocity.y, velocity.z);
        velocity.y = distance2d(location, runwayStart) / distance2d(runwayStart, runwayEnd);
        speed += 0.1;
        updateVelocity();

        if(distance2d(location, runwayEnd) < speed)
        {
            isTakingOff = false;
        }
    }
    else
    {
        move(velocity.x, velocity.y, velocity.z);
        if(distance2d(location, runwayStart) > 512)
        {
            setXZAngle(xzAngle + 0.05);
        }
        if(location.y >= cruisingAltitude)
        {
            velocity.y = 0;
        }
    }
}

void Airplane::draw() const
{
    glLineWidth(1.0);
    for(auto s : solids)
    {
        s->draw();
    }
    glLineWidth(2.0);
}
void Airplane::drawHitbox() const
{
    hitbox->draw();
}