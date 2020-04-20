#ifndef RANDOM_3D_CITY_GAMEMANAGER_H
#define RANDOM_3D_CITY_GAMEMANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
//#include "recPrism.h"
#include "player.h"
#include "perlinNoiseGenerator.h"
#include "car.h"
#include "dirigible.h"

class GameManager
{
private:
    Player player;

    // Controls
    bool wKey, aKey, sKey, dKey, rKey, cKey;

    // Chunks
    int chunkSize;
    int renderRadius;
    std::unordered_map<int, std::shared_ptr<Chunk>> allSeenChunks;
    std::vector<std::shared_ptr<Chunk>> currentChunks;
    int perlinSize;  // how many chunks before perlin repeats
    PerlinNoiseGenerator png;

    // Vehicles
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    int maxNumVehicles;
    int tickNumberMod100;
    std::vector<std::shared_ptr<Dirigible>> dirigibles;
    int maxNumDirigibles;
public:
    GameManager();
    GameManager(int inputChunkSize, int inputRenderRadius, int inputPerlinSize);

    // Getters
    Player getPlayer() const;
    bool getWKey();
    bool getAKey();
    bool getSKey();
    bool getDKey();
    bool getRKey();
    bool getCKey();

    // Setters
    void setWKey(bool input);
    void setAKey(bool input);
    void setSKey(bool input);
    void setDKey(bool input);
    void setRKey(bool input);
    void setCKey(bool input);

    // Chunks
    double getPerlinValue(Point2D p); // the value of the Perlin noise map at that point
    void updateCurrentChunks();
    // Returns a pointer to the chunk that p is in
    std::shared_ptr<Chunk> pointToChunk(Point p);

    // Vehicles
    void manageCars();
    // Randomly spawns a car in the chunk the player is currently in
    // Returns true if successful
    bool createCar();

    void manageDirigibles();
    bool createDirigible();

    // Camera
    Point getCameraLocation() const;
    Point getCameraLookingAt() const;
    Point getCameraUp() const;

    // Mouse
    void reactToMouseMovement(double theta);
    void reactToMouseClick();

    void draw() const;
    void tick();

    // Debug
    void printPlayerBuildingDebug();
};

// Since % can return negatives
int mod(int a, int m);

#endif //RANDOM_3D_CITY_GAMEMANAGER_H
