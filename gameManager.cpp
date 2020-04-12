#include "gameManager.h"

GameManager::GameManager()
{
    perlinSize = 10;
    png = PerlinNoiseGenerator(10, 10, 1);
    chunkSize = 512;
    renderRadius = 3;
    updateCurrentChunks();
}
GameManager::GameManager(int inputChunkSize, int inputRenderRadius, int inputPerlinSize)
{
    chunkSize = inputChunkSize;
    renderRadius = inputRenderRadius;
    perlinSize = inputPerlinSize;
    png = PerlinNoiseGenerator(perlinSize, perlinSize, 1);
    updateCurrentChunks();
}

void GameManager::reactToMouseMovement(double theta)
{
    player.updateLookingAt(theta);
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::reactToMouseClick()
{
    createCar();
}


void GameManager::draw() const
{
    for(auto &c : currentChunks)
    {
        c->draw();
    }


    for(std::shared_ptr<Vehicle> v : vehicles)
    {
        v->draw();
    }
}

void GameManager::tick()
{
    // The player moves
    player.tick();

    // Check for the player hitting a building
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[pointToInt(curPlayerChunk)];
    player.checkCollisionsAndCorrect(*c, 5);

    // If the player is entering a different chunk
    if(curPlayerChunk != player.getCurrentChunkCoords())
    {
        //std::cout << pointToInt({player.whatChunk().x, player.whatChunk().z}) << std::endl;
        //std::cout << player.getLocation().x << ", " << player.getLocation().z << std::endl;
        updateCurrentChunks();
    }

    // Vehicles move
    for(std::shared_ptr<Vehicle> v : vehicles)
    {
        v->tick();
    }
}

Player GameManager::getPlayer() const
{
    return player;
}
bool GameManager::getWKey()
{
    return wKey;
}
bool GameManager::getAKey()
{
    return aKey;
}
bool GameManager::getSKey()
{
    return sKey;
}
bool GameManager::getDKey()
{
    return dKey;
}
bool GameManager::getRKey()
{
    return rKey;
}
bool GameManager::getCKey()
{
    return cKey;
}

void GameManager::setWKey(bool input)
{
    wKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setAKey(bool input)
{
    aKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setSKey(bool input)
{
    sKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setDKey(bool input)
{
    dKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setRKey(bool input)
{
    rKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setCKey(bool input)
{
    cKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}

// ============================
//
//       Managing Chunks
//
// ============================
double GameManager::getPerlinValue(Point2D p)
{
    return png.getPerlinNoise()[mod(p.x, perlinSize)][mod(p.z, perlinSize)];
}

void GameManager::updateCurrentChunks()
{
    player.setCurrentChunkCoords(player.whatChunk());

    // Update the list of current chunks
    currentChunks = std::vector<std::shared_ptr<Chunk>>();
    std::vector<Point2D> chunksInRadius = getChunksAroundPointByPoint(player.getCurrentChunkCoords(), renderRadius);
    for(Point2D p : chunksInRadius)
    {
        int index = pointToInt(p);
        if(allSeenChunks.count(index) == 0) // if the chunk has never been seen before, make a new one
        {
            // Check for adjacent chunks to get edge road indices
            std::experimental::optional<std::shared_ptr<Chunk>> leftChunk, rightChunk, topChunk, bottomChunk;
            std::experimental::optional<std::vector<int>> inputLeftRoadIndices, inputRightRoadIndices,
                                                          inputTopRoadIndices, inputBottomRoadIndices;
            // Left border
            if(allSeenChunks.count(pointToInt({p.x - 1, p.z})) > 0)
            {
                leftChunk = allSeenChunks[pointToInt({p.x - 1, p.z})];
                inputLeftRoadIndices = leftChunk.value()->getRightRoadIndices();
            }
            else
            {
                leftChunk = std::experimental::nullopt;
                inputLeftRoadIndices = std::experimental::nullopt;
            }
            // Right border
            if(allSeenChunks.count(pointToInt({p.x + 1, p.z})) > 0)
            {
                rightChunk = allSeenChunks[pointToInt({p.x + 1, p.z})];
                inputRightRoadIndices = rightChunk.value()->getLeftRoadIndices();
            }
            else
            {
                rightChunk = std::experimental::nullopt;
                inputRightRoadIndices = std::experimental::nullopt;
            }
            // Top border
            if(allSeenChunks.count(pointToInt({p.x, p.z - 1})) > 0)
            {
                topChunk = allSeenChunks[pointToInt({p.x, p.z - 1})];
                inputTopRoadIndices = topChunk.value()->getBottomRoadIndices();
            }
            else
            {
                topChunk = std::experimental::nullopt;
                inputTopRoadIndices = std::experimental::nullopt;
            }
            // Bottom border
            if(allSeenChunks.count(pointToInt({p.x, p.z + 1})) > 0)
            {
                bottomChunk = allSeenChunks[pointToInt({p.x, p.z + 1})];
                inputBottomRoadIndices = bottomChunk.value()->getTopRoadIndices();
            }
            else
            {
                bottomChunk = std::experimental::nullopt;
                inputBottomRoadIndices = std::experimental::nullopt;
            }
            // Make the new chunk
            allSeenChunks[index] = std::make_shared<Chunk>(p, chunkSize, getPerlinValue(p), inputLeftRoadIndices,
                    inputRightRoadIndices, inputTopRoadIndices, inputBottomRoadIndices);
            // Update the road pointers
            allSeenChunks[index]->setRoadPlotPointers(leftChunk, rightChunk, topChunk, bottomChunk);
        }
        currentChunks.push_back(allSeenChunks[index]);
    }
}
std::shared_ptr<Chunk> GameManager::pointToChunk(Point p)
{
    int chunkIndex = pointToInt({(int)floor(p.x / chunkSize), (int)floor(p.z / chunkSize)});
    if(allSeenChunks.count(chunkIndex) == 0)
    {
        chunkIndex = 0; // If it's out of bounds, just check the 0 chunk for now. can't hurt.
    }
    return allSeenChunks[chunkIndex];
}


// Vehicles
bool GameManager::createCar()
{
    // Pointer to the player's current chunk
    Chunk* c = allSeenChunks[pointToInt(player.whatChunk())].get();
    std::experimental::optional<RoadPlot*> roadPlot = c->getRandomRoadPlot();
    if(!roadPlot)
    {
        return false;
    }
    RoadPlot* rp = roadPlot.value();
    Point location = {(double)rp->getCenter().x, 10, (double)rp->getCenter().z};
    Point lookingAt = {location.x, location.y, location.z - 10};
    vehicles.push_back(std::make_shared<Car>(Car(location, lookingAt, 5, {0,0,-1},
            30, 20, 20, {1, 1, 0, 1}, rp)));
    return true;
}





// Camera
Point GameManager::getCameraLocation() const
{
    return player.getLocation();
}
Point GameManager::getCameraLookingAt() const
{
    return player.getLookingAt();
}
Point GameManager::getCameraUp() const
{
    return player.getUp();
}

int mod(int a, int m)
{
    int x = a % m;
    if(x < 0)
    {
        return x + m;
    }
    return x;
}

void GameManager::printPlayerBuildingDebug()
{
    Point v = player.getLocation();
    std::cout << std::endl<< "Player location: " << v.x << "," << v.y << "," << v.z << std::endl;
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[pointToInt(curPlayerChunk)];
}