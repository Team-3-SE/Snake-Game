
#include "../src/GameRenderer.h"

#include <cassert>
#include <string>

int main()
{
    // SGNF-003: controls and status should be available for every game state.
    struct StateExpectation
    {
        RenderGameState state;
        const char* stateLabel;
        const char* expectedControls;
    };

    const StateExpectation cases[] = {
        {RenderGameState::Menu, "MENU", "Enter: Start | Esc: Exit"},
        {RenderGameState::Playing, "PLAYING",
         "Arrow keys: Move | P: Pause | Esc: Exit"},
        {RenderGameState::Paused, "PAUSED", "P: Resume | Esc: Exit"},
        {RenderGameState::GameOver, "GAME OVER", "R: Restart | Esc: Exit"}
    };

    for (const StateExpectation& testCase : cases)
    {
        const std::string name = GameRenderer::stateName(testCase.state);
        const std::string controls =
            GameRenderer::controlsFor(testCase.state);
        const std::string status =
            GameRenderer::buildStatus(testCase.state, 12);

        assert(name == testCase.stateLabel);
        assert(status.find(std::string("Game State: ") + testCase.stateLabel)
               != std::string::npos);

        assert(!controls.empty());
        assert(controls == testCase.expectedControls);
        assert(status.find(std::string("Controls: ") + testCase.expectedControls)
               != std::string::npos);

        assert(status.find("SNAKE GAME") != std::string::npos);
        assert(status.find("Score: 12") != std::string::npos);
    }

    // Test invalid game-state fallback behaviour.
    const RenderGameState invalidState =
        static_cast<RenderGameState>(-1);

    assert(GameRenderer::stateName(invalidState) == "UNKNOWN");

    assert(GameRenderer::controlsFor(invalidState).empty());

    const std::string invalidStatus =
        GameRenderer::buildStatus(invalidState, 25);

    assert(invalidStatus.find("Game State: UNKNOWN") != std::string::npos);
    assert(invalidStatus.find("Score: 25") != std::string::npos);
    assert(invalidStatus.find("Controls: \n") != std::string::npos);

    return 0;
}
