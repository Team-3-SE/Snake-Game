#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Check initial snake has 3 segments
    assert(snake.getBody().size() == 3);

    // Check initial direction is Right
    assert(snake.getDirection() == Direction::Right);

    // Check initial head position
    Position head = snake.getHead();
    assert(head.x == 5);
    assert(head.y == 5);

    return 0;
}