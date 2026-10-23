
#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Test 1: Food at the snake's head should be detected.
    const Position foodAtHead = {5, 5};
    assert(CollisionManager::checkFoodCollision(snake, foodAtHead));

    // Test 2: Food at a different position should not be detected.
    const Position foodElsewhere = {7, 5};
    assert(!CollisionManager::checkFoodCollision(snake, foodElsewhere));

    // Test 3: Food at the new head position should be detected after movement.
    snake.move();

    const Position foodAtNewHead = {6, 5};
    assert(CollisionManager::checkFoodCollision(snake, foodAtNewHead));

    return 0;
}
