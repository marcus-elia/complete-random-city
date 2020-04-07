#ifndef RANDOM_3D_CITY_CHUNK_H
#define RANDOM_3D_CITY_CHUNK_H




#include "graphics.h"
#include "player.h"
//#include "building.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

// int points used for the chunks
// in the xz plane
struct Point2D
{
    int x;
    int z;
    bool operator ==(const Point2D& p1)
    {
        return x == p1.x && z == p1.z;
    }
    bool operator !=(const Point2D& p1)
    {
        return x != p1.x || z != p1.z;
    }
};


class Chunk
{
private:
    Point2D bottomLeft; // The bottom left coordinate divided by sideLength
    int sideLength;
    Point2D center;   // The actual center

    // The number of the chunk based on its location
    int chunkID;

    // The value of the Perlin noise map for this chunk
    double perlinSeed;

    int propertySize; // = side length of square assigned to each building
public:
    Chunk();
    Chunk(Point2D inputBottomLeft, int inputSideLength, double inputPerlinSeed);

    void initializeCenter();

    // Getters
    Point2D getBottomLeft() const;
    int getSideLength() const;
    Point2D getCenter() const;

    void draw() const;

    // Wrapper function, returns chunks around this
    std::vector<Point2D> getChunksAround(int radius);

    // Divides by size and converts resulting ordered pair to int
    int chunkToInt() const;
};

// Returns the result of mapping a 2d point in Z x Z into the
// non-negative integers, by spiraling clockwise.
int pointToInt(Point2D p);

// Returns the ints corresponding to to all chunks that are within radius of this one,
// using the taxicab metric
std::vector<int> getChunksAroundPoint(Point2D p, int radius);
std::vector<Point2D> getChunksAroundPointByPoint(Point2D p, int radius);

// Makes a call to GL's vertex3f function
void drawPoint(const Point &p);


#endif //RANDOM_3D_CITY_CHUNK_H
