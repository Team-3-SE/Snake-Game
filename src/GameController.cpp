
#include "GameController.h"
#include "FoodManager.h"
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
    constexpr int POINTS_PER_FOOD = 10;
    constexpr int INITIAL_TICK_MS = 180;
    constexpr int MINIMUM_TICK_MS = 70;
    constexpr int SPEEDUP_PER_FOOD_MS = 10;

    bool samePosition(Position a, Position b)
    {
        return a.x == b.x && a.y == b.y;
    }

    bool outsideBoard(Position p)
    {
        return p.x < 0 || p.x >= BOARD_WIDTH ||
               p.y < 0 || p.y >= BOARD_HEIGHT;
    }

    bool hitsBody(const SnakeManager& snake)
    {
        const auto& body = snake.getBody();
        const Position head = snake.getHead();

        for (std::size_t i = 1; i < body.size(); ++i)
        {
            if (samePosition(body[i], head))
                return true;
        }
        return false;
    }

    bool isMove(InputCommand command)
    {
        return command == InputCommand::MoveUp ||
               command == InputCommand::MoveDown ||
               command == InputCommand::MoveLeft ||
               command == InputCommand::MoveRight;
    }

    Direction toDirection(InputCommand command)
    {
        switch (command)
        {
            case InputCommand::MoveUp:    return Direction::Up;
            case InputCommand::MoveDown:  return Direction::Down;
            case InputCommand::MoveLeft:  return Direction::Left;
            case InputCommand::MoveRight: return Direction::Right;
            default:                      return Direction::Right;
        }
    }

    RenderGameState toRenderState(GameState state)
    {
        switch (state)
        {
            case GameState::Menu:     return RenderGameState::Menu;
            case GameState::Playing:  return RenderGameState::Playing;
            case GameState::Paused:   return RenderGameState::Paused;
            case GameState::GameOver: return RenderGameState::GameOver;
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

                if (samePosition(cell, snake.getHead()))
                    symbol = 'O';
                else
                {
                    for (const Position& segment : snake.getBody())
                    {
                        if (samePosition(cell, segment))
                        {
                            symbol = 'o';
                            break;
                        }
                    }

                    if (samePosition(cell, food))
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

    InputCommand readArrowOrKey()
    {
        int key = _getch();

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

    void showScreen(const SnakeManager& snake, Position food,
                    GameState state, int score)
    {
        system("cls");
        GameRenderer::renderConsole(toRenderState(state), score);

        if (state == GameState::Playing || state == GameState::Paused)
            drawBoard(snake, food);
    }
}

GameController::GameController()
    : running(true)
{
}

void GameController::run()
{
    SnakeManager snake;
    FoodManager food(BOARD_WIDTH, BOARD_HEIGHT);
    int score = 0;

    food.generateFood(snake.getBody());

    auto lastTick = std::chrono::steady_clock::now();
    bool firstFrame = true;

    while (running)
    {
        GameState state = stateManager.getState();

        if (state == GameState::Menu)
        {
            system("cls");
            GameRenderer::renderConsole(RenderGameState::Menu, score);
            std::cout << "\nPress ENTER to start or ESC to exit.\n";

            int key = _getch();

            if (key == 27)
                running = false;
            else if (key == 13)
            {
                stateManager.setState(GameState::Playing);
                lastTick = std::chrono::steady_clock::now();
                firstFrame = true;
            }
            continue;
        }

        if (state == GameState::GameOver)
        {
            system("cls");
            GameRenderer::renderConsole(RenderGameState::GameOver, score);
            std::cout << "\nFinal score: " << score
                      << "\nPress R to restart or ESC to exit.\n";

            int key = _getch();

            if (key == 27)
                running = false;
            else if (key == 'r' || key == 'R')
            {
                snake = SnakeManager();
                food = FoodManager(BOARD_WIDTH, BOARD_HEIGHT);
                score = 0;
                food.generateFood(snake.getBody());
                stateManager.setState(GameState::Playing);
                lastTick = std::chrono::steady_clock::now();
                firstFrame = true;
            }
            continue;
        }

        if (state == GameState::Paused)
        {
            system("cls");
            GameRenderer::renderConsole(RenderGameState::Paused, score);
            std::cout << "\nPAUSED - Press P to resume or ESC to exit.\n";

            int key = _getch();

            if (key == 27)
                running = false;
            else if (key == 'p' || key == 'P')
            {
                stateManager.setState(GameState::Playing);
                lastTick = std::chrono::steady_clock::now();
                firstFrame = true;
            }
            continue;
        }

        if (firstFrame)
        {
            showScreen(snake, food.getFood(), GameState::Playing, score);
            firstFrame = false;
        }

        while (_kbhit())
        {
            InputCommand command = readArrowOrKey();

            if (command == InputCommand::Exit)
            {
                running = false;
                break;
            }

            if (command == InputCommand::Pause)
            {
                stateManager.setState(GameState::Paused);
                break;
            }

            if (isMove(command))
                InputHandler::applyDirection(snake, toDirection(command));
        }

        if (!running)
            break;

        if (stateManager.getState() != GameState::Playing)
            continue;

        const auto now = std::chrono::steady_clock::now();
        const int reduction =
            (score / POINTS_PER_FOOD) * SPEEDUP_PER_FOOD_MS;

        const int interval =
            (INITIAL_TICK_MS - reduction < MINIMUM_TICK_MS)
                ? MINIMUM_TICK_MS
                : INITIAL_TICK_MS - reduction;

        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastTick).count();

        if (elapsed >= interval)
        {
            lastTick = now;
            snake.move();

            if (outsideBoard(snake.getHead()) || hitsBody(snake))
            {
                stateManager.setState(GameState::GameOver);
                continue;
            }

            if (samePosition(snake.getHead(), food.getFood()))
            {
                snake.grow();
                score += POINTS_PER_FOOD;

                try
                {
                    food.generateFood(snake.getBody());
                }
                catch (const std::runtime_error&)
                {
                    stateManager.setState(GameState::GameOver);
                    continue;
                }
            }

            showScreen(snake, food.getFood(), GameState::Playing, score);
        }

        Sleep(5);
    }

    system("cls");
    std::cout << "Thanks for playing Snake!\n";
}
