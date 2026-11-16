# Branch Coverage Report — Snake Game

| | |
|---|---|
| **Scope** | All modules in `src/` |
| **Tooling** | `g++ --coverage` + `gcovr` (branch data from gcov) |
| **Measured with** | GCC 13.3 / Ubuntu 24.04, `-O0 -g`, 2026-11-16 |
| **Layers** | (A) 28 unit-test executables, (B) system harness running the real `GameController.cpp` (16 scenario runs), (C) both combined |
| **Reference** | Committed CI report: 140 / 193 branches (72.5 %), unit layer only |

---

## 1. Read this first: two ways to count

gcov counts **every conditional edge the compiler emits**, including hidden *exception* edges after each call that could throw (`std::string`, `std::vector::push_back`, `std::ostringstream`, `std::cout` …). Those only execute when an allocation or stream operation fails. They are not decisions in the source and no sensible test reaches them. So there are two honest views:

| Layer | Raw gcov (what CI prints) | **Real decisions only** (`--exclude-throw-branches`) |
|---|---|---|
| A. Unit suite only (8 modules) | 140 / 194 = 72.2 % | **140 / 149 = 94.0 %** |
| B. System harness (`GameController.cpp` only) | 165 / 240 = 68.8 % | **165 / 177 = 93.2 %** |
| **C. Combined, all 9 modules** | **308 / 434 = 71.0 %** | **308 / 326 = 94.5 %** |

Of the 126 branches combined layers do not take, **108 are exception edges** (not real decisions) and **18 are genuine decision outcomes**. Those 18 are listed in §3. The CI figure (72 %) is the pessimistic raw one; ~94 % reflects test quality.

`main.cpp` (4 branches, 0 taken) is not included; the harness replaces `main`.

## 2. Per-file results

| File | A. Unit, raw | A. Unit, real | B. System, real | **C. Combined, real** | Missed real branch lines |
|---|---|---|---|---|---|
| `CollisionManager.cpp` | 28 / 32 | 28 / 29 | not used by game | **28 / 29 = 96.6 %** | 25 |
| `DifficultyManager.cpp` | 4 / 4 | 4 / 4 | 2 / 4 | **4 / 4 = 100 %** | — |
| `FoodManager.cpp` | 26 / 38 | 26 / 29 | 19 / 29 | **26 / 29 = 89.7 %** | 24 |
| `GameController.cpp` | not compiled | — | 165 / 177 | **165 / 177 = 93.2 %** | 56, 68, 148, 301, 303 |
| `GameRenderer.cpp` | 36 / 68 | 36 / 39 | 36 / 39 | **39 / 39 = 100 %** | — |
| `GameStateManager.cpp` | 14 / 16 | 14 / 16 | 11 / 16 | **14 / 16 = 87.5 %** | 33 |
| `InputHandler.cpp` | 23 / 23 | 23 / 23 | 16 / 23 | **23 / 23 = 100 %** | — |
| `ScoreManager.cpp` | 0 / 0 | 0 / 0 | not used by game | no branches | — |
| `SnakeManager.cpp` | 9 / 13 | 9 / 9 | 8 / 9 | **9 / 9 = 100 %** | — |
| **Total** | **140 / 194** | **140 / 149** | | **308 / 326 = 94.5 %** | |

("System, real" = branches taken by the harness alone, for every module the controller touches. Combined raw: 308 / 434. The first column is raw gcov for the unit layer.)

## 3. The 18 genuine decision branches still not taken

