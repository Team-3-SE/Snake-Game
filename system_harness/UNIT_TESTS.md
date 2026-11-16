# Unit Test Specification & Results — Snake Game

| | |
|---|---|
| **Level** | Unit and module-integration tests (Test Plan §5) |
| **Framework** | Plain C++17 `assert`, one executable per test (`tests/TC_*.cpp`); exit code 0 = pass |
| **Runner** | `tests/run_sprint2_tests.ps1` (Windows, MinGW GCC); CI: `.github/workflows/ci.yml` |
| **Build flags** | `-std=c++17 -Wall -Wextra -pedantic -O0 -g --coverage` |
| **Executed for this report** | GCC 13.3 / Ubuntu 24.04, 2026-11-16 |
| **Naming** | Files use `TC_FUNC_NN`; SRS/RTM uses `TC-FUNC-NN` (same IDs) |

---

## 1. Result summary

| Item | Count |
|---|---|
| Test source files in `tests/` | 33 |
| Registered in the runner | 29 |
| **Executed on Linux — passed** | **28 / 28**, 0 compiler warnings |
| `TC_PORT_01` | Windows-only by design; prints `FAIL: Unsupported platform.` on Linux (verified), passes on the Windows CI |
| `TC_SEC_01`, `TC_SEC_02`, `TC_SEC_03` | **Empty files (0 bytes)**, not in the runner — see §4 |
| `TC_SEC_04` | Comment-only manual inspection record, not executable |
| Assertions failing | 0 |

`assert` is compiled in (no `-DNDEBUG`); a build with `NDEBUG` would make every test pass vacuously, so keep it that way.

## 2. Test inventory by module

Status column = result of this run.

### 2.1 SnakeManager (SG-F-002/003/004/007)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_MOVE_01 | SG-F-002 (per RTM) | Initial snake: 3 segments, direction Right, head (5,5) | Pass |
| TC_MOVE_02 | SG-F-003 | One `move()` advances head +1 in x, y unchanged | Pass |
| TC_MOVE_03 | SG-F-004 (per RTM) | Up / Down / Left / Right moves each shift head by exactly one cell; `setDirection`/`getDirection` round-trip; invalid enum value leaves head unchanged | Pass |
| TC_GROW_01 | SG-F-007 | `grow()` raises length 3 → 4 and the new tail duplicates the old tail position | Pass |

### 2.2 InputHandler (SG-F-002/004/015, SG-SR-001/002)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_INPUT_01 | SG-F-002, SG-F-004, SG-SR-001, SG-SR-002 | `InputKey` → command for all keys; `char` → command for `w a s d p r` in both cases and `27`; unknown (`x`, `?`, `Unknown`) → `None`; all 4 reversals rejected, all 8 valid turns accepted; `applyDirection` changes/doesn't change snake; invalid-enum direction rejected | Pass |
| TC_EXIT_01 | SG-F-015 | `Escape` and char 27 → `Exit`; unknown → `None` | Pass |
| TC_PERF_01 | SGNF-001 | 100 000 iterations of parse + validate average far below 100 ms (function-level only; not keyboard-to-screen latency) | Pass |

### 2.3 FoodManager (SG-F-005)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_FOOD_01 | SG-F-005 | Food in bounds and not on snake; empty snake OK; single occupied cell avoided; 1×2 board with one cell occupied always yields the free cell over 100 trials | Pass |
| TC_FOOD_03 | SG-F-005 (robustness) | Constructor throws `std::invalid_argument` for width 0 and height −5 | Pass |

### 2.4 CollisionManager (SG-F-006/009/010)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_FOOD_02 | SG-F-006 | `checkFoodCollision` true at head, false elsewhere, false for same x / different y; `generateFood` on a full 2×2 board throws `std::runtime_error` | Pass |
| TC_COLL_01 | SG-F-009 | No wall hit inside 10×10; hit on right, left, top, bottom; invalid board sizes (0, −1) report collision | Pass |
| TC_COLL_02 | SG-F-010 | Fresh snake no self-collision; same-x-different-y is not a collision; reversing into the neck is | Pass |

