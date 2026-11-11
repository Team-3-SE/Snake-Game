
#include "GameController.h"
#include "GameSession.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include "SnakeManager.h"

#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <windows.h>

namespace
{
    constexpr int BOARD_WIDTH = 20;
    constexpr int BOARD_HEIGHT = 15;

    bool sameCell(Position a, Position b)
    {
        return a.x == b.x && a.y == b.y;
    }

    RenderGameState toRenderState(GameState state)
    {
        switch (state)
        {
            case GameState::Menu:     return RenderGameState::Menu;
            case GameState::Playing:  return RenderGameState::Playing;
            case GameState::Paused:   return RenderGameState::Paused;
            case GameState::GameOver: return RenderGameState::GameOver;
            case GameState::Exiting: break;
        }
        return RenderGameState::Menu;
    }

    void drawBoard(const SnakeManager& snake, Position food)
    {
        std::cout << "\n+";
        for (int x = 0; x < BOARD_WIDTH; ++x)
            std::cout << "--";
        std::cout << "+\n";

        for (int y = 0; y < BOARD_HEIGHT; ++y)
        {
            std::cout << "|";
            for (int x = 0; x < BOARD_WIDTH; ++x)
            {
                Position cell{x, y};
                char symbol = ' ';

                if (sameCell(cell, snake.getHead()))
                    symbol = 'O';
                else
                {
                    for (const Position& segment : snake.getBody())
                    {
                        if (sameCell(cell, segment))
                        {
                            symbol = 'o';
                            break;
                        }
                    }

                    if (sameCell(cell, food))
                        symbol = '*';
                }

                std::cout << ' ' << symbol;
            }
            std::cout << " |\n";
        }

        std::cout << "+";
        for (int x = 0; x < BOARD_WIDTH; ++x)
            std::cout << "--";
        std::cout << "+\n";
        std::cout << "Arrow keys/WASD: Move | P: Pause | Esc: Exit\n";
    }

    InputCommand readCommand(int key)
    {

        if (key == 0 || key == 224)
        {
            switch (_getch())
            {
                case 72: return InputHandler::parseInput(InputKey::Up);
                case 80: return InputHandler::parseInput(InputKey::Down);
                case 75: return InputHandler::parseInput(InputKey::Left);
                case 77: return InputHandler::parseInput(InputKey::Right);
                default: return InputCommand::None;
            }
        }

        return InputHandler::parseInput(static_cast<char>(key));
    }

    void showScreen(const GameSession& session)
    {
        system("cls");
        GameRenderer::renderConsole(RenderGameState::Playing,
                                    session.getScore());
        drawBoard(session.getSnake(), session.getFood());
    }
}

GameController::GameController() = default;

void GameController::run()
{
    GameSession session(BOARD_WIDTH, BOARD_HEIGHT);

    auto lastTick = std::chrono::steady_clock::now();
    bool needsRedraw = true;
    GameState shown = session.getState();

    while (session.getState() != GameState::Exiting)
    {
        const GameState state = session.getState();

        if (state != shown)
        {
            shown = state;
            lastTick = std::chrono::steady_clock::now();
            needsRedraw = true;
        }

        if (state != GameState::Playing)
        {
            if (needsRedraw)
            {
                system("cls");
                GameRenderer::renderConsole(toRenderState(state),
                                            session.getScore());
                if (state == GameState::Menu)
                    std::cout << "\nPress ENTER to start or ESC to exit.\n";
                else if (state == GameState::Paused)
                    std::cout << "\nPAUSED - Press P to resume or ESC to exit.\n";
                else
                    std::cout << "\nFinal score: " << session.getScore()
                              << "\nPress R to restart or ESC to exit.\n";
                needsRedraw = false;
            }

            const int key = _getch();
            if (state == GameState::Menu && key == 13)
                session.start();
            else
                session.handle(readCommand(key));
            continue;
        }

        if (needsRedraw)
        {
            showScreen(session);
            needsRedraw = false;
        }

        while (_kbhit() && session.getState() == GameState::Playing)
            session.handle(readCommand(_getch()));

        if (session.getState() != GameState::Playing)
            continue;

        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - lastTick).count();

        if (elapsed >= session.getTickInterval())
        {
            lastTick = std::chrono::steady_clock::now();
            session.tick();
            needsRedraw = true;
        }

        Sleep(5);
    }

    system("cls");
    std::cout << "Thanks for playing Snake!\n";
}
