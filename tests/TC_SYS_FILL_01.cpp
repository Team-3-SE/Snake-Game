/*
 * System test: a bot following a Hamiltonian cycle fills a 6x6 board.
 * Expected: 33 foods eaten, score 330, length 36, then GameOver (SG-F-005/008/010).
 */
#include "../src/GameSession.h"
#include "check.h"

#include <map>
#include <utility>
#include <vector>

int main()
{
    const int N = 6;
    std::vector<std::pair<int, int>> cycle;
    for (int x = 0; x < N; ++x) cycle.push_back({x, 0});
    for (int y = 1; y < N; ++y)
    {
        if (y % 2 == 1) for (int x = N - 1; x >= 1; --x) cycle.push_back({x, y});
        else            for (int x = 1; x < N; ++x)      cycle.push_back({x, y});
    }
    for (int y = N - 1; y >= 1; --y) cycle.push_back({0, y});
    CHECK(static_cast<int>(cycle.size()) == N * N);

    // Walk the cycle backwards so the initial body trails behind the head.
    std::map<std::pair<int, int>, std::pair<int, int>> next;
    for (std::size_t i = 0; i < cycle.size(); ++i)
        next[cycle[(i + 1) % cycle.size()]] = cycle[i];

    GameSession s(N, N);
    s.start();

    for (int t = 0; t < 5000 && s.getState() == GameState::Playing; ++t)
    {
        const Position h = s.getSnake().getHead();
        const auto n = next[{h.x, h.y}];
        if (n.first > h.x) s.handle(InputCommand::MoveRight);
        else if (n.first < h.x) s.handle(InputCommand::MoveLeft);
        else if (n.second > h.y) s.handle(InputCommand::MoveDown);
        else s.handle(InputCommand::MoveUp);
        s.tick();
    }

    CHECK(s.getState() == GameState::GameOver);
    CHECK(s.getScore() == 330);
    CHECK(s.getSnake().getBody().size() == 36);
    return finish("TC_SYS_FILL_01");
}
