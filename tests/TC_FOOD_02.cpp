#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include "../src/FoodManager.h"

#include <cassert>
#include <stdexcept>
#include <vector>

int main()
{
    // Tests for SG-F-006: Food collision detection.
    SnakeManager snake;

    const Position foodAtHead = {5, 5};
    assert(CollisionManager::checkFoodCollision(snake, foodAtHead));

    const Position foodElsewhere = {7, 5};
    assert(!CollisionManager::checkFoodCollision(snake, foodElsewhere));

    snake.move();

    const Position foodAtNewHead = {6, 5};
    assert(CollisionManager::checkFoodCollision(snake, foodAtNewHead));

    // Test that food generation fails when the board is full.
    FoodManager foodManager(2, 2);

    const std::vector<Position> fullSnake = {
        {0, 0}, {1, 0}, {0, 1}, {1, 1}
    };

    bool exceptionThrown = false;

    try
    {
        foodManager.generateFood(fullSnake);
    }
    catch (const std::runtime_error&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    return 0;
}