### 2.5 ScoreManager (SG-F-008/016)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_SCORE_01 | SG-F-008 | Starts 0; +10 per `addFoodScore` (10, 20, 30) | Pass |
| TC_SCORE_02 | SG-F-016 | Score 20 appears as `Score: 20` and `Game State: PLAYING` in the status text | Pass |

### 2.6 DifficultyManager (SG-F-012)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_SPEED_01 | SG-F-012 | Interval 180 ms at 0 and 9; 170 at 10; 130 at 50; 80 at 100; floor 70 at 110 and 120; negative score → 180 | Pass |

### 2.7 GameStateManager (SG-F-001/011/013/014/015/017, SGNF-002)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_START_01 | SG-F-001 | Menu → Playing | Pass |
| TC_PAUSE_01 | SG-F-013 | Pause, resume, repeat; `isGameplayActive()` true only in Playing | Pass |
| TC_PAUSE_02 | SG-F-013 | Illegal: Menu→Paused/GameOver, Playing→Menu, Paused→Menu/GameOver, GameOver→Paused/Menu; state unchanged after refusal | Pass |
| TC_GAMEOVER_01 | SG-F-011 | Playing → GameOver → Playing | Pass |
| TC_RESTART_01 | SG-F-014 | Simulated restart returns snake (3 segs, Right, (5,5)/(4,5)/(3,5)), score 0, speed 180 ms, valid food, state Playing | Pass |
| TC_STATE_01 | SG-F-017 | Valid transition chain; Playing→Menu refused | Pass |
| TC_STATE_EXIT_01 | SG-F-015 | Exiting reachable from all four states; terminal; gameplay inactive after | Pass |
| TC_STATE_TRANSITION_01 | SG-F-013/015 | `canTransitionTo` gives correct answer for every state pair and never mutates state | Pass |
| TC_REL_01 | SGNF-002 | Full state lifecycle stays consistent; illegal moves refused | Pass |
| TC_SEC_05 | SG-SR-005 | Initial state Menu; snake 3 segs Right; food valid, in bounds, off-snake | Pass |

### 2.8 GameRenderer (SG-F-016/017, SGNF-003)
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_STATE_RENDER_01 | SG-F-017, SGNF-003 | Each of 4 states shows its state name, score and its control hints | Pass |
| TC_UX_01 | SGNF-003 | `stateName`, `controlsFor`, `buildStatus` exact strings for all states; invalid enum → `UNKNOWN` / empty controls | Pass |

### 2.9 Cross-module / non-functional
| Test | Requirement | What it verifies | Status |
|---|---|---|---|
| TC_MAINT_01 | SGNF-005 | Snake, Score, Food, Collision each work using only the others' public interfaces | Pass |
| TC_PORT_01 | SGNF-004 | `_WIN32` defined | Pass on Windows / Fail on Linux (by design) |

## 3. Requirement → unit-test traceability