| # | File : line | Decision | Missed | Reachable? | Disposition |
|---|---|---|---|---|---|
| 1 | `CollisionManager.cpp:25` | `if (body.size() < 2)` | true outcome | No. `SnakeManager` always has ≥ 3 segments | Defensive; delete guard or add test constructor |
| 2–4 | `FoodManager.cpp:24` | branches gcov attributes to `static std::mt19937 generator`; most likely the compiler's thread-safe static-init guard (inferred from the line, not inspected in the assembly) | 3 | No (single-threaded, no recursion) | Almost certainly a compiler artifact; exclude |
| 5–6 | `GameStateManager.cpp:33` | `switch (currentState)` | `case Exiting`, implicit fall-out | No. Early `return false` on `Exiting` precedes the switch; state never out of range | Defensive; delete the `Exiting` case |
| 7 | `GameController.cpp:56` | `toDirection` `switch`: `default` | 1 | No. Called only after `isMove()` | Dead |
| 8–12 | `GameController.cpp:68` | `toRenderState` `switch`: cases Menu / Paused / GameOver / Exiting / default | 5 | No. `showScreen()` is only ever called with `GameState::Playing`; other screens call `renderConsole` directly | Dead |
| 13–15 | `GameController.cpp:148` | `showScreen`: `state == Playing \|\| state == Paused` | 3 | No. Same reason | Dead |
| 16–18 | `GameController.cpp:301, 303` | `catch (std::runtime_error&)` when `generateFood` cannot place food (board full) | 3 | Only with a 300-segment snake | **Real gap.** Needs a smaller injectable board, or a `FoodManager` test seam |

**Bottom line:** 9 of the 18 are dead code in `GameController` (rows 7–15), 6 are defensive or compiler artifacts (rows 1–6), and only **3 (rows 16–18, the board-full "you win / game over") are genuine testing gaps.**

## 4. Decision-level matrix (what *is* covered)

"T / F" = both outcomes exercised. Source column: **U** = unit test, **S** = system scenario.

### 4.1 `CollisionManager`
| Decision | True covered by | False covered by |
|---|---|---|
| `boardWidth <= 0` | U TC_COLL_01 (width 0, −1) | U TC_COLL_01 |
| `boardHeight <= 0` | U TC_COLL_01 (height 0, −1) | U TC_COLL_01 |
| `head.x < 0` / `head.x >= W` / `head.y < 0` / `head.y >= H` | U TC_COLL_01 tests 2, 4, 5, 6 (one per wall) | U TC_COLL_01 test 1 |
| `body.size() < 2` | **not covered** (row 1) | U TC_COLL_02 |
| loop + `head.x == body[i].x`, `head.y == body[i].y` | U TC_COLL_02 (same x/different y, and real overlap) | U TC_COLL_02 |
| `checkFoodCollision` x==, y== | U TC_FOOD_02 (hit, miss, same-x-different-y) | U TC_FOOD_02 |

### 4.2 `DifficultyManager::getTickInterval` — 100 %
| Decision | True | False |
|---|---|---|
| `score < 0` | U TC_SPEED_01 (−10) | U TC_SPEED_01 |
| `interval < MINIMUM_TICK_MS` | U TC_SPEED_01 (120 → 70) | U TC_SPEED_01 (100 → 80; boundary 110 → 70) |
| In the live loop | S ST-14: 180 → 170 → 160 … → 130 ms measured | |

### 4.3 `FoodManager`
| Decision | True | False |
|---|---|---|
| ctor `width <= 0 \|\| height <= 0` | U TC_FOOD_03 | U TC_FOOD_01 |
| board full | U TC_FOOD_02 (2×2) | U TC_FOOD_01 |
| candidate overlaps snake (retry) | U TC_FOOD_01 test 4 (1×2 board, 100 trials) | U TC_FOOD_01 |

### 4.4 `GameStateManager` — 14 / 16
Every `current → new` pair between different states is covered (plus all five targets from `Exiting`): **TC_STATE_TRANSITION_01** checks the answer for every pair and that state is never mutated; TC_PAUSE_02 and TC_STATE_EXIT_01 confirm via `setState`. `setState` true/false both covered. In the live loop: S ST-22…ST-26.

### 4.5 `InputHandler` — 100 %
`parseInput(InputKey)`: all 8 values. `parseInput(char)`: `w a s d p r` in both cases, `27`, unknown. `isValidDirection`: 4 reversals false, 8 turns true, invalid enum falls through. `applyDirection`: accepted / rejected / invalid current direction. Tests: TC_INPUT_01, TC_EXIT_01. Live path: S ST-05…ST-10, ST-27.

