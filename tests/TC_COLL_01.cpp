
#include "../src/SnakeManager.h"
#include "../src/CollisionManager.h"
#include <cassert>

int main()
{
    // Test 1: Snake inside the board.
    SnakeManager snake;
    assert(!CollisionManager::checkWallCollision(snake, 10, 10));

    // Test 2: Right boundary.
    for (int i = 0; i < 5; ++i)
    {
        snake.move();
    }
    assert(CollisionManager::checkWallCollision(snake, 10, 10));

    // Test 3: Invalid board dimensions.
    SnakeManager invalidBoardSnake;
    assert(CollisionManager::checkWallCollision(invalidBoardSnake, 0, 10));
    assert(CollisionManager::checkWallCollision(invalidBoardSnake, 10, 0));
    assert(CollisionManager::checkWallCollision(invalidBoardSnake, -1, 10));
    assert(CollisionManager::checkWallCollision(invalidBoardSnake, 10, -1));

    // Test 4: Left boundary.
    SnakeManager leftSnake;
    leftSnake.setDirection(Direction::Left);
    for (int i = 0; i < 6; ++i)
    {
        leftSnake.move();
    }
    assert(CollisionManager::checkWallCollision(leftSnake, 10, 10));

    // Test 5: Top boundary.
    SnakeManager topSnake;
    topSnake.setDirection(Direction::Up);
    for (int i = 0; i < 6; ++i)
    {
        topSnake.move();
    }
    assert(CollisionManager::checkWallCollision(topSnake, 10, 10));

    // Test 6: Bottom boundary.
    SnakeManager bottomSnake;
    bottomSnake.setDirection(Direction::Down);
    for (int i = 0; i < 5; ++i)
    {
        bottomSnake.move();
    }
    assert(CollisionManager::checkWallCollision(bottomSnake, 10, 10));

    return 0;
}