| Req | Covered by unit tests | Gap |
|---|---|---|
| SG-F-001 | TC_START_01 | Only the state change; the real Enter-key handling in `GameController` is untested |
| SG-F-002 | TC_INPUT_01, TC_MOVE_01 | RTM names TC_MOVE_01, which checks initial snake state, not input. TC_INPUT_01 is the real test |
| SG-F-003 | TC_MOVE_02 | Continuous timed movement (tick loop) untested |
| SG-F-004 | TC_INPUT_01, TC_MOVE_03 | **Defect: two turns in one tick defeat it (see §5)** |
| SG-F-005 | TC_FOOD_01, TC_FOOD_03 | — |
| SG-F-006 | TC_FOOD_02 | — |
| SG-F-007 | TC_GROW_01 | — |
| SG-F-008 | TC_SCORE_01 | Controller adds score itself, bypassing `ScoreManager` |
| SG-F-009 | TC_COLL_01 | Controller uses its own `outsideBoard()` |
| SG-F-010 | TC_COLL_02 | Controller uses its own `hitsBody()` |
| SG-F-011 | TC_GAMEOVER_01 | State only; trigger path in the controller untested |
| SG-F-012 | TC_SPEED_01 | — |
| SG-F-013 | TC_PAUSE_01/02 | P-key mapping tested in TC_INPUT_01 |
| SG-F-014 | TC_RESTART_01 | Simulates, rather than calls, the controller restart |
| SG-F-015 | TC_EXIT_01, TC_STATE_EXIT_01 | — |
| SG-F-016 | TC_SCORE_02 | — |
| SG-F-017 | TC_STATE_01, TC_STATE_RENDER_01, TC_UX_01 | — |
| SGNF-001 | TC_PERF_01 | Function-level only |
| SGNF-002 | TC_REL_01 | — |
| SGNF-003 | TC_UX_01, TC_STATE_RENDER_01 | — |
| SGNF-004 | TC_PORT_01 | — |
| SGNF-005 | TC_MAINT_01 | — |
| SG-SR-001 | TC_INPUT_01 | RTM test `TC_SEC_01` is an empty file |
| SG-SR-002 | TC_INPUT_01 | RTM test `TC_SEC_02` is an empty file |
| SG-SR-003 | none | `TC_SEC_03` is an empty file |
| SG-SR-004 | TC_SEC_04 (manual) | Not automated |
| SG-SR-005 | TC_SEC_05 | — |

**Coverage of requirements:** 27 / 27 have a named test, but the RTM files for SG-SR-001/002/003 are empty and SG-SR-004 is manual. In practice 25 / 27 have a working automated test (SG-SR-001 and SG-SR-002 are genuinely exercised by TC_INPUT_01, whose comments cite them). **SG-SR-003 has no executable test**, and SG-SR-004 is manual. The exit criterion "100 % of planned test cases executed" is therefore not yet strictly met.

## 4. Missing / empty tests — proposed content

### TC_SEC_01 — SG-SR-001 (validate input before movement)
```cpp
#include "../src/InputHandler.h"
#include <cassert>
int main() {
    // Every raw char value 0..255 must map to a defined command without crashing.
    for (int c = 0; c < 256; ++c) {
        InputCommand cmd = InputHandler::parseInput(static_cast<char>(c));
        assert(cmd >= InputCommand::None && cmd <= InputCommand::Exit);
    }
    // Only an explicit move command may produce a direction.
    assert(InputHandler::parseInput('x') == InputCommand::None);
    return 0;
}
```

### TC_SEC_02 — SG-SR-002 (ignore invalid input, state unchanged)
```cpp
#include "../src/InputHandler.h"
#include <cassert>
int main() {
    SnakeManager s;
    const Direction before = s.getDirection();
    const auto bodyBefore = s.getBody().size();
    for (char c : {'x','z','1','?',' ','\t','\n','\0'})
        assert(InputHandler::parseInput(c) == InputCommand::None);
    assert(InputHandler::parseInput(InputKey::Unknown) == InputCommand::None);
    assert(s.getDirection() == before && s.getBody().size() == bodyBefore);
    return 0;
}
```

### TC_SEC_03 — SG-SR-003 (unsupported input cannot change Game State)
```cpp
#include "../src/GameStateManager.h"
#include <cassert>
int main() {
    GameStateManager g;
    // Out-of-range enum values must be rejected in every state.
    const GameState bad = static_cast<GameState>(99);
    assert(!g.setState(bad)); assert(g.getState() == GameState::Menu);
    g.setState(GameState::Playing);
    assert(!g.setState(bad)); assert(g.getState() == GameState::Playing);
    return 0;
}
```
The three snippets use only the existing public API and were compiled and run with the project's flags (all pass). Add all three to `$tests` in `run_sprint2_tests.ps1` (sources: `InputHandler`, `SnakeManager`, `GameStateManager` respectively).

