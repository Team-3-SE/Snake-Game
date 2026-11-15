
#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include <cassert>

int main()
{
    // Test 1: Initial snake should not collide with itself.
    SnakeManager snake;
    assert(!CollisionManager::checkSelfCollision(snake));

    // Test 2: Same X coordinate but different Y coordinate.
    // This checks that matching X alone is not a collision.
    snake.setDirection(Direction::Up);
    snake.move();

    assert(!CollisionManager::checkSelfCollision(snake));

    // Test 3: Moving backward into the body causes self-collision.
    SnakeManager collisionSnake;
    collisionSnake.setDirection(Direction::Left);
    collisionSnake.move();

    assert(CollisionManager::checkSelfCollision(collisionSnake));

    return 0;
}
