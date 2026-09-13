#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Initial direction is Right
    assert(snake.getDirection() == Direction::Right);

    // Change direction to Up
    snake.setDirection(Direction::Up);

    // Verify direction changed
    assert(snake.getDirection() == Direction::Up);

    // Move one step
    Position initialHead = snake.getHead();
    snake.move();
    Position newHead = snake.getHead();

    // Snake should move one cell upward
    assert(newHead.x == initialHead.x);
    assert(newHead.y == initialHead.y - 1);

    return 0;
}