## 5. New unit tests recommended (gaps and defects)

| ID | Purpose | Expected | Today |
|---|---|---|---|
| **TC_MOVE_04** | Reverse-prevention across multiple inputs before one `move()`: moving Right, `applyDirection(Up)` then `applyDirection(Left)`, then `move()`, then `assert(!checkSelfCollision(s))` | Second call rejected (or snake direction still Up); `checkSelfCollision` false | **Fails.** Both accepted; self-collision true (reproduced). Fix: validate against the direction of the *last executed move*, or accept one turn per tick |
| TC_SCORE_03 | `ScoreManager::reset()` after scoring returns 0 | 0 | Covered only incidentally by TC_MAINT_01 |
| TC_SCORE_04 | Score at +10 per food through 1 000 foods does not overflow | 10 000 | Not tested |
| TC_GROW_02 | After `grow()` + `move()` the length stays 4 and the duplicated tail resolves | size 4, tail distinct | Not tested |
| TC_GROW_03 | Grow several times consecutively | size 3 + n | Not tested |
| TC_COLL_03 | Wall collision exactly at the edge cells: head at (0,0) and (W−1,H−1) is **not** a collision; (−1,0), (W,0), (0,−1), (0,H) are | Boundary values | Partially covered |
| TC_COLL_04 | Self-collision with a long, U-turn snake (head enters segment 3+ not the neck) | true | Only neck case tested |
| TC_FOOD_04 | Food generation never lands on a long snake over 10 000 trials | always free | Not tested |
| TC_FOOD_05 | Board with exactly one free cell: food goes there | that cell | Partially (1×2) |
| TC_SPEED_02 | Interval is non-increasing as score rises 0 → 500 step 10 | monotone | Not tested |
| TC_RENDER_02 | `renderConsole` writes the status text to `cout` | contains state+score | Not tested (this is branch gap 7–9) |
| TC_CTRL_01 … | Controller tests (menu keys, pause, restart, game-over trigger, tick timing) | see `SYSTEM_TESTS.md` | Impossible until `GameController` takes injectable input/clock |

## 6. Test design rules observed (and gaps)
- One behaviour per file, deterministic (no sleeps), except `TC_FOOD_*`, which is randomised but constructed so any outcome must satisfy the same assertions. TC_FOOD_01 test 4 (100 trials) makes a failure ~certain if the occupied-retry path breaks.
- Boundary values: well covered for score thresholds (9/10/110/120) and walls; weaker for board corners (TC_COLL_03).
- Negative testing: present for invalid enums (`-1`) and invalid dimensions.
- Tests include sources by path (`../src/...`) so they are independent of the build directory.
- Weakness: several tests *simulate* the controller (`TC_RESTART_01`, `TC_SCORE_02`) rather than exercise it, so they pass even if `GameController` is wrong.

## 7. How to run
```powershell
# Windows (authoritative)
.\tests\run_sprint2_tests.ps1        # exit code 0 and "All automated tests passed."
```
```bash
# Single test on any OS (example)
g++ -std=c++17 -Wall -Wextra -pedantic -I src -o tc_input tests/TC_INPUT_01.cpp src/InputHandler.cpp src/SnakeManager.cpp && ./tc_input; echo $?
```

## 8. Exit criteria for the unit level
| Criterion | Status |
|---|---|
| All registered tests pass | **Met** (28/28 on Linux; PORT on Windows CI) |
| No compiler warnings with `-Wall -Wextra -pedantic` | Met |
| Every requirement has an executable test | **Not met** — SG-SR-003 has none, SG-SR-004 manual, SEC_01/02 files empty |
| No known failing behaviour | **Not met** — reverse-input defect (TC_MOVE_04) |
| Line coverage ≥ 90 % on instrumented modules | Met (94.1 %), but see `CODE_COVERAGE.md` for the unmeasured controller |
