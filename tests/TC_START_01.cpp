#include "../src/GameStateManager.h"
#include <cassert>

int main()
{
    GameStateManager state;
    assert(state.getState() == GameState::Menu);
    assert(state.setState(GameState::Playing));
    assert(state.getState() == GameState::Playing);
    return 0;
}
