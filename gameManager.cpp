#include "gameManager.h"

GameManager::GameManager()
{
    tickNumberMod100 = 0;
    png = PerlinNoiseGenerator(10, 10, 1);
    renderRadius = 3;
    updateCurrentChunks();
    initializeKeys();
}
GameManager::GameManager(int inputRenderRadius)
{
    tickNumberMod100 = 0;
    renderRadius = inputRenderRadius;
    png = PerlinNoiseGenerator(PERLIN_SIZE, PERLIN_SIZE, 1);
    updateCurrentChunks();
    initializeKeys();
}


void GameManager::initializeKeys()
{
    aKey = false, sKey = false, wKey = false, dKey = false, rKey = false, cKey = false;
}


void GameManager::reactToMouseMovement(int mx, int my, double theta, double distance)
{
    double clampedDistance = fmin(distance, 50);
    player.updateAngles(theta, clampedDistance);
    player.updateSphericalDirectionBasedOnAngles();
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::reactToMouseClick(int mx, int my)
{
    std::shared_ptr<Chunk> playerChunk = allSeenChunks[player.getCurrentChunkInt()];
    if(playerChunk->hasAirport())
    {
        playerChunk->makeAirportCreatePlane();
    }
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
    for(std::shared_ptr<Dirigible> d : dirigibles)
    {
        d->draw();
    }
}

void GameManager::tick()
{
    for(auto &c : currentChunks)
    {
        c->tick();
    }

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
    for(std::shared_ptr<Dirigible> d : dirigibles)
    {
        d->tick();
    }

    tickNumberMod100++;
    if(tickNumberMod100 == 100)
    {
        tickNumberMod100 = 0;
        manageCars();
    }
    else if(tickNumberMod100 == 50)
    {
        manageDirigibles();
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
    return png.getPerlinNoise()[mod(p.x, PERLIN_SIZE)][mod(p.z, PERLIN_SIZE)];
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
            allSeenChunks[index] = std::make_shared<Chunk>(p, CHUNK_SIZE, PLOTS_PER_SIDE, getPerlinValue(p), inputLeftRoadIndices,
                    inputRightRoadIndices, inputTopRoadIndices, inputBottomRoadIndices);
            // Update the road pointers
            allSeenChunks[index]->setRoadPlotPointers(leftChunk, rightChunk, topChunk, bottomChunk);
        }
        currentChunks.push_back(allSeenChunks[index]);
    }
}
std::shared_ptr<Chunk> GameManager::pointToChunk(Point p)
{
    int chunkIndex = pointToInt({(int)floor(p.x / CHUNK_SIZE), (int)floor(p.z / CHUNK_SIZE)});
    if(allSeenChunks.count(chunkIndex) == 0)
    {
        chunkIndex = 0; // If it's out of bounds, just check the 0 chunk for now. can't hurt.
    }
    return allSeenChunks[chunkIndex];
}


// Vehicles
void GameManager::manageCars()
{
    // Remove any cars too far away
    int L = vehicles.size();
    int i = 0;
    while(i < L)
    {
        std::shared_ptr<Vehicle> v = vehicles[i];
        if(distance2d(v->getLocation(), player.getLocation()) > 2*CHUNK_SIZE)
        {
            vehicles.erase(vehicles.begin() + i);
            L--;
            i--;
        }
        i++;
    }

    // Add a new car if needed
    while(vehicles.size() < MAX_NUM_VEHICLES)
    {
        createCar();
    }
}

bool GameManager::createCar()
{
    // Pointer to the player's current chunk
    Chunk* c = allSeenChunks[pointToInt(player.whatChunk())].get();
    std::experimental::optional<RoadPlot*> roadPlot = c->getRandomRoadPlot();
    if(!roadPlot)
    {
        return false;
    }
    // Determine where to put the new car
    RoadPlot* rp = roadPlot.value();
    Point location = {(double)rp->getCenter().x, 10, (double)rp->getCenter().z};
    Point lookingAt = {location.x, location.y, location.z - 10};

    // Determine what kind of car it will be
    int r1 = rand();
    int r2 = rand();
    typeOfCar inputCarType;
    RGBAcolor inputColor;
    double carLength;
    if(r1 % 100 < 40)
    {
        inputCarType = Sedan;
        carLength = 22;
    }
    else if(r1 % 100 < 70)
    {
        inputCarType = SUV;
        carLength = 26;
    }
    else
    {
        inputCarType = PickupTruck;
        carLength = 26;
    }
    if(r2 % 100 < 15)
    {
        inputColor = {0.8, 0, 0.1, 1}; // Red
    }
    else if(r2 % 100 < 30)
    {
        inputColor = {0, 0.4, 0.4, 1};  // Dark Cyan
    }
    else if(r2 % 100 < 45)
    {
        inputColor = {.9,.9,0,1}; // Yellow
    }
    else if(r2 % 100 < 60)
    {
        inputColor = {0.15, 0.15, 0.15, 1}; // Dark Gray
    }
    else if(r2 % 100 < 70)
    {
        inputColor = {0, 0.7, 0, 1}; // Green
    }
    else if(r2 % 100 < 85)
    {
        inputColor = {.9, .9, 0.7, 1}; // Off-white
    }
    else
    {
        inputColor = {0,0,1,1}; // Blue
    }
    vehicles.push_back(std::make_shared<Car>(Car(location, lookingAt, 2, {0,0,-1},
            25, 10, rp->getLaneWidth() - 4, inputColor,
            inputCarType, rp)));
    return true;
}


void GameManager::manageDirigibles()
{
    // Remove any airships too far away
    int L = dirigibles.size();
    int i = 0;
    while(i < L)
    {
        std::shared_ptr<Dirigible> d = dirigibles[i];
        if(distance2d(d->getLocation(), player.getLocation()) > 2.25*CHUNK_SIZE)
        {
            dirigibles.erase(dirigibles.begin() + i);
            L--;
            i--;
        }
        i++;
    }

    // Add a new airship if needed
    if(dirigibles.size() < MAX_NUM_DIRIGIBLES)
    {
        createDirigible();
    }
}
bool GameManager::createDirigible()
{
    int r1 = rand() % 100;
    int r2 = rand() % 100;
    double randAngle = (rand() % 100) * (2*PI/100);
    Point location = {player.getLocation().x + 400*cos(randAngle), 280.0 + r1 + r2, player.getLocation().z + 400*sin(randAngle)};
    Point lookingAt = {location.x, location.y, location.z - 10};
    RGBAcolor inputColor;
    if(r1 < 20)
    {
        inputColor = {0.8, 0.5, 0.5, 1};
    }
    else if(r1 < 40)
    {
        inputColor = {0.4, 0.8, 0.4, 1};
    }
    else if(r1 < 60)
    {
        inputColor = {1, 0.5, 0, 1}; // orange
    }
    else if(r1 < 80)
    {
        inputColor = {0.5, 0.2, 0.5, 1};
    }
    else
    {
        inputColor = {0.4, 0.8, 0.8, 1};
    }
    if(r2 > 50)
    {
        dirigibles.push_back(std::make_shared<Dirigible>(Dirigible(location, lookingAt, 0.3, {0,0,-1},
                                                                   90, 40, 40, inputColor,
                                                                   Blimp)));
    }
    else
    {
        dirigibles.push_back(std::make_shared<Dirigible>(Dirigible(location, lookingAt, 0.1, {0,0,-1},
                                                                   40, 40, 40, inputColor,
                                                                   Balloon)));
    }

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