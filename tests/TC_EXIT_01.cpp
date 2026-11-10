#include "../src/InputHandler.h"
#include <cassert>

int main()
{
    // SG-F-015: Escape key must generate an Exit command.
    assert(InputHandler::parseInput(InputKey::Escape) ==
           InputCommand::Exit);

    // SG-F-015: Console Escape character must generate Exit.
    assert(InputHandler::parseInput(static_cast<char>(27)) ==
           InputCommand::Exit);

    // Unsupported input must not generate an Exit command.
    assert(InputHandler::parseInput(InputKey::Unknown) ==
           InputCommand::None);

    assert(InputHandler::parseInput('x') ==
           InputCommand::None);

    return 0;
}
