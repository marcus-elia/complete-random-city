#include "chunk.h"

Chunk::Chunk()
{
    bottomLeft = {0,0};
    sideLength = 1024;
    perlinSeed = 0.5;
    initializeCenter();
    propertySize = sideLength / 8;
}
Chunk::Chunk(Point2D inputBottomLeft, int inputSideLength, double inputPerlinSeed,
             std::experimental::optional<std::vector<int>> inputLeftRoadIndices,
             std::experimental::optional<std::vector<int>> inputRightRoadIndices,
             std::experimental::optional<std::vector<int>> inputTopRoadIndices,
             std::experimental::optional<std::vector<int>> inputBottomRoadIndices)
{
    bottomLeft = inputBottomLeft;
    sideLength = inputSideLength;
    perlinSeed = inputPerlinSeed;
    initializeCenter();
    propertySize = sideLength / 8;

    // If the roads entering a side are specified, use them. If not, make them randomly
    srand(time(NULL));
    leftRoadIndices = inputLeftRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    rightRoadIndices = inputRightRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    topRoadIndices = inputTopRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    bottomRoadIndices = inputBottomRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
}

void Chunk::initializeCenter()
{
    center = {sideLength*bottomLeft.x + sideLength/2, sideLength*bottomLeft.z - sideLength/2};
}
std::vector<int> Chunk::makeRandomRoadIndices(double seed, int size)
{
    std::vector<int> indices;
    bool roadAtPrev = (rand() % 100) > 50;
    for(int i = 0; i < size; i++)
    {
        // If there isn't a road adjacent to this one and a random value is less than
        // the perlin seed, put a road entering there 4/5 of the time. This means that
        // higher perlin seed means more roads.
        if(!roadAtPrev && (rand() % 100) > 20 && (rand() % 100)/100.0 < seed)
        {
            indices.push_back(i);
            roadAtPrev = true;
        }
        else
        {
            roadAtPrev = false;
        }
    }
    return indices;
}
void Chunk::initializeRoadLocations()
{
    // Initialize the whole chunk to not having roads
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            roadLocations[i][j] = false;
        }
    }

    // Add in roads on the edges
    for(int i : topRoadIndices)
    {
        roadLocations[i][0] = true;
    }
    for(int i : bottomRoadIndices)
    {
        roadLocations[i][7] = true;
    }
    for(int j : leftRoadIndices)
    {
        roadLocations[0][j] = true;
    }
    for(int j : rightRoadIndices)
    {
        roadLocations[7][j] = true;
    }
}




// Getters
Point2D Chunk::getBottomLeft() const
{
    return bottomLeft;
}
int Chunk::getSideLength() const
{
    return sideLength;
}
Point2D Chunk::getCenter() const
{
    return center;
}

void Chunk::draw() const
{
    glBegin(GL_QUADS);
    /*if((bottomLeft.x + bottomLeft.z) % 2 == 0)
    {
        glColor4f(1, 1, 0.3, 1);
    }
    else
    {
        glColor4f(0, 1, 0.8, 1);
    }*/
    //glColor4f(perlinSeed, 0, 1, 1);
    glColor4f(0.5, 0.5, 0.5, 1);
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z);
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z);

    glEnd();
}

int Chunk::chunkToInt() const
{
    return pointToInt({bottomLeft.x, bottomLeft.z});
}

std::vector<Point2D> Chunk::getChunksAround(int radius)
{
    return getChunksAroundPointByPoint(bottomLeft, radius);
}

int pointToInt(Point2D p)
{
    int a = p.x;
    int b = p.z;
    if(a > 0 && b <= a-1 && b > -a)
    {
        return 4*a*a - 3*a - b;
    }
    else if(b < 0 && a <= -b && a > b+1)
    {
        return 4*b*b + b - a;
    }
    else if(a < 0 && b > a-1 && b < -a)
    {
        return 4*a*a - a + b;
    }
    else if(b > -1 && a >= -b && a < b+1)
    {
        return 4*b*b + 3*b + a;
    }
    return (2*a-1)*(2*a-3); // idk why, but this works for (0,-1) and (-1,-2)
}

std::vector<int> getChunksAroundPoint(Point2D p, int radius)
{
    std::vector<int> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back(pointToInt({a,b}));
        }
    }
    return result;
}
std::vector<Point2D> getChunksAroundPointByPoint(Point2D p, int radius)
{
    std::vector<Point2D> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back({a,b});
        }
    }
    return result;
}

void drawPoint(const Point &p)
{
    glVertex3f(p.x, p.y, p.z);
}

bool makesSquareUpLeft(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i-1][j] && roadLocs[i-1][j-1] && roadLocs[i][j-1];
}
bool makesSquareUpRight(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i][j-1] && roadLocs[i+1][j-1] && roadLocs[i+1][j];
}
bool makesSquareDownRight(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i+1][j] && roadLocs[i+1][j+1] && roadLocs[i][j+1];
}
bool makesSquareDownLeft(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i][j+1] && roadLocs[i-1][j+1] && roadLocs[i-1][j];
}
bool makesSquare(int i, int j, bool roadLocs[8][8])
{
    if(i == 0 && j == 0)
    {
        return makesSquareDownRight(i, j, roadLocs);
    }
    if(i == 7 && j == 0)
    {
        return makesSquareDownLeft(i, j, roadLocs);
    }
    if(i == 7 && j == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs);
    }
    if(i == 0 && j == 7)
    {
        return makesSquareUpRight(i, j, roadLocs);
    }
    if(j == 0)
    {
        return makesSquareDownLeft(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs);
    }
    if(i == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs) || makesSquareDownLeft(i, j, roadLocs);
    }
    if(j == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs) || makesSquareUpRight(i, j, roadLocs);
    }
    if(i == 0)
    {
        return makesSquareUpRight(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs);
    }
    return makesSquareUpRight(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs) ||
            makesSquareUpLeft(i, j, roadLocs) || makesSquareDownLeft(i, j, roadLocs);
}