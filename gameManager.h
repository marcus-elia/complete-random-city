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
#include "button.h"

enum GameStatus {Intro, Playing, End, Paused};

class GameManager
{
private:
    Player player;

    // Controls
    bool wKey, aKey, sKey, dKey, rKey, cKey, hyperSpeed;

    // Chunks
    int renderRadius;
    std::unordered_map<int, std::shared_ptr<Chunk>> allSeenChunks;
    std::vector<std::shared_ptr<Chunk>> currentChunks;
    PerlinNoiseGenerator png;

    // Vehicles
    std::vector<std::shared_ptr<Car>> cars;
    int tickNumberMod100;
    std::vector<std::shared_ptr<Dirigible>> dirigibles;

    // Game management
    GameStatus currentStatus;
    bool showMouse = true;
    bool closeWindow = false;
    bool showHitboxes = false;

    // UI
    int screenWidth, screenHeight;
    Button playButton;
    Button quitButton;
    Button continueButton;
    Button hitboxButton;
    std::vector<std::string> instructions;

    // Game parameters
    int CHUNK_SIZE = 512;
    int PERLIN_SIZE = 10; // how many chunks before perlin repeats
    int PLOTS_PER_SIDE = 8;
    int MAX_NUM_VEHICLES = 15;
    int MAX_NUM_DIRIGIBLES = 4;
    int MAX_NUM_PLANES = 2;
    double PLAYER_HEIGHT = 10;
    double PLAYER_RADIUS = 3;
    double PLAYER_SPEED = 2;
    double MOUSE_SENSITIVITY = 0.005;
    int HYPER_SPEED_FACTOR = 6;
    int BUTTON_WIDTH = 128;
    int BUTTON_HEIGHT = 64;
    int BUTTON_RADIUS = 16;
    RGBAcolor PLAY_BUTTON_COLOR = {0.0, 0.0, 0.7, 1.0};   // Slightly Dark Blue
    RGBAcolor QUIT_BUTTON_COLOR = {0.7, 0.0, 0.0, 1.0};   // Slightly Dark Red
    RGBAcolor HITBOX_BUTTON_COLOR = {0.9, 0.6, 0.1, 1.0};   // Orange
    RGBAcolor PLAY_BUTTON_COLOR_H = {0.0, 0.2, 1.0, 1.0}; // Lighter Blue
    RGBAcolor QUIT_BUTTON_COLOR_H = {1.0, 0.2, 0.0, 1.0}; // Lighter Red
    RGBAcolor HITBOX_BUTTON_COLOR_H = {1.0, 0.7, 0.2, 1.0}; // Lighter Orange
    RGBAcolor BUTTON_TEXT_COLOR = {1.0,1.0,1.0,1.0};      // White
    RGBAcolor CURSOR_COLOR = {0.3, 0.3, 0.3, 1.0};        // Dark Gray
    RGBAcolor BLACK = {0.0, 0.0, 0.0, 1.0};

public:
    GameManager();
    GameManager(int inputScreenWidth, int inputScreenHeight, int inputRenderRadius);

    // Helper functions for the constructor
    void initializeButtons();
    void makeInstructions();
    void initializeKeys(); // All keys should initially be false.
    void initializePlayer();

    // Getters
    Player getPlayer() const;
    bool getWKey();
    bool getAKey();
    bool getSKey();
    bool getDKey();
    bool getRKey();
    bool getCKey();
    GameStatus getCurrentStatus() const;
    bool getCloseWindow() const;
    bool getShowMouse() const;

    // Setters
    void setWKey(bool input);
    void setAKey(bool input);
    void setSKey(bool input);
    void setDKey(bool input);
    void setRKey(bool input);
    void setCKey(bool input);
    void setHyperSpeed(bool input);

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
    void reactToMouseMovement(int mx, int my, double theta, double distance);
    void reactToMouseClick(int mx, int my);

    void draw() const;
    void tick();
    void playerTick();
    void carsTick();
    void dirigiblesTick();

    // Debug
    void printPlayerBuildingDebug();

    // Game Management
    void togglePaused();
    void resetGame();

    // UI
    void drawUI() const;
    void drawCursor() const;
    void displayInstructions() const;
};

#endif //RANDOM_3D_CITY_GAMEMANAGER_H
