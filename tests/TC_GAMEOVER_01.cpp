#include "../src/GameStateManager.h"
#include <cassert>

int main()
{
    GameStateManager state;

    assert(state.setState(GameState::Playing));
    assert(state.setState(GameState::GameOver));
    assert(state.getState() == GameState::GameOver);

    // Game Over can be exited by starting a new game.
    assert(state.setState(GameState::Playing));
    assert(state.getState() == GameState::Playing);

    return 0;
}
