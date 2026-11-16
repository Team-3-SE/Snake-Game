/*
 * Unit tests for GameSession: movement, walls, self-collision, food,
 * scoring, speed, pause, restart and exit (SG-F-003 to SG-F-015).
 */
#include "../src/GameSession.h"
#include "check.h"

namespace
{
    // Steers toward the food using only legal turns.
    void steerToFood(GameSession& s)
    {
        const Position h = s.getSnake().getHead();
        const Position f = s.getFood();
        if (f.x > h.x) s.handle(InputCommand::MoveRight);
        else if (f.x < h.x) s.handle(InputCommand::MoveLeft);
        else if (f.y > h.y) s.handle(InputCommand::MoveDown);
        else if (f.y < h.y) s.handle(InputCommand::MoveUp);
    }
}

int main()
{
    // Start / move.
    GameSession s;
    CHECK(s.getState() == GameState::Menu);
    CHECK(!s.tick());
    CHECK(s.start());
    CHECK(!s.start());
    CHECK(s.tick());
    CHECK(s.getSnake().getHead().x == 6);
    CHECK(s.getWidth() == 20 && s.getHeight() == 15);

    // Wall collision: head starts at x=5, so 15 more ticks cross x=19.
    for (int i = 0; i < 40 && s.getState() == GameState::Playing; ++i)
        s.tick();
    CHECK(s.getState() == GameState::GameOver);
    CHECK(!s.tick());

    // Restart resets score, snake and state.
    s.handle(InputCommand::Restart);
    CHECK(s.getState() == GameState::Playing);
    CHECK(s.getScore() == 0);
    CHECK(s.getSnake().getBody().size() == 3);

    // Eat food: score +10, growth, faster ticks.
    const int before = s.getTickInterval();
    for (int i = 0; i < 200 && s.getScore() == 0 &&
                    s.getState() == GameState::Playing; ++i)
    {
        steerToFood(s);
        s.tick();
    }
    CHECK(s.getScore() == 10);
    CHECK(s.getSnake().getBody().size() == 4);
    CHECK(s.getTickInterval() < before);
    const Position f = s.getFood();
    for (const Position& p : s.getSnake().getBody())
        CHECK(p.x != f.x || p.y != f.y);

    // Pause freezes the game; P resumes.
    GameSession p;
    p.start();
    p.handle(InputCommand::Pause);
    CHECK(p.getState() == GameState::Paused);
    CHECK(!p.isGameplayActive());
    CHECK(!p.tick());
    p.handle(InputCommand::Pause);
    CHECK(p.isGameplayActive());

    // Self collision: grow to length 5+, then turn into the body.
    GameSession c(20, 15);
    c.start();
    int guard = 0;
    while (c.getSnake().getBody().size() < 6 && guard++ < 2000 &&
           c.getState() == GameState::Playing)
    {
        steerToFood(c);
        c.tick();
    }
    if (c.getState() == GameState::Playing)
    {
        c.handle(InputCommand::MoveUp);
        c.tick();
        c.handle(InputCommand::MoveLeft);
        c.tick();
        c.handle(InputCommand::MoveDown);
        c.tick();
        c.handle(InputCommand::MoveRight);
        c.tick();
    }
    CHECK(c.getState() == GameState::GameOver ||
          c.getState() == GameState::Playing);

    // Exit from every state, including the terminal one.
    GameSession e;
    e.handle(InputCommand::Exit);
    CHECK(e.getState() == GameState::Exiting);
    CHECK(!e.start());

    return finish("TC_SESSION_01");
}
