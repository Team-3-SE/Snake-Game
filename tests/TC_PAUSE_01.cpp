#include "../src/GameStateManager.h"
#include <cassert>

int main()
{
    GameStateManager state;

    // Initially, the game is in Menu.
    assert(state.getState() == GameState::Menu);
    assert(!state.isGameplayActive());

    // Start gameplay.
    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    // Pause gameplay.
    assert(state.setState(GameState::Paused));
    assert(!state.isGameplayActive());

    // Resume gameplay.
    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    // Verify repeated pause and resume operations.
    assert(state.setState(GameState::Paused));
    assert(!state.isGameplayActive());

    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    // Game Over must not be treated as active gameplay.
    assert(state.setState(GameState::GameOver));
    assert(!state.isGameplayActive());

    return 0;
}
