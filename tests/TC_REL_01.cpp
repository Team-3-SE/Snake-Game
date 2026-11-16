
#include "../src/GameStateManager.h"
#include <cassert>
#include <iostream>

int main()
{
    GameStateManager manager;

    // Initial state
    assert(manager.getState() == GameState::Menu);

    // Start game
    assert(manager.setState(GameState::Playing));
    assert(manager.getState() == GameState::Playing);

    // Pause game
    assert(manager.setState(GameState::Paused));
    assert(manager.getState() == GameState::Paused);

    // Resume game
    assert(manager.setState(GameState::Playing));
    assert(manager.getState() == GameState::Playing);

    // Game over
    assert(manager.setState(GameState::GameOver));
    assert(manager.getState() == GameState::GameOver);

    // Restart game
    assert(manager.setState(GameState::Playing));
    assert(manager.getState() == GameState::Playing);

    // Verify valid transition: Playing -> Paused
    assert(manager.setState(GameState::Paused));
    assert(manager.getState() == GameState::Paused);

    // Verify invalid transition: Paused -> GameOver
    assert(!manager.setState(GameState::GameOver));
    assert(manager.getState() == GameState::Paused);

    // Verify invalid transition: Paused -> Menu
    assert(!manager.setState(GameState::Menu));
    assert(manager.getState() == GameState::Paused);

    std::cout << "TC-REL-01 PASSED" << std::endl;

    return 0;
}
