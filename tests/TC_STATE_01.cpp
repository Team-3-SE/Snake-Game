#include "../src/GameStateManager.h"
#include <cassert>

int main()
{
    GameStateManager state;
    assert(state.getState() == GameState::Menu);

    // Valid transitions.
    assert(state.setState(GameState::Playing));
    assert(state.setState(GameState::Paused));
    assert(state.setState(GameState::Playing));
    assert(state.setState(GameState::GameOver));
    assert(state.setState(GameState::Playing));

    // Menu cannot be entered directly from Playing.
    assert(!state.setState(GameState::Menu));
    assert(state.getState() == GameState::Playing);

    return 0;
}
