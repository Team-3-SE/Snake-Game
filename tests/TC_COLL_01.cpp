
#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Test 1: Snake inside the board should not collide with a wall.
    assert(!CollisionManager::checkWallCollision(snake, 10, 10));

    // Test 2: Move the snake until its head crosses the right boundary.
    for (int i = 0; i < 5; ++i)
    {
        snake.move();
    }

    assert(CollisionManager::checkWallCollision(snake, 10, 10));

    // Test 3: Invalid board dimensions should be treated as a collision.
    assert(CollisionManager::checkWallCollision(snake, 0, 10));
    assert(CollisionManager::checkWallCollision(snake, 10, 0));

    return 0;
}
