
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

enum class GameState
{
    Menu,
    Playing,
    Paused,
    GameOver,
    Exiting
};

class GameStateManager
{
private:
    GameState currentState;

public:
    GameStateManager();

    // Returns the current game state.
    GameState getState() const;

    // SG-F-013: Returns true only when gameplay updates are allowed.
    bool isGameplayActive() const;

    // Changes the current game state if the transition is valid.
    bool setState(GameState newState);
};

#endif
