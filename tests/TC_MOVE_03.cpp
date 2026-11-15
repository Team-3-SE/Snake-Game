
#include "../src/SnakeManager.h"
#include <cassert>

int main()
{
    SnakeManager snake;

    // Test 1: Initial direction and Right movement.
    assert(snake.getDirection() == Direction::Right);

    Position initialHead = snake.getHead();
    snake.move();

    Position newHead = snake.getHead();

    assert(newHead.x == initialHead.x + 1);
    assert(newHead.y == initialHead.y);

    // Test 2: Up movement.
    snake.setDirection(Direction::Up);
    assert(snake.getDirection() == Direction::Up);

    initialHead = snake.getHead();
    snake.move();

    newHead = snake.getHead();

    assert(newHead.x == initialHead.x);
    assert(newHead.y == initialHead.y - 1);

    // Test 3: Down movement.
    snake.setDirection(Direction::Down);
    assert(snake.getDirection() == Direction::Down);

    initialHead = snake.getHead();
    snake.move();

    newHead = snake.getHead();

    assert(newHead.x == initialHead.x);
    assert(newHead.y == initialHead.y + 1);

    // Test 4: Left movement.
    snake.setDirection(Direction::Left);
    assert(snake.getDirection() == Direction::Left);

    initialHead = snake.getHead();
    snake.move();

    newHead = snake.getHead();

    assert(newHead.x == initialHead.x - 1);
    assert(newHead.y == initialHead.y);
        // Test 5: Invalid direction value.
    const Direction invalidDirection =
        static_cast<Direction>(-1);

    snake.setDirection(invalidDirection);

    initialHead = snake.getHead();
    snake.move();
    newHead = snake.getHead();

    // An invalid direction does not change the head coordinates.
    assert(newHead.x == initialHead.x);
    assert(newHead.y == initialHead.y);

    // The direction value remains unchanged.
    assert(snake.getDirection() == invalidDirection);

    return 0;
}
