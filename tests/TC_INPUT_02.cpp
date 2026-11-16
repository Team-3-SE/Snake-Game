/*
 * Regression: two direction changes inside one tick must not reverse the
 * snake into its own body (SG-F-004 / SG-SR-001).
 */
#include "../src/GameSession.h"
#include "check.h"

int main()
{
    GameSession session;
    session.start();

    // Heading Right: Up then Left in the same tick would otherwise reverse.
    session.handle(InputCommand::MoveUp);
    session.handle(InputCommand::MoveLeft);
    CHECK(session.getSnake().getDirection() == Direction::Up);
    session.tick();
    CHECK(session.getState() == GameState::Playing);

    // After a move the new heading is the reference, so Left is now legal.
    session.handle(InputCommand::MoveLeft);
    CHECK(session.getSnake().getDirection() == Direction::Left);
    session.tick();
    CHECK(session.getState() == GameState::Playing);

    // A turn followed by its opposite in one tick: last legal one wins.
    SnakeManager snake;
    CHECK(InputHandler::applyDirection(snake, Direction::Down));
    CHECK(!InputHandler::applyDirection(snake, Direction::Up));
    CHECK(!InputHandler::applyDirection(snake, Direction::Left));
    return finish("TC_INPUT_02");
}
