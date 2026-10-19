
#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <string>

enum class RenderGameState
{
    Menu,
    Playing,
    Paused,
    GameOver
};

class GameRenderer
{
public:
    // Return a readable name for the current state.
    static std::string stateName(RenderGameState state);

    // Return the controls relevant to the current state.
    static std::string controlsFor(RenderGameState state);

    // Build a complete textual status display.
    static std::string buildStatus(
        RenderGameState state,
        int score
    );

    // Clear the console and display the current status.
    static void renderConsole(
        RenderGameState state,
        int score
    );

    // Clear the console without displaying status.
    static void clearConsole();
};

#endif
