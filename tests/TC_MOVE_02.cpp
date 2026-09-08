#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    Position initialHead = snake.getHead();

    // Move the snake one step
    snake.move();

    Position newHead = snake.getHead();

    // Snake should move one cell to the right
    assert(newHead.x == initialHead.x + 1);
    assert(newHead.y == initialHead.y);

    return 0;
}