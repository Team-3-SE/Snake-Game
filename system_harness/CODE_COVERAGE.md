# Code Coverage Report — Snake Game

| | |
|---|---|
| **Scope** | All 9 modules in `src/` + `main.cpp` |
| **Metrics** | Line and function coverage (branch coverage is in `BRANCH_COVERAGE.md`) |
| **Tooling** | `g++ --coverage` (gcov) + `gcovr` |
| **Measured with** | GCC 13.3, Ubuntu 24.04, `-O0 -g`, 2026-11-16 |
| **Two test layers measured** | (a) the 28 unit-test executables; (b) the system harness in `system_harness/`, which runs the **real** `GameController.cpp` with scripted keys |
| **Reference** | Committed CI report `docs/coverage-report/` (gcovr 8.8, MinGW, Windows) |

---

## 1. Headline numbers

| View | Lines | Functions | What it represents |
|---|---|---|---|
| **A. Unit suite only** (what your CI report shows) | **191 / 203 = 94.1 %** | 30 / 32 = 93.8 % | Only the 8 modules a unit test links in. `GameController.cpp` and `main.cpp` are **absent**, not 0 % |
| **B. System harness only** | `GameController.cpp`: **158 / 168 = 94.0 %** | 11 / 11 | The real controller, driven by 16 scripted scenario runs (24 checks) |
| **C. Unit + system combined** | **356 / 371 = 96.0 %** | **43 / 43 = 100 %** | Everything except `main.cpp` |
| C + `main.cpp` (4 lines, not run by either layer) | 356 / 375 = 94.9 % | | Whole `src/` |

**Why this matters:** the CI percentage (view A, 94 %) leaves out the file that contains the entire game loop, 168 of the project's 375 executable lines (45 %). Before the harness existed that file had **no test of any kind**. View C is the honest whole-project figure.

## 2. Per-file results

| File | A. Unit | B. System | **C. Combined** | Still uncovered (combined) |
|---|---|---|---|---|
| `CollisionManager.cpp` | 20 / 21 | not used by game | **20 / 21 (95.2 %)** | 26 |
| `DifficultyManager.cpp` | 8 / 8 | 7 / 8 | **8 / 8 (100 %)** | — |
| `FoodManager.cpp` | 27 / 27 | 22 / 27 | **27 / 27 (100 %)** | — |
| `GameController.cpp` | not compiled | 158 / 168 | **158 / 168 (94.0 %)** | 62, 70, 72–74, 76, 301, 303–305 |
| `GameRenderer.cpp` | 25 / 33 | 30 / 33 | **32 / 33 (97.0 %)** | 50 |
| `GameStateManager.cpp` | 27 / 30 | 25 / 30 | **27 / 30 (90.0 %)** | 48, 49, 52 |
| `InputHandler.cpp` | 37 / 37 | 29 / 37 | **37 / 37 (100 %)** | — |
| `ScoreManager.cpp` | 11 / 11 | not used by game | **11 / 11 (100 %)** | — |
| `SnakeManager.cpp` | 36 / 36 | 36 / 36 | **36 / 36 (100 %)** | — |
| `main.cpp` | not compiled | harness has its own `main` | 0 / 4 | all |
| **Total** | **191 / 203** | | **356 / 371 (96.0 %)** | |

The committed Windows/MinGW report differs by ±1 line in a few files (e.g. it counts `GameStateManager.cpp` as 30/30 where GCC 13 on Linux marks lines 48, 49, 52 unreachable). Compiler difference only; no conclusion changes.

## 3. The 15 remaining uncovered lines — all explained

