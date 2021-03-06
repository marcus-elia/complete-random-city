#include "gameManager.h"

GameManager::GameManager()
{
    screenWidth = 1024;
    screenHeight = 512;
    currentStatus = Intro;
    showMouse = true;
    tickNumberMod100 = 0;
    png = PerlinNoiseGenerator(10, 10, 1);
    renderRadius = 3;
    updateCurrentChunks();
    initializeKeys();
    initializeButtons();
    makeInstructions();
}
GameManager::GameManager(int inputScreenWidth, int inputScreenHeight, int inputRenderRadius)
{
    screenWidth = inputScreenWidth;
    screenHeight = inputScreenHeight;
    currentStatus = Intro;
    showMouse = true;
    tickNumberMod100 = 0;
    renderRadius = inputRenderRadius;
    png = PerlinNoiseGenerator(PERLIN_SIZE, PERLIN_SIZE, 1);
    updateCurrentChunks();
    initializeKeys();
    initializeButtons();
    makeInstructions();
}

void GameManager::initializeButtons()
{
    playButton = Button(screenWidth/2, screenHeight/2, BUTTON_WIDTH, BUTTON_HEIGHT,
                        BUTTON_RADIUS, "Play", PLAY_BUTTON_COLOR, BUTTON_TEXT_COLOR, PLAY_BUTTON_COLOR_H);
    continueButton = Button(screenWidth/2, screenHeight/2, BUTTON_WIDTH, BUTTON_HEIGHT,
                            BUTTON_RADIUS, "Continue", PLAY_BUTTON_COLOR, BUTTON_TEXT_COLOR, PLAY_BUTTON_COLOR_H);
    quitButton = Button(screenWidth/2, screenHeight/2 - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT,
                        BUTTON_RADIUS, "Quit", QUIT_BUTTON_COLOR, BUTTON_TEXT_COLOR, QUIT_BUTTON_COLOR_H);
    hitboxButton = Button(screenWidth/2, screenHeight/2 - 2*BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT,
                          BUTTON_RADIUS, "Hitboxes", HITBOX_BUTTON_COLOR, BUTTON_TEXT_COLOR, HITBOX_BUTTON_COLOR_H);
    incRenderRadButton = Button(screenWidth/2 + BUTTON_WIDTH, screenHeight/2 - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT,
                                BUTTON_RADIUS, "View++", INCRAD_BUTTON_COLOR, BUTTON_TEXT_COLOR, INCRAD_BUTTON_COLOR_H);
    decRenderRadButton = Button(screenWidth/2 - BUTTON_WIDTH, screenHeight/2 - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT,
                                BUTTON_RADIUS, "View--", DECRAD_BUTTON_COLOR, BUTTON_TEXT_COLOR, DECRAD_BUTTON_COLOR_H);
}
void GameManager::initializeKeys()
{
    aKey = false, sKey = false, wKey = false, dKey = false, rKey = false, cKey = false, hyperSpeed = false;
}
void GameManager::makeInstructions()
{
    instructions.emplace_back("Use w,a,s,d to move and r,c to move up and down. Press p to pause.");
    instructions.emplace_back("Hold e to go fast.");
}
void GameManager::initializePlayer()
{
    player = Player({0, PLAYER_HEIGHT/2, 0}, {0, PLAYER_HEIGHT/2, -1}, {0, 1, 0}, PLAYER_SPEED, MOUSE_SENSITIVITY, CHUNK_SIZE);
}

