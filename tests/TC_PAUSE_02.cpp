
#include "../src/GameStateManager.h"

#include <cassert>

int main()
{
    GameStateManager state;

    // Menu must not transition directly to Paused or GameOver.
    assert(state.getState() == GameState::Menu);
    assert(!state.setState(GameState::Paused));
    assert(!state.setState(GameState::GameOver));
    assert(state.getState() == GameState::Menu);
    assert(!state.isGameplayActive());

    // Start gameplay.
    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    // Playing must not transition directly back to Menu.
    assert(!state.setState(GameState::Menu));
    assert(state.getState() == GameState::Playing);

    // Pause gameplay.
    assert(state.setState(GameState::Paused));
    assert(state.getState() == GameState::Paused);
    assert(!state.isGameplayActive());

    // Paused must not transition directly to Menu or GameOver.
    assert(!state.setState(GameState::Menu));
    assert(!state.setState(GameState::GameOver));
    assert(state.getState() == GameState::Paused);
    assert(!state.isGameplayActive());

    // Resume gameplay.
    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    // Enter Game Over.
    assert(state.setState(GameState::GameOver));
    assert(state.getState() == GameState::GameOver);
    assert(!state.isGameplayActive());

    // Game Over must not transition directly to Paused or Menu.
    assert(!state.setState(GameState::Paused));
    assert(!state.setState(GameState::Menu));
    assert(state.getState() == GameState::GameOver);

    // Restart from Game Over is valid.
    assert(state.setState(GameState::Playing));
    assert(state.isGameplayActive());

    return 0;
}