| Lines | Code | Why not covered | Action |
|---|---|---|---|
| `CollisionManager.cpp` 26 | `return false;` for `body.size() < 2` | Unreachable: `SnakeManager` always has ≥ 3 segments | Delete guard or add a test constructor |
| `GameStateManager.cpp` 48–49 | `case Exiting: return false;` inside `switch` | Unreachable: early return on `Exiting` precedes the switch | Delete the case |
| `GameStateManager.cpp` 52 | `return false;` after the `switch` | Needs an out-of-range stored state; `setState` never stores one | Keep for compiler warnings, exclude |
| `GameRenderer.cpp` 50 | `<< "Game State: " << stateName(state) …` | **gcov artifact.** The line demonstrably runs: the tests and scenarios read `Game State: …` from the output it produces. | Ignore |
| `GameController.cpp` 62 | `toDirection()` `default:` | Unreachable: only called after `isMove(command)` | Remove `default` or keep |
| `GameController.cpp` 70, 72–74, 76 | `toRenderState()` cases for Menu / Paused / GameOver, and fall-through | **Dead code in practice:** `showScreen()` is only ever called with `GameState::Playing`; Menu/Paused/GameOver screens call `renderConsole()` with the `RenderGameState` value directly. Same for `showScreen`'s `state == Paused` board branch | Simplify or remove |
| `GameController.cpp` 301, 303–305 | `catch (const std::runtime_error&)` when `generateFood` fails | Needs a snake filling all 300 cells of the 20×15 board; unreachable by play or by scripted input | Needs a smaller injectable board size to test |
| `main.cpp` | `main()` | Harness supplies its own `main` | Trivial; add a smoke test that runs the real binary |

Net: **only the board-full catch (4 lines) and `main` are real, testable-with-effort gaps.** Everything else is dead or defensive code.

## 4. Findings that matter beyond the percentages

1. **`CollisionManager` and `ScoreManager` are not used by the game.** The system harness shows 0 executed lines for both because `GameController.cpp` never calls them; it has its own `outsideBoard()`, `hitsBody()` and `score += POINTS_PER_FOOD`. They are well unit-tested (95 % / 100 %) but the shipped game runs different, duplicate code. Behaviour matches today (system scenarios ST-16…ST-20 pass), but a change to one copy will not be caught by tests of the other.
2. **Reverse-direction defect (SG-F-004), reproduced through the real controller.** Moving Right, pressing Up then Left within one tick ends the game after 2 frames with final score 0 (ST-10). Cause: `applyDirection` validates against the direction already overwritten by the previous key. Also reproduced at unit level.
3. **Three copies of project constants:** board 20×15 and `POINTS_PER_FOOD = 10` in `GameController.cpp`; `+10` hard-coded in `ScoreManager`; 10-point step in `DifficultyManager`.
4. **README says "SFML build"; no source uses SFML.** The only match is a comment in `InputHandler.h`. The implementation is console-only and Windows-only (`<conio.h>`, `<windows.h>`). The README's `-lsfml-*` command is misleading, and `-DCONSOLE_MODE` has no effect (the macro is never read).
5. **`TC_PORT_01` fails on Linux by design** (verified); 28 of the 29 runner tests are portable.

## 5. Limits of the system-harness measurement

- The harness replaces `_getch`, `_kbhit`, `Sleep` and `system()` with stubs and keeps real wall-clock timing. It exercises the real control flow and timing logic, **not** the Windows console. It feeds the same key codes the controller expects (224 + 72/80/75/77), so it does not prove Windows delivers those codes (standard `_getch` behaviour, but unverified here).
- `system("cls")` is stubbed out, so screen clearing and flicker are not tested.
- Coverage was measured with GCC on Linux, not the Windows CI toolchain.

## 6. How to reproduce

```bash
# (A) unit layer: see UNIT_TESTS.md §7, then
python3 -m gcovr --root . --filter 'src/' --txt build/
# (B)+(C) system layer — Linux / macOS / WSL
python3 system_harness/run_system_tests.py . --coverage
python3 -m gcovr --root . --filter 'src/' --txt-summary build/ build/system/
```
Windows CI (unchanged): `.\tests\run_sprint2_tests.ps1`, then the existing `gcovr` step.

## 7. Recommended actions

| # | Action | Effect |
|---|---|---|
| 1 | Add `src/GameController.cpp` to the CI coverage filter (and ideally run the harness on the ubuntu job) so the 45 % hidden in view A becomes visible | Honest CI number |
| 2 | Make `GameController` use `CollisionManager` / `ScoreManager` and share constants | Removes duplicate logic; makes unit tests meaningful |
| 3 | Fix the reverse-input defect (ST-10 / TC_MOVE_04) | SG-F-004 compliant |
| 4 | Remove dead code (§3 rows 1, 2, 5, 6) | +~9 lines to 100 % reachable |
| 5 | Add an injectable board size or `generateFood` failure seam to test the board-full path | +4 lines |

**Targets:** ≥ 90 % line coverage on the whole of `src/` (met: 94.9–96.0 %); 100 % of *reachable* lines.
