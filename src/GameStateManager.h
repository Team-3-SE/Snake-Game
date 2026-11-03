
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

enum class GameState
{
    Menu,
    Playing,
    Paused,
    GameOver
};

class GameStateManager
{
private:
    GameState currentState;

public:
    GameStateManager();

    GameState getState() const;
    bool setState(GameState newState);
};

#endif
