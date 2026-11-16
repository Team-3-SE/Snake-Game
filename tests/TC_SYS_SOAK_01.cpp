/*
 * System soak test: random input for many ticks. Invariants must hold
 * every step (SG-SR-002, SG-SR-005, SG-F-005/008/009/010).
 */
#include "../src/GameSession.h"
#include "check.h"

#include <random>

int main()
{
    std::mt19937 rng(12345);
    const InputCommand cmds[] = {
        InputCommand::None, InputCommand::MoveUp, InputCommand::MoveDown,
        InputCommand::MoveLeft, InputCommand::MoveRight, InputCommand::Pause,
        InputCommand::Restart};

    GameSession s(10, 8);
    s.start();

    for (int step = 0; step < 200000; ++step)
    {
        const InputCommand c = cmds[rng() % 7];
        s.handle(c);
        if (s.getState() == GameState::Playing && rng() % 4 != 0)
            s.tick();

        const auto& body = s.getSnake().getBody();
        CHECK(s.getState() != GameState::Exiting);
        CHECK(body.size() >= 3);
        CHECK(s.getScore() == 10 * static_cast<int>(body.size() - 3));
        CHECK(s.getTickInterval() >= 70 && s.getTickInterval() <= 180);

        if (s.getState() == GameState::Playing)
        {
            for (const Position& p : body)
            {
                CHECK(p.x >= 0 && p.x < 10 && p.y >= 0 && p.y < 8);
            }
            const Position f = s.getFood();
            CHECK(f.x >= 0 && f.x < 10 && f.y >= 0 && f.y < 8);
            // Distinct cells (a freshly grown tail may be duplicated once).
            for (std::size_t i = 0; i + 1 < body.size(); ++i)
                for (std::size_t j = i + 1; j + 1 < body.size(); ++j)
                    CHECK(body[i].x != body[j].x || body[i].y != body[j].y);
        }
        if (s.getState() == GameState::GameOver)
            s.handle(InputCommand::Restart);
    }
    return finish("TC_SYS_SOAK_01");
}
