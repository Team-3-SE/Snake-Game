#include "../src/GameStateManager.h"
#include "../src/SnakeManager.h"
#include "../src/FoodManager.h"
#include <cassert>

int main()
{
    GameStateManager state;
    assert(state.getState() == GameState::Menu);

    SnakeManager snake;
    assert(snake.getBody().size() == 3);
    assert(snake.getDirection() == Direction::Right);

    FoodManager food(20, 15);
    food.generateFood(snake.getBody());
    Position p = food.getFood();

    assert(p.x >= 0 && p.x < 20);
    assert(p.y >= 0 && p.y < 15);

    for (const Position& segment : snake.getBody())
        assert(p.x != segment.x || p.y != segment.y);

    return 0;
}
