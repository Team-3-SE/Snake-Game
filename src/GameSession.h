#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "CollisionManager.h"
#include "FoodManager.h"
#include "GameStateManager.h"
#include "InputHandler.h"
#include "ScoreManager.h"
#include "SnakeManager.h"

// Platform-independent game rules. GameController only feeds it input,
// calls tick() on a timer and draws the result.
class GameSession
{
private:
    int width;
    int height;
    GameStateManager state;
    SnakeManager snake;
    FoodManager foodManager;
    ScoreManager scoreManager;

    void reset();

public:
    GameSession(int boardWidth = 20, int boardHeight = 15);

    // Menu -> Playing.
    bool start();

    // Applies a command according to the current state. Unsupported
    // commands leave the session unchanged.
    void handle(InputCommand command);

    // Advances one step when Playing. Returns true if the snake moved.
    bool tick();

    GameState getState() const;
    bool isGameplayActive() const;
    int getScore() const;
    int getTickInterval() const;
    const SnakeManager& getSnake() const;
    Position getFood() const;
    int getWidth() const;
    int getHeight() const;
};

#endif
