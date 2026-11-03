
#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Test 1: Initial snake should not collide with itself.
    assert(!CollisionManager::checkSelfCollision(snake));

    // Test 2: Force the snake to move backward into its own body.
    // This creates an overlapping head and body segment.
    snake.setDirection(Direction::Left);
    snake.move();

    assert(CollisionManager::checkSelfCollision(snake));

    return 0;
}
