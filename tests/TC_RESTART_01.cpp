#include "../src/GameStateManager.h"
#include "../src/SnakeManager.h"
#include "../src/FoodManager.h"
#include "../src/DifficultyManager.h"

#include <cassert>

int main()
{
    GameStateManager state;

    // Enter gameplay and simulate Game Over.
    assert(state.setState(GameState::Playing));
    assert(state.setState(GameState::GameOver));

    // Simulate a previous game with modified values.
    SnakeManager snake;
    snake.setDirection(Direction::Up);
    snake.move();
    snake.grow();

    FoodManager food(20, 15);
    food.generateFood(snake.getBody());

    int score = 50;

    // Verify that the previous game is not in its initial condition.
    assert(snake.getDirection() == Direction::Up);
    assert(snake.getBody().size() == 4);
    assert(score == 50);
    assert(DifficultyManager::getTickInterval(score) == 130);

    // SG-F-014: Simulate the restart operations in GameController.
    snake = SnakeManager();
    food = FoodManager(20, 15);
    score = 0;

    food.generateFood(snake.getBody());

    // Return the game to Playing state.
    assert(state.setState(GameState::Playing));

    // Verify the snake has returned to its initial configuration.
    assert(snake.getBody().size() == 3);
    assert(snake.getDirection() == Direction::Right);

    Position head = snake.getHead();
    assert(head.x == 5);
    assert(head.y == 5);

    // Verify score and speed have reset.
    assert(score == 0);
    assert(DifficultyManager::getTickInterval(score) == 180);

    // Verify the regenerated food is inside the board.
    Position p = food.getFood();

    assert(p.x >= 0 && p.x < 20);
    assert(p.y >= 0 && p.y < 15);

    // Food must not overlap any snake segment.
    for (const Position& segment : snake.getBody())
    {
        assert(p.x != segment.x || p.y != segment.y);
    }

    assert(state.getState() == GameState::Playing);

    return 0;
}
