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
    }

    return false;
}
