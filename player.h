#ifndef RANDOM_3D_CITY_PLAYER_H
#define RANDOM_3D_CITY_PLAYER_H

#include "graphics.h"
#include "chunk.h"

struct Point
{
    double x;
    double y;
    double z;
};


class Player
{
private:
    Vector3 location;
    Vector3 lookingAt;
    Vector3 up;

    double speed;     // how fast the player can move
    Vector3 velocity; // current x y and z velocity

    double sensitivity; // turning speed for mouse movement

    int chunkSize;
    Point2D currentChunkCoords; // which chunk the player is in
public:
    Player();
    Player(Vector3 inputLocation, Vector3 inputLookingAt, Vector3 inputUp, double inputSpeed, int inputChunkSize);

    // Getters
    Vector3 getLocation() const;
    Vector3 getLookingAt() const;
    Vector3 getUp() const;
    double getSpeed() const;
    Point2D getCurrentChunkCoords() const;
    int getCurrentChunkInt() const;

    // Setters
    void setLocation(Vector3 inputLocation);
    void setLookingAt(Vector3 inputLookingAt);
    void setUp(Vector3 inputUp);
    void setSpeed(double inputSpeed);
    void setCurrentChunkCoords(Point2D input);

    // Movement

    // Based on which keys are pressed, set the velocity
    void setVelocity(bool wKey, bool aKey, bool sKey, bool dKey, bool rKey, bool cKey);

    // Rotates lookingAt around location in the xz plane
    void rotateLookingAtHorizontal(double theta);

    // Rotates lookingAt around location vertically
    void rotateLookingAtVertical(double theta);

    // Based on the angle of mouse movement, change the lookingat
    void updateLookingAt(double theta);


    // Chunks

    // Returns the Point2D of the chunk underneath where the player is
    Point2D whatChunk() const;

    void tick();

    // Check every Building in the given Chunk to see if we are hitting it,
    // and move according to what the Building says if we are
    void checkCollisionsAndCorrect(Chunk &c, int buffer);
};

#endif //RANDOM_3D_CITY_PLAYER_H
