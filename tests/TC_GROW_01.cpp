#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Initial snake has 3 segments
    assert(snake.getBody().size() == 3);

    // Grow the snake
    snake.grow();

    // Snake should now have 4 segments
    assert(snake.getBody().size() == 4);

    return 0;
}