#ifndef RANDOM_3D_CITY_PLAYER_H
#define RANDOM_3D_CITY_PLAYER_H

#include "graphics.h"
#include "chunk.h"
#include "structs.h"

class Player
{
private:
    Point location;
    Point lookingAt;
    Point up;

    // The direction the player is facing in spherical coordinates
    // Must be on the unit sphere
    Point sphericalDirection;

    double xzAngle; // Where the player is looking in the xz-plane
    double yAngle;  // How much the player is looking up (in [-Pi/2, Pi/2])

    double speed;     // how fast the player can move
    Point velocity; // current x y and z velocity

    double sensitivity; // turning speed for mouse movement

    int chunkSize;
    Point2D currentChunkCoords; // which chunk the player is in

    // Can't look perfectly vertically or it will glitch
    double VERTICAL_LIMIT = 0.01;
public:
    Player();
    Player(Point inputLocation, Point inputLookingAt, Point inputUp, double inputSpeed, int inputChunkSize);

    void initializeAngles();
    void initializeSphericalDirection();

    // Getters
    Point getLocation() const;
    Point getLookingAt() const;
    Point getUp() const;
    double getXZAngle() const;
    double getYAngle() const;
    double getSpeed() const;
    Point2D getCurrentChunkCoords() const;
    int getCurrentChunkInt() const;

    // Setters
    void setLocation(Point inputLocation);
    void setLookingAt(Point inputLookingAt);
    void setUp(Point inputUp);
    void setSpeed(double inputSpeed);
    void setCurrentChunkCoords(Point2D input);

    // Movement

    // Based on which keys are pressed, set the velocity
    void setVelocity(bool wKey, bool aKey, bool sKey, bool dKey, bool rKey, bool cKey);

    // Update the xzAngle and yAngle based on theta resulting from a mouse movement
    void updateAngles(double theta, double distance);

    // Use xzAngle, yAngle, and location to determine the spherical direction.
    void updateSphericalDirectionBasedOnAngles();


    // Chunks

    // Returns the Point2D of the chunk underneath where the player is
    Point2D whatChunk() const;

    void tick();

    // Check every Building in the given Chunk to see if we are hitting it,
    // and move according to what the Building says if we are
    void checkCollisionsAndCorrect(Chunk &c, int buffer);
};

#endif //RANDOM_3D_CITY_PLAYER_H