// ===========================================
//
//               Mouse Events
//
// ===========================================
void GameManager::reactToMouseMovement(int mx, int my, double theta, double distance)
{
    if(currentStatus == Intro)
    {
        playButton.setIsHighlighted(playButton.containsPoint(mx, screenHeight - my));

        quitButton.setIsHighlighted(quitButton.containsPoint(mx, screenHeight - my));
    }
    else if(currentStatus == Playing)
    {
        double clampedDistance = fmin(distance, 50);
        player.updateAngles(theta, clampedDistance);
        player.updateSphericalDirectionBasedOnAngles();
        player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
    }
    else if(currentStatus == Paused)
    {
        continueButton.setIsHighlighted(continueButton.containsPoint(mx, screenHeight - my));

        quitButton.setIsHighlighted(quitButton.containsPoint(mx, screenHeight - my));

        hitboxButton.setIsHighlighted(hitboxButton.containsPoint(mx, screenHeight - my));

        incRenderRadButton.setIsHighlighted(incRenderRadButton.containsPoint(mx, screenHeight - my));

        decRenderRadButton.setIsHighlighted(decRenderRadButton.containsPoint(mx, screenHeight - my));
    }
}
void GameManager::reactToMouseClick(int mx, int my)
{
    if(currentStatus == Intro)
    {
        if(playButton.containsPoint(mx, screenHeight - my))
        {
            showMouse = false;
            currentStatus = Playing;
            resetGame();
        }
        else if(quitButton.containsPoint(mx, screenHeight - my))
        {
            closeWindow = true;
        }
    }
    else if(currentStatus == Playing)
    {
        std::shared_ptr<Chunk> playerChunk = allSeenChunks[player.getCurrentChunkInt()];
        if(playerChunk->hasAirport())
        {
            playerChunk->makeAirportCreatePlane();
        }
    }
    else if(currentStatus == Paused)
    {
        if(continueButton.containsPoint(mx, screenHeight - my))
        {
            showMouse = false;
            currentStatus = Playing;
        }
        else if(quitButton.containsPoint(mx,screenHeight -  my))
        {
            closeWindow = true;
        }
        else if(hitboxButton.containsPoint(mx, screenHeight - my))
        {
            showHitboxes = !showHitboxes;
        }
        else if(incRenderRadButton.containsPoint(mx, screenHeight - my))
        {
            updateRenderRadius(renderRadius + 1);
        }
        else if(decRenderRadButton.containsPoint(mx, screenHeight - my))
        {
            updateRenderRadius(renderRadius - 1);
        }
    }

}


void GameManager::draw() const
{
    if(currentStatus == Playing || currentStatus == Paused)
    {
        for(auto &c : currentChunks)
        {
            c->draw();
        }


        for(std::shared_ptr<Car> c : cars)
        {
            c->draw();
            if(showHitboxes)
            {
                c->drawHitbox();
            }
        }
        for(std::shared_ptr<Dirigible> d : dirigibles)
        {
            d->draw();
            if(showHitboxes)
            {
                d->drawHitbox();
            }
        }
    }
}

void GameManager::tick()
{
    if(currentStatus == Playing)
    {
        for(auto &c : currentChunks)
        {
            c->tick();
        }

        playerTick();
        carsTick();
        dirigiblesTick();

        tickNumberMod100++;
        if(tickNumberMod100 == 100)
        {
            tickNumberMod100 = 0;
        }
    }
}
void::GameManager::playerTick()
{
    // The player moves
    player.tick();

    if(hyperSpeed)
    {
        for(int i = 0; i < HYPER_SPEED_FACTOR; i++)
        {
            player.tick();
        }
    }

    // Check for the player hitting a building
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[point2DtoChunkID(curPlayerChunk)];
    player.checkCollisionsAndCorrect(*c, 5);

    // If the player is entering a different chunk
    if(curPlayerChunk != player.getCurrentChunkCoords())
    {
        updateCurrentChunks();
    }
}
void GameManager::carsTick()
{
    for(std::shared_ptr<Car> c : cars)
    {
        c->tick();
    }
    manageCars();
}
void GameManager::dirigiblesTick()
{
    for(std::shared_ptr<Dirigible> d : dirigibles)
    {
        d->tick();
    }
    if(tickNumberMod100 == 50)
    {
        manageDirigibles();
    }
}

