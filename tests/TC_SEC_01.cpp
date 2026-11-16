/*
 * Test Case: TC-SEC-01   Requirement: SG-SR-001
 * Player input is validated before it is processed: only supported keys
 * map to commands, and a reversal is rejected before movement.
 */
#include "../src/GameSession.h"
#include "check.h"

int main()
{
    CHECK(InputHandler::parseInput('w') == InputCommand::MoveUp);
    CHECK(InputHandler::parseInput('S') == InputCommand::MoveDown);
    CHECK(InputHandler::parseInput('a') == InputCommand::MoveLeft);
    CHECK(InputHandler::parseInput('d') == InputCommand::MoveRight);
    CHECK(InputHandler::parseInput('p') == InputCommand::Pause);
    CHECK(InputHandler::parseInput('r') == InputCommand::Restart);
    CHECK(InputHandler::parseInput(InputKey::Escape) == InputCommand::Exit);

    SnakeManager snake; // heading Right
    CHECK(!InputHandler::applyDirection(snake, Direction::Left));
    CHECK(snake.getDirection() == Direction::Right);
    CHECK(InputHandler::applyDirection(snake, Direction::Up));
    return finish("TC_SEC_01");
}
