
#include "../src/InputHandler.h"

#include <cassert>

int main()
{
    
    // SG-F-002: directional keyboard commands
    assert(InputHandler::parseInput(InputKey::Up) ==
           InputCommand::MoveUp);
    assert(InputHandler::parseInput(InputKey::Down) ==
           InputCommand::MoveDown);
    assert(InputHandler::parseInput(InputKey::Left) ==
           InputCommand::MoveLeft);
    assert(InputHandler::parseInput(InputKey::Right) ==
           InputCommand::MoveRight);

    // SG-SR-002: unknown keyboard input is ignored
    assert(InputHandler::parseInput(InputKey::Unknown) ==
           InputCommand::None);

    // SG-F-002: recognized directional input
    assert(InputHandler::parseInput('w') == InputCommand::MoveUp);
    assert(InputHandler::parseInput('a') == InputCommand::MoveLeft);
    assert(InputHandler::parseInput('s') == InputCommand::MoveDown);
    assert(InputHandler::parseInput('d') == InputCommand::MoveRight);

    // SG-SR-002: unknown input is ignored
    assert(InputHandler::parseInput('x') == InputCommand::None);
    assert(InputHandler::parseInput('?') == InputCommand::None);

    // SG-F-004: reject immediate reversal
    assert(!InputHandler::isValidDirection(
        Direction::Right, Direction::Left));
    assert(!InputHandler::isValidDirection(
        Direction::Up, Direction::Down));

    // Valid turns are accepted
    assert(InputHandler::isValidDirection(
        Direction::Right, Direction::Up));
    assert(InputHandler::isValidDirection(
        Direction::Up, Direction::Left));

    // Verify applying a valid direction changes the snake.
    SnakeManager snake;
    assert(InputHandler::applyDirection(snake, Direction::Up));
    assert(snake.getDirection() == Direction::Up);

    // Invalid reversal must leave the direction unchanged.
    assert(!InputHandler::applyDirection(snake, Direction::Down));
    assert(snake.getDirection() == Direction::Up);

    return 0;
}
