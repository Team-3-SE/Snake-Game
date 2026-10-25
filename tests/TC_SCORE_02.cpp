
#include "../src/ScoreManager.h"
#include "../src/GameRenderer.h"
#include <cassert>
#include <string>

int main()
{
    ScoreManager scoreManager;

    // Collect two food items.
    scoreManager.addFoodScore();
    scoreManager.addFoodScore();

    // Build the status text using the current score.
    const int score = scoreManager.getScore();

    const std::string status =
        GameRenderer::buildStatus(RenderGameState::Playing, score);

    // Verify the correct game state is displayed.
    assert(status.find("Game State: PLAYING") != std::string::npos);

    // Verify the exact score label and value are displayed.
    assert(status.find("Score: 20") != std::string::npos);

    return 0;
}
