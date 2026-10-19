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

    // SG-F-002: lowercase console input
    assert(InputHandler::parseInput('w') == InputCommand::MoveUp);
    assert(InputHandler::parseInput('a') == InputCommand::MoveLeft);
    assert(InputHandler::parseInput('s') == InputCommand::MoveDown);
    assert(InputHandler::parseInput('d') == InputCommand::MoveRight);

    // SG-F-002: uppercase console input
    assert(InputHandler::parseInput('W') == InputCommand::MoveUp);
    assert(InputHandler::parseInput('A') == InputCommand::MoveLeft);
    assert(InputHandler::parseInput('S') == InputCommand::MoveDown);
    assert(InputHandler::parseInput('D') == InputCommand::MoveRight);

    // SG-SR-002: unknown input is ignored
    assert(InputHandler::parseInput('x') == InputCommand::None);
    assert(InputHandler::parseInput('?') == InputCommand::None);

    // SG-F-004: reject immediate reversal in all four directions
    assert(!InputHandler::isValidDirection(
        Direction::Right, Direction::Left));
    assert(!InputHandler::isValidDirection(
        Direction::Left, Direction::Right));
    assert(!InputHandler::isValidDirection(
        Direction::Up, Direction::Down));
    assert(!InputHandler::isValidDirection(
        Direction::Down, Direction::Up));

    // SG-F-004: valid turns are accepted
    assert(InputHandler::isValidDirection(
        Direction::Right, Direction::Up));
    assert(InputHandler::isValidDirection(
        Direction::Right, Direction::Down));
    assert(InputHandler::isValidDirection(
        Direction::Left, Direction::Up));
    assert(InputHandler::isValidDirection(
        Direction::Left, Direction::Down));
    assert(InputHandler::isValidDirection(
        Direction::Up, Direction::Left));
    assert(InputHandler::isValidDirection(
        Direction::Up, Direction::Right));
    assert(InputHandler::isValidDirection(
        Direction::Down, Direction::Left));
    assert(InputHandler::isValidDirection(
        Direction::Down, Direction::Right));

    // Applying a valid direction changes the snake's direction.
    SnakeManager snake;
    assert(snake.getDirection() == Direction::Right);
    assert(InputHandler::applyDirection(snake, Direction::Up));
    assert(snake.getDirection() == Direction::Up);

    // An invalid reversal is rejected without changing direction.
    assert(!InputHandler::applyDirection(snake, Direction::Down));
    assert(snake.getDirection() == Direction::Up);

    // Unknown console input must not alter the snake's direction.
    InputCommand unknown = InputHandler::parseInput('x');
    assert(unknown == InputCommand::None);
    assert(snake.getDirection() == Direction::Up);

    // Verify control-key mappings.
    assert(InputHandler::parseInput(InputKey::P) ==
       InputCommand::Pause);
    assert(InputHandler::parseInput(InputKey::R) ==
       InputCommand::Restart);
    assert(InputHandler::parseInput(InputKey::Escape) ==
       InputCommand::Exit);

    // Verify console control-key mappings.
    assert(InputHandler::parseInput('p') == InputCommand::Pause);
    assert(InputHandler::parseInput('r') == InputCommand::Restart);
    assert(InputHandler::parseInput(static_cast<char>(27)) == InputCommand::Exit);
    
    return 0;
}
