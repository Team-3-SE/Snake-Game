#include "../src/GameStateManager.h"
#include "../src/SnakeManager.h"
#include "../src/FoodManager.h"
#include <cassert>

int main()
{
    GameStateManager state;
    assert(state.setState(GameState::Playing));
    assert(state.setState(GameState::GameOver));

    // Model the reset operations performed by GameController on restart.
    SnakeManager snake;
    FoodManager food(20, 15);
    int score = 0;
    food.generateFood(snake.getBody());

    assert(score == 0);
    assert(snake.getBody().size() == 3);
    assert(snake.getDirection() == Direction::Right);

    Position p = food.getFood();
    assert(p.x >= 0 && p.x < 20);
    assert(p.y >= 0 && p.y < 15);

    for (const Position& segment : snake.getBody())
        assert(p.x != segment.x || p.y != segment.y);

    assert(state.setState(GameState::Playing));
    return 0;
}
