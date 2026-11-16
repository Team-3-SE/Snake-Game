/*
 * Test Case: TC-SEC-02   Requirement: SG-SR-002
 * Unrecognised input is ignored without affecting Game State.
 */
#include "../src/GameSession.h"
#include "check.h"

int main()
{
    CHECK(InputHandler::parseInput('x') == InputCommand::None);
    CHECK(InputHandler::parseInput('\0') == InputCommand::None);
    CHECK(InputHandler::parseInput(static_cast<char>(-1)) == InputCommand::None);
    CHECK(InputHandler::parseInput(InputKey::Unknown) == InputCommand::None);

    GameSession session;
    session.start();
    session.tick();
    const Position head = session.getSnake().getHead();
    const int score = session.getScore();

    session.handle(InputHandler::parseInput('x'));
    session.handle(InputHandler::parseInput(InputKey::Unknown));

    CHECK(session.getState() == GameState::Playing);
    CHECK(session.getScore() == score);
    CHECK(session.getSnake().getHead().x == head.x);
    CHECK(session.getSnake().getHead().y == head.y);
    CHECK(session.getSnake().getDirection() == Direction::Right);
    return finish("TC_SEC_02");
}