### 4.6 `SnakeManager` — 100 % of real branches
`move()` switch: Up/Down/Left/Right (U TC_MOVE_03; S ST-16…ST-19) plus invalid-direction fall-through.

### 4.7 `GameRenderer` — 100 % of real branches (combined)
`stateName`/`controlsFor`: 4 states + invalid fallback (U TC_UX_01). `renderConsole`/`clearConsole` are not touched by any unit test but **are** exercised by every system scenario, which closes the three renderer gaps in the unit-only run. Conversely the harness alone misses 3 renderer branches that the unit tests (e.g. the invalid-state fallbacks) cover; together they reach 39 / 39.

### 4.8 `GameController::run` — 165 / 177 real branches, all by system scenarios
| Decision | Outcomes covered by |
|---|---|
| Loop `state != Exiting` | ST-26 (exit from Menu, Playing, Paused, Game Over) |
| Menu: `key == 27` / `13` / other | ST-26a, ST-02, ST-27a |
| Game Over: `Esc` / `'r'` / `'R'` / other | ST-26d, ST-25, ST-22b, ST-27c |
| Paused: `Esc` / `'p'` / `'P'` / other | ST-26c, ST-23, ST-22b, ST-22/23/24 (invalid key) |
| `firstFrame` | every scenario |
| `_kbhit` loop; extended-key prefix `0`/`224`; codes 72/80/75/77; unknown extended code (F1) | ST-05…ST-08, ST-27b |
| `command == Exit / Pause / isMove` | ST-26b, ST-22, ST-05…ST-10 |
| `elapsed >= interval` | ST-04, ST-14 |
| `outsideBoard(head) \|\| hitsBody(snake)`: wall / body / neither | ST-16…ST-19 / ST-20 / ST-04 |
| `samePosition(head, food)` | ST-12 |
| `drawBoard` symbols `O` `o` `*` ` ` | every scenario |
| `catch` on `generateFood` failure | **not covered** (rows 16–18) |

## 5. Recommended actions

| Gap | Action | Result |
|---|---|---|
| Rows 7–15 (dead controller code) | Delete `toRenderState`, the `Paused` arm in `showScreen`, and `toDirection`'s `default`, or call them with real arguments | −9 uncoverable branches |
| Rows 1, 5–6 | Delete unreachable guards/case | −3 |
| Rows 2–4 | `// GCOVR_EXCL_BR_LINE` on the static generator | −3 |
| Rows 16–18 | Give `GameController` (or a thin `GameSession`) an injectable board size so a 2×2 board can be filled in a test | covers the board-full path |
| Reporting | Add `--exclude-throw-branches` to the CI `gcovr` call, and include `GameController.cpp` in the CI filter | CI prints ~94 % instead of 72 %, and includes the whole game |

After these, **100 % of reachable real-decision branches** is attainable.

## 6. Reproduce

```bash
# unit layer (see UNIT_TESTS.md §7), then:
python3 -m gcovr --root . --filter 'src/' --txt --txt-metric branch build/                                   # raw
python3 -m gcovr --root . --filter 'src/' --txt --txt-metric branch --exclude-throw-branches build/          # real
# system layer (Linux / macOS / WSL), then combined:
python3 system_harness/run_system_tests.py .
python3 -m gcovr --root . --filter 'src/' --txt --txt-metric branch --exclude-throw-branches build/ build/system/
```
The harness exercises the real controller with stubbed console I/O; see `CODE_COVERAGE.md` §5 for its limits.

## 7. Exit criteria

| Criterion | Target | Result | Status |
|---|---|---|---|
| Branch coverage, real decisions, all modules | ≥ 90 % | 94.5 % (combined) | **Met** |
| Same, unit layer only (CI-equivalent) | ≥ 90 % | 94.0 %, but excludes the controller | Met, with caveat |
| Every state-transition pair tested | 100 % | 100 % | Met |
| Every wall (4) and self-collision tested through the real game loop | 100 % | 100 % | Met |
| Raw gcov figure | informational | 71.0 % | n/a (inflated by exception edges) |
