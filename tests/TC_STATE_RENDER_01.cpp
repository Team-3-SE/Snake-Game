
#include "../src/GameRenderer.h"

#include <cassert>
#include <string>

int main()
{
    // SG-F-017: display the current game state.
    const std::string playing =
        GameRenderer::buildStatus(RenderGameState::Playing, 50);

    assert(playing.find("Game State: PLAYING") != std::string::npos);
    assert(playing.find("Score: 50") != std::string::npos);

    // SGNF-003: provide clear, state-relevant controls.
    assert(playing.find("Arrow keys: Move") != std::string::npos);
    assert(playing.find("P: Pause") != std::string::npos);

    const std::string paused =
        GameRenderer::buildStatus(RenderGameState::Paused, 50);

    assert(paused.find("Game State: PAUSED") != std::string::npos);
    assert(paused.find("P: Resume") != std::string::npos);

    const std::string gameOver =
        GameRenderer::buildStatus(RenderGameState::GameOver, 50);

    assert(gameOver.find("Game State: GAME OVER") != std::string::npos);
    assert(gameOver.find("R: Restart") != std::string::npos);

    return 0;
}
