
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "SnakeManager.h"

enum class InputCommand
{
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pause,
    Restart,
    Exit
};

// Platform-independent representation of keyboard keys.
// The controller translates SFML events into these values.
enum class InputKey
{
    Up,
    Down,
    Left,
    Right,
    P,
    R,
    Escape,
    Unknown
};

class InputHandler
{
public:
    // For actual keyboard events translated by the controller.
    static InputCommand parseInput(InputKey key);

    // For console input.
    static InputCommand parseInput(char key);

    static bool isValidDirection(
        Direction currentDirection,
        Direction requestedDirection
    );

    static bool applyDirection(
        SnakeManager& snake,
        Direction requestedDirection
    );
};

#endif
