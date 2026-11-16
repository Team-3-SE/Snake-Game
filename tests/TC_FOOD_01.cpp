
#include "../src/FoodManager.h"
#include <cassert>
#include <vector>

int main()
{
    FoodManager foodManager(20, 15);

    // Test 1: Food generation with a normal snake body.
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

    // Test 2: Food generation with an empty snake body.
    const std::vector<Position> emptySnake;
    foodManager.generateFood(emptySnake);

    food = foodManager.getFood();

    assert(food.x >= 0 && food.x < 20);
    assert(food.y >= 0 && food.y < 15);

    // Test 3: Food generation with one occupied position.
    const std::vector<Position> oneSegment = {
        {0, 0}
    };

    foodManager.generateFood(oneSegment);
    food = foodManager.getFood();

    assert(food.x >= 0 && food.x < 20);
    assert(food.y >= 0 && food.y < 15);
    assert(food.x != 0 || food.y != 0);

    // Test 4: Exercise occupied-position retry.
    // On a 1x2 board, one of the two positions is occupied.
    FoodManager smallFoodManager(1, 2);

    const std::vector<Position> almostFullSnake = {
        {0, 0}
    };

    for (int i = 0; i < 100; ++i)
    {
        smallFoodManager.generateFood(almostFullSnake);

        food = smallFoodManager.getFood();

        assert(food.x == 0);
        assert(food.y == 1);
    }

    return 0;
}
