
#include "GameStateManager.h"

GameStateManager::GameStateManager()
    : currentState(GameState::Menu)
{
}

GameState GameStateManager::getState() const
{
    return currentState;
}

bool GameStateManager::isGameplayActive() const
{
    return currentState == GameState::Playing;
}

// Checks valid transitions without modifying the current state.
bool GameStateManager::canTransitionTo(GameState newState) const
{
    if (currentState == GameState::Exiting)
    {
        return false;
    }

    // Exit is allowed from every non-terminal state.
    if (newState == GameState::Exiting)
    {
        return true;
    }

    switch (currentState)
    {
        case GameState::Menu:
            return newState == GameState::Playing;

        case GameState::Playing:
            return newState == GameState::Paused ||
                   newState == GameState::GameOver;

        case GameState::Paused:
            return newState == GameState::Playing;

        case GameState::GameOver:
            return newState == GameState::Playing;

        case GameState::Exiting:
            return false;
    }

    return false;
}

bool GameStateManager::setState(GameState newState)
{
    if (!canTransitionTo(newState))
    {
        return false;
    }

    currentState = newState;
    return true;
}
