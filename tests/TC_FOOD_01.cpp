#include "../src/FoodManager.h"
#include <cassert>

int main()
{
    FoodManager foodManager(20, 15);
    std::vector<Position> snakeBody = {
        {5, 5}, {4, 5}, {3, 5}
    };

    foodManager.generateFood(snakeBody);
    Position food = foodManager.getFood();

    assert(food.x >= 0 && food.x < 20);
    assert(food.y >= 0 && food.y < 15);

    for (const Position& segment : snakeBody)
    {
        assert(food.x != segment.x || food.y != segment.y);
    }

    return 0;
}