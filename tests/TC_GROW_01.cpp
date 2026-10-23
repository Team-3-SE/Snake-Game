#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    assert(snake.getBody().size() == 3);

    Position originalTail = snake.getBody().back();

    snake.grow();

    assert(snake.getBody().size() == 4);

    Position newTail = snake.getBody().back();

    assert(newTail.x == originalTail.x);
    assert(newTail.y == originalTail.y);

    return 0;
}
