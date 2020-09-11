#include "player.h"
#include <cmath>

Player::Player()
{
    location = {0, 0, 0};
    lookingAt = {0,0,-60};
    up = {0, 1, 0};
    initializeAngles();
    initializeSphericalDirection();
    speed = 4;
    velocity = {0,0,0};
    sensitivity = 0.005;
    chunkSize = 512;
    currentChunkCoords = whatChunk();
}
Player::Player(Point inputLocation, Point inputLookingAt, Point inputUp, double inputSpeed, int inputChunkSize)
{
    location = inputLocation;
    lookingAt = inputLookingAt;
    up = inputUp;
    initializeAngles();
    initializeSphericalDirection();
    speed = inputSpeed;
    velocity = {0,0,0};
    sensitivity = 0.02;
    chunkSize = inputChunkSize;
    currentChunkCoords = whatChunk();
}

void Player::initializeAngles()
{
    xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);
    yAngle = atan2(lookingAt.y - location.y, distance2d(location, lookingAt));
}
void Player::initializeSphericalDirection()
{
    sphericalDirection.x = cos(xzAngle);
    sphericalDirection.z = sin(xzAngle);

    // Must scale the component in the xz plane for spherical coordinates
    double scaleAmount = cos(yAngle) / sqrt(sphericalDirection.x*sphericalDirection.x + sphericalDirection.z*sphericalDirection.z);
    sphericalDirection.x *= scaleAmount;
    sphericalDirection.z *= scaleAmount;
    sphericalDirection.y = sin(yAngle);
}




// Getters
Point Player::getLocation() const
{
    return location;
}
Point Player::getLookingAt() const
{
    return lookingAt;
}
Point Player::getUp() const
{
    return up;
}
double Player::getXZAngle() const
{
    return xzAngle;
}
double Player::getYAngle() const
{
    return yAngle;
}
double Player::getSpeed() const
{
    return speed;
}
Point2D Player::getCurrentChunkCoords() const
{
    return currentChunkCoords;
}
int Player::getCurrentChunkInt() const
{
    return pointToInt(currentChunkCoords);
}

// Setters
void Player::setLocation(Point inputLocation)
{
    location = inputLocation;
}
void Player::setLookingAt(Point inputLookingAt)
{
    lookingAt = inputLookingAt;
}
void Player::setUp(Point inputUp)
{
    up = inputUp;
}
void Player::setSpeed(double inputSpeed)
{
    speed = inputSpeed;
}
void Player::setCurrentChunkCoords(Point2D input)
{
    currentChunkCoords = input;
}


void Player::setVelocity(bool wKey, bool aKey, bool sKey, bool dKey, bool rKey, bool cKey)
{
    bool forward = wKey && !sKey;
    bool backward = !wKey && sKey;
    bool left = aKey && !dKey;
    bool right = !aKey && dKey;
    bool ascending = rKey && !cKey; // name up already used :(
    bool descending = !rKey && cKey;

    // First figure out the vertical (y) velocity, since that is independent
    if(ascending)
    {
        velocity.y = speed;
    }
    else if(descending)
    {
        velocity.y = -speed;
    }
    else
    {
        velocity.y = 0;
    }

    // The angle the player is facing in the xz plane
    double xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);

    // The angle the player should move based on input
    double angleToMove;
    if(forward)
    {
        if(right)
        {
            angleToMove = xzAngle + PI/4;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI/4;
        }
        else
        {
            angleToMove = xzAngle;
        }
    }
    else if(backward)
    {
        if(right)
        {
            angleToMove = xzAngle - PI - PI/4;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI + PI/4;
        }
        else
        {
            angleToMove = xzAngle - PI;
        }
    }
    else
    {
        if(right)
        {
            angleToMove = xzAngle + PI/2;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI/2;
        }
        else
        {
            velocity.x = 0;
            velocity.z = 0;
            return;
        }
    }
    velocity.x = speed * cos(angleToMove);
    velocity.z = speed * sin(angleToMove);
}

