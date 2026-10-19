
#include "InputHandler.h"

#include <cctype>

InputCommand InputHandler::parseInput(InputKey key)
{
    switch (key)
    {
        case InputKey::Up:     return InputCommand::MoveUp;
        case InputKey::Down:   return InputCommand::MoveDown;
        case InputKey::Left:   return InputCommand::MoveLeft;
        case InputKey::Right:  return InputCommand::MoveRight;
        case InputKey::P:      return InputCommand::Pause;
        case InputKey::R:      return InputCommand::Restart;
        case InputKey::Escape: return InputCommand::Exit;
        case InputKey::Unknown:
        default:               return InputCommand::None;
    }
}

InputCommand InputHandler::parseInput(char key)
{
    switch (std::tolower(static_cast<unsigned char>(key)))
    {
        case 'w': return InputCommand::MoveUp;
        case 's': return InputCommand::MoveDown;
        case 'a': return InputCommand::MoveLeft;
        case 'd': return InputCommand::MoveRight;
        case 'p': return InputCommand::Pause;
        case 'r': return InputCommand::Restart;
        case 27:  return InputCommand::Exit;
        default:  return InputCommand::None;
    }
}

bool InputHandler::isValidDirection(
    Direction currentDirection,
    Direction requestedDirection)
{
    switch (currentDirection)
    {
        case Direction::Up:
            return requestedDirection != Direction::Down;

        case Direction::Down:
            return requestedDirection != Direction::Up;

        case Direction::Left:
            return requestedDirection != Direction::Right;

        case Direction::Right:
            return requestedDirection != Direction::Left;
    }

    return false;
}

bool InputHandler::applyDirection(
    SnakeManager& snake,
    Direction requestedDirection)
{
    if (!isValidDirection(snake.getDirection(), requestedDirection))
        return false;

    snake.setDirection(requestedDirection);
    return true;
}
