/*
 * Test Case: TC-SEC-03   Requirement: SG-SR-003
 * Unsupported input cannot directly modify Game State: commands that do
 * not apply in a state are ignored, and Exiting is terminal.
 */
#include "../src/GameSession.h"
#include "check.h"

int main()
{
    GameSession session; // Menu
    session.handle(InputCommand::Pause);
    session.handle(InputCommand::Restart);
    session.handle(InputCommand::MoveUp);
    CHECK(session.getState() == GameState::Menu);
    CHECK(!session.tick()); // no gameplay outside Playing

    session.start();
    session.handle(InputCommand::Restart); // not valid while Playing
    CHECK(session.getState() == GameState::Playing);

    session.handle(InputCommand::Pause);
    const Position head = session.getSnake().getHead();
    session.handle(InputCommand::MoveUp);
    CHECK(!session.tick());
    CHECK(session.getSnake().getHead().x == head.x);
    CHECK(session.getSnake().getDirection() == Direction::Right);

    session.handle(InputCommand::Exit);
    CHECK(session.getState() == GameState::Exiting);
    session.handle(InputCommand::Restart);
    session.handle(InputCommand::Pause);
    CHECK(session.getState() == GameState::Exiting);
    return finish("TC_SEC_03");
}
