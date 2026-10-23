
#include "GameController.h"

#include "FoodManager.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include "SnakeManager.h"

#include <iostream>
#include <vector>

namespace
{
    constexpr int BOARD_WIDTH = 20;
    constexpr int BOARD_HEIGHT = 15;
    constexpr int POINTS_PER_FOOD = 10;

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

    bool isMove(InputCommand command)
    {
        return command == InputCommand::MoveUp ||
               command == InputCommand::MoveDown ||
               command == InputCommand::MoveLeft ||
               command == InputCommand::MoveRight;
    }

    bool outsideBoard(Position p)
    {
        return p.x < 0 || p.x >= BOARD_WIDTH ||
               p.y < 0 || p.y >= BOARD_HEIGHT;
    }

    bool samePosition(Position a, Position b)
    {
        return a.x == b.x && a.y == b.y;
    }

    bool hitsBody(const SnakeManager& snake)
    {
        const std::vector<Position>& body = snake.getBody();
        const Position head = snake.getHead();

        for (std::size_t i = 1; i < body.size(); ++i)
        {
            if (samePosition(body[i], head))
                return true;
        }

        return false;
    }

    RenderGameState toRenderState(GameState state)
    {
        switch (state)
        {
            case GameState::Menu:    return RenderGameState::Menu;
            case GameState::Playing: return RenderGameState::Playing;
            case GameState::Paused:  return RenderGameState::Paused;
            case GameState::GameOver:return RenderGameState::GameOver;
        }

        return RenderGameState::Menu;
    }

    void drawBoard(const SnakeManager& snake, Position food, int score)
    {
        std::cout << "\nScore: " << score << "\n+";
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
                {
                    symbol = 'O';
                }
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
        std::cout << "Controls: W/A/S/D + Enter | P pause | "
                     "R restart after Game Over | Q quit\n";
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

    while (running)
    {
        GameRenderer::renderConsole(
            toRenderState(stateManager.getState()), score
        );
        drawBoard(snake, food.getFood(), score);

        const GameState currentState = stateManager.getState();

        if (currentState == GameState::GameOver)
            std::cout << "\nGAME OVER! Press R to restart or Q to quit.\n";

        std::cout << "\nCommand: ";

        char key;
        if (!(std::cin >> key))
        {
            running = false;
            break;
        }

        if (key == 'q' || key == 'Q')
        {
            running = false;
            continue;
        }

        const InputCommand command = InputHandler::parseInput(key);

        if (command == InputCommand::Restart)
        {
            if (stateManager.getState() == GameState::GameOver)
            {
                snake = SnakeManager();
                food = FoodManager(BOARD_WIDTH, BOARD_HEIGHT);
                score = 0;

                food.generateFood(snake.getBody());
                stateManager.setState(GameState::Playing);
            }
            else
            {
                std::cout << "\nRestart is available after Game Over.\n";
            }

            continue;
        }

        if (command == InputCommand::Pause)
        {
            if (stateManager.getState() == GameState::Playing)
                stateManager.setState(GameState::Paused);
            else if (stateManager.getState() == GameState::Paused)
                stateManager.setState(GameState::Playing);

            continue;
        }

        if (!isMove(command))
            continue;

        if (stateManager.getState() == GameState::GameOver ||
            stateManager.getState() == GameState::Paused)
            continue;

        if (stateManager.getState() == GameState::Menu)
        {
            if (!stateManager.setState(GameState::Playing))
                continue;
        }

        if (!InputHandler::applyDirection(snake, toDirection(command)))
            continue;

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
            }
        }
    }

    std::cout << "\nThanks for playing Snake!\n";
}