// ===================================
//
//             Getters
//
// ==================================
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
GameStatus GameManager::getCurrentStatus() const
{
    return currentStatus;
}
bool GameManager::getCloseWindow() const
{
    return closeWindow;
}
bool GameManager::getShowMouse() const
{
    return showMouse;
}

// ===================================
//
//             Setters
//
// ==================================
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
void GameManager::setHyperSpeed(bool input)
{
    hyperSpeed = input;
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
    std::vector<Point2D> chunksInRadius = getChunkTopLeftCornersAroundPoint(player.getCurrentChunkCoords(), renderRadius);
    for(Point2D p : chunksInRadius)
    {
        int index = point2DtoChunkID(p);
        if(allSeenChunks.count(index) == 0) // if the chunk has never been seen before, make a new one
        {
            // Check for adjacent chunks to get edge road indices
            std::experimental::optional<std::shared_ptr<Chunk>> leftChunk, rightChunk, topChunk, bottomChunk;
            std::experimental::optional<std::vector<int>> inputLeftRoadIndices, inputRightRoadIndices,
                                                          inputTopRoadIndices, inputBottomRoadIndices;
            // Left border
            if(allSeenChunks.count(point2DtoChunkID({p.x - 1, p.z})) > 0)
            {
                leftChunk = allSeenChunks[point2DtoChunkID({p.x - 1, p.z})];
                inputLeftRoadIndices = leftChunk.value()->getRightRoadIndices();
            }
            else
            {
                leftChunk = std::experimental::nullopt;
                inputLeftRoadIndices = std::experimental::nullopt;
            }
            // Right border
            if(allSeenChunks.count(point2DtoChunkID({p.x + 1, p.z})) > 0)
            {
                rightChunk = allSeenChunks[point2DtoChunkID({p.x + 1, p.z})];
                inputRightRoadIndices = rightChunk.value()->getLeftRoadIndices();
            }
            else
            {
                rightChunk = std::experimental::nullopt;
                inputRightRoadIndices = std::experimental::nullopt;
            }
            // Top border
            if(allSeenChunks.count(point2DtoChunkID({p.x, p.z - 1})) > 0)
            {
                topChunk = allSeenChunks[point2DtoChunkID({p.x, p.z - 1})];
                inputTopRoadIndices = topChunk.value()->getBottomRoadIndices();
            }
            else
            {
                topChunk = std::experimental::nullopt;
                inputTopRoadIndices = std::experimental::nullopt;
            }
            // Bottom border
            if(allSeenChunks.count(point2DtoChunkID({p.x, p.z + 1})) > 0)
            {
                bottomChunk = allSeenChunks[point2DtoChunkID({p.x, p.z + 1})];
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
    int chunkIndex = point2DtoChunkID({(int)floor(p.x / CHUNK_SIZE), (int)floor(p.z / CHUNK_SIZE)});
    if(allSeenChunks.count(chunkIndex) == 0)
    {
        chunkIndex = 0; // If it's out of bounds, just check the 0 chunk for now. can't hurt.
    }
    return allSeenChunks[chunkIndex];
}

void GameManager::updateRenderRadius(int newRadius)
{
    if(newRadius > 0)
    {
        renderRadius = newRadius;
        updateCurrentChunks();
    }
}



// =============================================
//
//                Vehicles
//
// =============================================
void GameManager::manageCars()
{
    // Remove any cars too far away
    if(tickNumberMod100 % 50 == 0)
    {
        int L = cars.size();
        int i = 0;
        while(i < L)
        {
            std::shared_ptr<Vehicle> v = cars[i];
            if(distance2d(v->getLocation(), player.getLocation()) > 2*CHUNK_SIZE)
            {
                cars.erase(cars.begin() + i);
                L--;
                i--;
            }
            i++;
        }
    }


    // Check for collisions
    if(tickNumberMod100 % 10 == 0)
    {
        updateCarCollisions();
    }

    // Add a new car if needed
    while(cars.size() < MAX_NUM_VEHICLES)
    {
        createCar();
    }
}

bool GameManager::createCar()
{
    // Pointer to the player's current chunk
    Chunk* c = allSeenChunks[point2DtoChunkID(player.whatChunk())].get();
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
    cars.push_back(std::make_shared<Car>(Car(location, lookingAt, 2, {0,0,-1},
            25, 10, rp->getLaneWidth() - 4, inputColor,
            inputCarType, rp, HITBOX_GREEN)));
    return true;
}

void GameManager::updateCarCollisions()
{
    for(int i = 0; i < cars.size(); i++)
    {
        cars[i]->setHitboxColor(HITBOX_GREEN);
    }
    for(int i = 0; i < (int)cars.size()-1; i++)
    {
        std::shared_ptr<Car> c1 = cars[i];
        for(int j = i+1; j < cars.size(); j++)
        {
            std::shared_ptr<Car> c2 = cars[j];
            if(c1->correctCollision(c2->getFrontCollisionPoint(), 0) || c1->correctCollision(c2->getBackCollisionPoint(), 0))
            {
                c1->setHitboxColor(HITBOX_RED);
                c2->setHitboxColor(HITBOX_RED);
            }
        }
    }
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
                                                                   Blimp, HITBOX_GREEN)));
    }
    else
    {
        dirigibles.push_back(std::make_shared<Dirigible>(Dirigible(location, lookingAt, 0.1, {0,0,-1},
                                                                   40, 40, 40, inputColor,
                                                                   Balloon, HITBOX_GREEN)));
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


void GameManager::printPlayerBuildingDebug()
{
    Point v = player.getLocation();
    std::cout << std::endl<< "Player location: " << v.x << "," << v.y << "," << v.z << std::endl;
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[point2DtoChunkID(curPlayerChunk)];
}

void GameManager::resetGame()
{
    initializePlayer();
    currentStatus = Playing;
}
void GameManager::togglePaused()
{
    if(currentStatus == Paused)
    {
        currentStatus = Playing;
        showMouse = false;
    }
    else if(currentStatus == Playing)
    {
        currentStatus = Paused;
        showMouse = true;
    }
}

void GameManager::drawUI() const
{
    if(currentStatus == Intro)
    {
        playButton.draw();
        quitButton.draw();
        displayInstructions();
    }
    else if(currentStatus == Playing)
    {
        drawCursor();
    }
    else if(currentStatus == Paused)
    {
        continueButton.draw();
        quitButton.draw();
        hitboxButton.draw();
        incRenderRadButton.draw();
        decRenderRadButton.draw();
    }
    else if(currentStatus == End)
    {
        quitButton.draw();
    }
}
void GameManager::drawCursor() const
{
    setGLColor(CURSOR_COLOR);
    glBegin(GL_QUADS);    // Draw a + shape with two quads
    glVertex2f(screenWidth/2 - 5, screenHeight/2 + 2);
    glVertex2f(screenWidth/2 - 5, screenHeight/2 - 2);
    glVertex2f(screenWidth/2 + 5, screenHeight/2 - 2);
    glVertex2f(screenWidth/2 + 5, screenHeight/2 + 2);


    glVertex2f(screenWidth/2 - 2, screenHeight/2 + 5);
    glVertex2f(screenWidth/2 - 2, screenHeight/2 - 5);
    glVertex2f(screenWidth/2 + 2, screenHeight/2 - 5);
    glVertex2f(screenWidth/2 + 2, screenHeight/2 + 5);
    glEnd();
}
void GameManager::displayInstructions() const
{
    setGLColor(BLACK);
    for(int i = 0; i < instructions.size(); i++)
    {
        std::string s = instructions[i];
        glRasterPos2i(10, screenHeight - 15*i - 15);
        for(const char &letter : s)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
        }
    }
}