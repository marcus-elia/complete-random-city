#ifndef RANDOM_3D_CITY_CHUNK_H
#define RANDOM_3D_CITY_CHUNK_H




#include "graphics.h"
#include "player.h"
#include "roadPlot.h"
//#include "building.h"
#include <vector>
#include <stdlib.h>
#include <time.h>




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

    int propertySize; // = side length of square assigned to each plot

    // Plots and roads

    // A 2d vector of the plots
    //std::vector<std::vector<std::shared_ptr<Plot>>> plots;
    std::shared_ptr<Plot>[8][8] plots;


    // where roads are
    bool[8][8] roadLocations;

    // Where there are roads leaving/entering this chunk
    std::vector<int> leftRoadIndices, rightRoadIndices, topRoadIndices, bottomRoadIndices;
public:
    Chunk();
    Chunk(Point2D inputBottomLeft, int inputSideLength, double inputPerlinSeed,
            std::experimental::optional<std::vector<int>> inputLeftRoadIndices,
          std::experimental::optional<std::vector<int>> inputRightRoadIndices,
          std::experimental::optional<std::vector<int>> inputTopRoadIndices,
          std::experimental::optional<std::vector<int>> inputBottomRoadIndices);

    // Initialization functions
    void initializeCenter();
    std::vector<int> makeRandomRoadIndices(double seed, int size);
    void initializeRoadLocations();
    void initializePlots();

    // Getters
    Point2D getBottomLeft() const;
    int getSideLength() const;
    Point2D getCenter() const;
    std::vector<int> getLeftRoadIndices() const;
    std::vector<int> getRightRoadIndices() const;
    std::vector<int> getTopRoadIndices() const;
    std::vector<int> getBottomRoadIndices() const;

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

// Helper functions for filling in the road locations
bool makesSquareUpLeft(int i, int j, bool roadLocs[8][8]);
bool makesSquareUpRight(int i, int j, bool roadLocs[8][8]);
bool makesSquareDownRight(int i, int j, bool roadLocs[8][8]);
bool makesSquareDownLeft(int i, int j, bool roadLocs[8][8]);
bool makesSquare(int i, int j, bool roadLocs[8][8]);

// Vector containment
template <class Object>
bool hasElement(std::vector<Object> vec, Object obj);

// Converts indices of where a plot is the chunk to the center of the plot
Point chunkCoordinatesToCenter(int i, int j, Point2D bottomLeft, int propertySize);


#endif //RANDOM_3D_CITY_CHUNK_H