/*void Player::rotateLookingAtHorizontal(double theta)
{
    // Translate to the origin
    lookingAt.x -= location.x;
    lookingAt.z -= location.z;

    // Rotate around y-axis
    double prevX = lookingAt.x;
    double prevZ = lookingAt.z;
    lookingAt.x = prevX * cos(theta) - prevZ * sin(theta);
    lookingAt.z = prevX * sin(theta) + prevZ * cos(theta);

    // Translate back
    lookingAt.x += location.x;
    lookingAt.z += location.z;
}

void Player::rotateLookingAtVertical(double theta)
{
    // Rotate around the y axis to get on the +x axis
    double xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);
    rotateLookingAtHorizontal(2*PI - xzAngle);

    // Translate to origin
    lookingAt.x -= location.x;
    lookingAt.y -= location.y;

    // Clip the angle between -Pi and Pi
    double curAngle = atan2(lookingAt.y, lookingAt.x);
    if(curAngle + theta > PI)
    {
        theta = PI - curAngle;
    }
    else if(curAngle + theta < -PI)
    {
        theta = -PI - curAngle;
    }

    // Rotate around y-axis
    double prevX = lookingAt.x;
    double prevY = lookingAt.y;
    lookingAt.x = prevX * cos(theta) - prevY * sin(theta);
    lookingAt.y = prevX * sin(theta) + prevY * cos(theta);

    // Translate back
    lookingAt.x += location.x;
    lookingAt.y += location.y;

    // Unrotate horizontally
    rotateLookingAtHorizontal(xzAngle);
}

void Player::updateLookingAt(double theta)
{
    // Horizontal turning
    double horizontalAmount = sensitivity * cos(theta);
    rotateLookingAtHorizontal(horizontalAmount);

    // Vertical turning
    double verticalAmount = sensitivity * sin(theta);
    rotateLookingAtVertical(-verticalAmount); // negative sign since we are rotating up from +x axis
}*/

// Update the xzAngle and yAngle based on theta resulting from a mouse movement
void Player::updateAngles(double theta, double distance)
{
    std::cout << distance << std::endl;
    double horizontalAmount = sensitivity * distance* cos(theta);
    xzAngle += horizontalAmount;
    if(xzAngle > 2*PI)
    {
        xzAngle -= 2*PI;
    }
    else if(xzAngle < 0)
    {
        xzAngle += 2*PI;
    }

    double verticalAmount = sensitivity * distance* sin(theta);
    yAngle -= verticalAmount; // negative sign since Glut's mouse functions treat +y as down
    if(yAngle > PI/2 - VERTICAL_LIMIT)
    {
        yAngle = PI/2 - VERTICAL_LIMIT;
    }
    else if(yAngle < -PI/2 + VERTICAL_LIMIT)
    {
        yAngle = -PI/2 + VERTICAL_LIMIT;
    }
}

// Use xzAngle, yAngle, and location to determine the spherical direction.
void Player::updateSphericalDirectionBasedOnAngles()
{
    sphericalDirection.x = cos(xzAngle);
    sphericalDirection.z = sin(xzAngle);

    // Must scale the component in the xz plane for spherical coordinates
    double scaleAmount = cos(yAngle) / sqrt(sphericalDirection.x*sphericalDirection.x + sphericalDirection.z*sphericalDirection.z);
    sphericalDirection.x *= scaleAmount;
    sphericalDirection.z *= scaleAmount;
    sphericalDirection.y = sin(yAngle);

    lookingAt.x = location.x + sphericalDirection.x;
    lookingAt.y = location.y + sphericalDirection.y;
    lookingAt.z = location.z + sphericalDirection.z;
}


void Player::tick()
{
    location.x += velocity.x;
    lookingAt.x += velocity.x;
    location.y += velocity.y;
    lookingAt.y += velocity.y;
    location.z += velocity.z;
    lookingAt.z += velocity.z;
    if(location.y < 5)
    {
        lookingAt.y += 5 - location.y;
        location.y = 5;
    }
}

Point2D Player::whatChunk() const
{
    return {(int)floor(location.x / chunkSize), (int)floor(location.z / chunkSize)};
}

void Player::checkCollisionsAndCorrect(Chunk &c, int buffer)
{
    /*std::experimental::optional<Point> newPoint = c.correctCollision({location.x, location.y, location.z}, buffer);
    if(newPoint)
    {
        Point p = newPoint.value();
        lookingAt.x += p.x - location.x;
        lookingAt.y += p.y - location.y;
        lookingAt.z += p.z - location.z;
        location.x = p.x;
        location.y = p.y;
        location.z = p.z;
    }*/
}