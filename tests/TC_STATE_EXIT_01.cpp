#include "../src/GameStateManager.h"

#include <cassert>

int main()
{
    // Test exit directly from Menu.
    GameStateManager fromMenu;

    assert(fromMenu.getState() == GameState::Menu);
    assert(fromMenu.setState(GameState::Exiting));
    assert(fromMenu.getState() == GameState::Exiting);

    // Test exit from Playing.
    GameStateManager fromPlaying;

    assert(fromPlaying.setState(GameState::Playing));
    assert(fromPlaying.setState(GameState::Exiting));

    // Test exit from Paused.
    GameStateManager fromPaused;

    assert(fromPaused.setState(GameState::Playing));
    assert(fromPaused.setState(GameState::Paused));
    assert(fromPaused.setState(GameState::Exiting));

    // Test exit from GameOver.
    GameStateManager fromGameOver;

    assert(fromGameOver.setState(GameState::Playing));
    assert(fromGameOver.setState(GameState::GameOver));
    assert(fromGameOver.setState(GameState::Exiting));

    // Exiting must be terminal.
    assert(!fromMenu.setState(GameState::Playing));
    assert(!fromMenu.setState(GameState::Paused));
    assert(!fromMenu.setState(GameState::GameOver));

    // Gameplay must not be active after exiting.
    assert(!fromMenu.isGameplayActive());
    assert(!fromPlaying.isGameplayActive());
    assert(!fromPaused.isGameplayActive());
    assert(!fromGameOver.isGameplayActive());

    return 0;
}
