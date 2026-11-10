
#include "GameStateManager.h"

GameStateManager::GameStateManager()
    : currentState(GameState::Menu)
{
}

GameState GameStateManager::getState() const
{
    return currentState;
}

// SG-F-013: Check whether gameplay updates are allowed.
bool GameStateManager::isGameplayActive() const
{
    return currentState == GameState::Playing;
}

bool GameStateManager::setState(GameState newState)
{
    // Exiting is a terminal state.
    // No further state transitions are allowed after exit.
    if (currentState == GameState::Exiting)
    {
        return false;
    }

    // SG-F-015: Allow exit from any active game state.
    if (newState == GameState::Exiting)
    {
        currentState = GameState::Exiting;
        return true;
    }

    switch (currentState)
    {
        case GameState::Menu:
            if (newState == GameState::Playing)
            {
                currentState = newState;
                return true;
            }
            break;

        case GameState::Playing:
            if (newState == GameState::Paused ||
                newState == GameState::GameOver)
            {
                currentState = newState;
                return true;
            }
            break;

        case GameState::Paused:
            if (newState == GameState::Playing)
            {
                currentState = newState;
                return true;
            }
            break;

        case GameState::GameOver:
            if (newState == GameState::Playing)
            {
                currentState = newState;
                return true;
            }
            break;

        case GameState::Exiting:
            break;
    }

    return false;
}
