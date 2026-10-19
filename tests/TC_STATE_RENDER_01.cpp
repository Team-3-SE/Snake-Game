#include "../src/GameRenderer.h"

#include <cassert>
#include <string>

int main()
{
    // SG-F-017 and SGNF-003: MENU state.
    const std::string menu =
        GameRenderer::buildStatus(RenderGameState::Menu, 0);

    assert(menu.find("Game State: MENU") != std::string::npos);
    assert(menu.find("Score: 0") != std::string::npos);
    assert(menu.find("Enter: Start") != std::string::npos);
    assert(menu.find("Esc: Exit") != std::string::npos);

    // SG-F-017 and SGNF-003: PLAYING state.
    const std::string playing =
        GameRenderer::buildStatus(RenderGameState::Playing, 50);

    assert(playing.find("Game State: PLAYING") != std::string::npos);
    assert(playing.find("Score: 50") != std::string::npos);
    assert(playing.find("Arrow keys: Move") != std::string::npos);
    assert(playing.find("P: Pause") != std::string::npos);
    assert(playing.find("Esc: Exit") != std::string::npos);

    // SG-F-017 and SGNF-003: PAUSED state.
    const std::string paused =
        GameRenderer::buildStatus(RenderGameState::Paused, 50);

    assert(paused.find("Game State: PAUSED") != std::string::npos);
    assert(paused.find("Score: 50") != std::string::npos);
    assert(paused.find("P: Resume") != std::string::npos);
    assert(paused.find("Esc: Exit") != std::string::npos);

    // SG-F-017 and SGNF-003: GAME OVER state.
    const std::string gameOver =
        GameRenderer::buildStatus(RenderGameState::GameOver, 50);

    assert(gameOver.find("Game State: GAME OVER") != std::string::npos);
    assert(gameOver.find("Score: 50") != std::string::npos);
    assert(gameOver.find("R: Restart") != std::string::npos);
    assert(gameOver.find("Esc: Exit") != std::string::npos);

    return 0;
}
