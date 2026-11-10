#include "../src/GameStateManager.h"

#include <cassert>

int main()
{
    // Menu state.
    GameStateManager menu;

    assert(menu.canTransitionTo(GameState::Playing));
    assert(menu.canTransitionTo(GameState::Exiting));
    assert(!menu.canTransitionTo(GameState::Paused));
    assert(!menu.canTransitionTo(GameState::GameOver));
    assert(menu.getState() == GameState::Menu);

    // Playing state.
    GameStateManager playing;

    assert(playing.setState(GameState::Playing));

    assert(playing.canTransitionTo(GameState::Paused));
    assert(playing.canTransitionTo(GameState::GameOver));
    assert(playing.canTransitionTo(GameState::Exiting));
    assert(!playing.canTransitionTo(GameState::Menu));
    assert(playing.getState() == GameState::Playing);

    // Paused state.
    GameStateManager paused;

    assert(paused.setState(GameState::Playing));
    assert(paused.setState(GameState::Paused));

    assert(paused.canTransitionTo(GameState::Playing));
    assert(paused.canTransitionTo(GameState::Exiting));
    assert(!paused.canTransitionTo(GameState::Menu));
    assert(!paused.canTransitionTo(GameState::GameOver));
    assert(paused.getState() == GameState::Paused);

    // Game Over state.
    GameStateManager gameOver;

    assert(gameOver.setState(GameState::Playing));
    assert(gameOver.setState(GameState::GameOver));

    assert(gameOver.canTransitionTo(GameState::Playing));
    assert(gameOver.canTransitionTo(GameState::Exiting));
    assert(!gameOver.canTransitionTo(GameState::Menu));
    assert(!gameOver.canTransitionTo(GameState::Paused));
    assert(gameOver.getState() == GameState::GameOver);

    // Exiting must reject every further transition.
    GameStateManager exiting;

    assert(exiting.setState(GameState::Exiting));

    assert(!exiting.canTransitionTo(GameState::Menu));
    assert(!exiting.canTransitionTo(GameState::Playing));
    assert(!exiting.canTransitionTo(GameState::Paused));
    assert(!exiting.canTransitionTo(GameState::GameOver));
    assert(!exiting.canTransitionTo(GameState::Exiting));

    assert(exiting.getState() == GameState::Exiting);

    return 0;
}
