
#include "GameRenderer.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

std::string GameRenderer::stateName(RenderGameState state)
{
    switch (state)
    {
        case RenderGameState::Menu:    return "MENU";
        case RenderGameState::Playing: return "PLAYING";
        case RenderGameState::Paused:  return "PAUSED";
        case RenderGameState::GameOver:return "GAME OVER";
    }

    return "UNKNOWN";
}

std::string GameRenderer::controlsFor(RenderGameState state)
{
    switch (state)
    {
        case RenderGameState::Menu:
            return "Enter: Start | Esc: Exit";

        case RenderGameState::Playing:
            return "Arrow keys: Move | P: Pause | Esc: Exit";

        case RenderGameState::Paused:
            return "P: Resume | Esc: Exit";

        case RenderGameState::GameOver:
            return "R: Restart | Esc: Exit";
    }

    return {};
}

std::string GameRenderer::buildStatus(
    RenderGameState state,
    int score)
{
    std::ostringstream output;

    output << "============================\n"
           << "         SNAKE GAME         \n"
           << "============================\n"
           << "Game State: " << stateName(state) << '\n'
           << "Score: " << score << '\n'
           << "Controls: " << controlsFor(state) << '\n'
           << "============================\n";

    return output.str();
}

void GameRenderer::clearConsole()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void GameRenderer::renderConsole(
    RenderGameState state,
    int score)
{
    clearConsole();
    std::cout << buildStatus(state, score) << std::flush;
}
