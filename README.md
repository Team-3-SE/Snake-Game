# Snake Game — SE Mini Project

A single-player, standalone desktop Snake Game implemented in C/C++, using SFML for rendering with a console-based build as a fallback interface.

This repository implements the requirements defined in the team's Software Requirements Specification (SRS). Every module below traces back to the SRS's functional requirements (`SG-F-###`), non-functional requirements (`SGNF-###`), and security requirements (`SG-SR-###`).

## Team

| Member | GitHub | SRS Sections Owned | Code Modules Owned |
|---|---|---|---|
| Krishna | [@prajothgrandhi-creator](https://github.com/prajothgrandhi-creator) | 1 (Introduction), 4 (System Features) | GameController, GameStateManager |
| Krithika | [@krithika297](https://github.com/krithika297) | 2 (Overall Description), 7 (System Models) | SnakeManager, DifficultyManager, FoodManager |
| Jaanhavi | [@janhavik777](https://github.com/janhavik777) | 3 (External Interfaces), 8 (RTM) | InputHandler, GameRenderer, tests/ |
| Joey | [@joeyfedrick74-creator](https://github.com/joeyfedrick74-creator) | 5 (Non-Functional Requirements), 6 (Quality Attributes) | CollisionManager, ScoreManager |

Module ownership is also encoded in `.github/CODEOWNERS`, so GitHub automatically requests the right reviewer on any pull request touching a given module.

## Project Decisions (Fixed)

These are locked project decisions and should not be changed without team agreement:

- Grid-based movement, keyboard input only
- Score: +10 per food item
- Speed increases as score increases
- Controls: Arrow keys (move), `P` (pause/resume), `R` (restart), `Esc` (exit)
- No network, no database, no multiplayer, no online leaderboard

## Build

```bash
# SFML build
g++ -std=c++17 src/*.cpp -o snake_game -lsfml-graphics -lsfml-window -lsfml-system

# Console-only build
g++ -std=c++17 -DCONSOLE_MODE src/*.cpp -o snake_game_console
```

## Project Structure

```
snake-game-se-project/
├── src/
│   ├── GameController.cpp / .h        # SG-F-001, SG-F-014, SG-F-015
│   ├── InputHandler.cpp / .h          # SG-F-002, SG-F-004, SG-SR-001, SG-SR-002
│   ├── SnakeManager.cpp / .h          # SG-F-003, SG-F-007
│   ├── FoodManager.cpp / .h           # SG-F-005
│   ├── CollisionManager.cpp / .h      # SG-F-006, SG-F-009, SG-F-010
│   ├── ScoreManager.cpp / .h          # SG-F-008, SG-F-016
│   ├── GameRenderer.cpp / .h          # SG-F-016, SG-F-017, SGNF-003
│   ├── GameStateManager.cpp / .h      # SG-F-011, SG-F-013, SG-F-017
│   └── DifficultyManager.cpp / .h     # SG-F-012
├── tests/
│   ├── TC_START_01.cpp
│   ├── TC_MOVE_01.cpp ... TC_MOVE_03.cpp
│   ├── TC_FOOD_01.cpp / TC_FOOD_02.cpp
│   ├── TC_GROW_01.cpp
│   ├── TC_SCORE_01.cpp / TC_SCORE_02.cpp
│   ├── TC_COLL_01.cpp / TC_COLL_02.cpp
│   ├── TC_GAMEOVER_01.cpp
│   ├── TC_SPEED_01.cpp
│   ├── TC_PAUSE_01.cpp
│   ├── TC_RESTART_01.cpp
│   ├── TC_EXIT_01.cpp
│   ├── TC_STATE_01.cpp
│   ├── TC_PERF_01.cpp / TC_REL_01.cpp / TC_UX_01.cpp / TC_PORT_01.cpp / TC_MAINT_01.cpp
│   └── TC_SEC_01.cpp ... TC_SEC_05.cpp
├── docs/
│   └── SRS/                           # Final SRS document(s)
├── .gitignore
└── README.md
```

Test file names follow the SRS's naming convention, `TC-[FUNCTION]-[NUMBER]`, so any test can be traced straight back to its row in the Requirements Traceability Matrix (Section 8 of the SRS).

## Branching & Commit Convention

- `main` is protected — no direct pushes; all changes go through a pull request.
- Feature branches: `feature/<short-description>`, e.g. `feature/snake-movement`.
- Commit messages should reference the requirement ID they implement, e.g.:
  ```
  Implement SG-F-002: keyboard input handling for snake direction
  ```
- Pull request descriptions should list which `SG-F-###` / `SGNF-###` / `SG-SR-###` IDs are addressed, so reviewers can cross-check against the SRS and RTM.

## Modules (Conceptual)

These do not need to be individual C++ classes if a simpler structure is more practical:

Game Controller · Input Handler · Snake Manager · Food Manager · Collision Manager · Score Manager · Game Renderer · Game State Manager · Difficulty Manager
