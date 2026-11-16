
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

    // Verify the previous game has different values.
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

    // Verify the initial snake length and direction.
    assert(snake.getBody().size() == 3);
    assert(snake.getDirection() == Direction::Right);

    // Verify the initial snake head position.
    Position head = snake.getHead();
    assert(head.x == 5);
    assert(head.y == 5);

    // Verify every snake segment has its exact initial position.
    const auto& body = snake.getBody();

    assert(body[0].x == 5 && body[0].y == 5);
    assert(body[1].x == 4 && body[1].y == 5);
    assert(body[2].x == 3 && body[2].y == 5);

    // Verify score and speed have reset.
    assert(score == 0);
    assert(DifficultyManager::getTickInterval(score) == 180);

    // Verify regenerated food is inside the board.
    Position p = food.getFood();

    assert(p.x >= 0 && p.x < 20);
    assert(p.y >= 0 && p.y < 15);

    // Food must not overlap any snake segment.
    for (const Position& segment : snake.getBody())
    {
        assert(p.x != segment.x || p.y != segment.y);
    }

    // Verify the final game state.
    assert(state.getState() == GameState::Playing);

    return 0;
}
