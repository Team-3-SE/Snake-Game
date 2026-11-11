#include "GameSession.h"
#include "DifficultyManager.h"

#include <stdexcept>

GameSession::GameSession(int boardWidth, int boardHeight)
    : width(boardWidth), height(boardHeight),
      foodManager(boardWidth, boardHeight)
{
    foodManager.generateFood(snake.getBody());
}

void GameSession::reset()
{
    snake = SnakeManager();
    scoreManager.reset();
    foodManager.generateFood(snake.getBody());
}

bool GameSession::start()
{
    return state.setState(GameState::Playing);
}

void GameSession::handle(InputCommand command)
{
    if (command == InputCommand::Exit)
    {
        state.setState(GameState::Exiting);
        return;
    }

    switch (state.getState())
    {
        case GameState::Playing:
            if (command == InputCommand::Pause)
                state.setState(GameState::Paused);
            else if (command == InputCommand::MoveUp)
                InputHandler::applyDirection(snake, Direction::Up);
            else if (command == InputCommand::MoveDown)
                InputHandler::applyDirection(snake, Direction::Down);
            else if (command == InputCommand::MoveLeft)
                InputHandler::applyDirection(snake, Direction::Left);
            else if (command == InputCommand::MoveRight)
                InputHandler::applyDirection(snake, Direction::Right);
            break;

        case GameState::Paused:
            if (command == InputCommand::Pause)
                state.setState(GameState::Playing);
            break;

        case GameState::GameOver:
            if (command == InputCommand::Restart)
            {
                reset();
                state.setState(GameState::Playing);
            }
            break;

        case GameState::Menu:
        case GameState::Exiting:
            break;
    }
}

bool GameSession::tick()
{
    if (!state.isGameplayActive())
        return false;

    snake.move();

    if (CollisionManager::checkWallCollision(snake, width, height) ||
        CollisionManager::checkSelfCollision(snake))
    {
        state.setState(GameState::GameOver);
        return true;
    }

    if (CollisionManager::checkFoodCollision(snake, foodManager.getFood()))
    {
        snake.grow();
        scoreManager.addFoodScore();

        try
        {
            foodManager.generateFood(snake.getBody());
        }
        catch (const std::runtime_error&)
        {
            state.setState(GameState::GameOver); // board full
        }
    }
    return true;
}

GameState GameSession::getState() const { return state.getState(); }
bool GameSession::isGameplayActive() const { return state.isGameplayActive(); }
int GameSession::getScore() const { return scoreManager.getScore(); }
int GameSession::getTickInterval() const
{
    return DifficultyManager::getTickInterval(scoreManager.getScore());
}
const SnakeManager& GameSession::getSnake() const { return snake; }
Position GameSession::getFood() const { return foodManager.getFood(); }
int GameSession::getWidth() const { return width; }
int GameSession::getHeight() const { return height; }
