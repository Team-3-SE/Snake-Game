#!/usr/bin/env python3
"""Builds the real GameController against stub conio/windows headers and runs the
scripted system scenarios, asserting expected behaviour.  Linux / macOS / WSL only.

    python3 run_system_tests.py [PROJECT_ROOT] [--coverage]

PROJECT_ROOT defaults to the current directory (must contain src/).
Exit code 0 = all non-known checks passed.
"""
import os, re, subprocess, sys, shutil

HERE = os.path.dirname(os.path.abspath(__file__))
args = [a for a in sys.argv[1:] if not a.startswith("--")]
ROOT = os.path.abspath(args[0] if args else ".")
COVER = "--coverage" in sys.argv
SRC = os.path.join(ROOT, "src")
OUT = os.path.join(ROOT, "build", "system")
os.makedirs(OUT, exist_ok=True)
for f in os.listdir(OUT):
    if f.endswith(".gcda"): os.remove(os.path.join(OUT, f))

def sh(cmd, **kw): return subprocess.run(cmd, shell=True, capture_output=True, text=True, **kw)

# ---- build ----
base = f"-std=c++17 -O0 -g --coverage -I{HERE} -I{SRC}"
objs = []
for name, shim in [("GameController", True), ("GameRenderer", True), ("DifficultyManager", False),
                   ("FoodManager", False), ("InputHandler", False), ("SnakeManager", False),
                   ("GameStateManager", False)]:
    o = os.path.join(OUT, name + ".o"); objs.append(o)
    inc = f"-include {HERE}/shim.h" if shim else ""
    r = sh(f"g++ {base} {inc} -c {SRC}/{name}.cpp -o {o}")
    if r.returncode: sys.exit("BUILD FAILED " + name + "\n" + r.stderr)
o = os.path.join(OUT, "harness.o"); objs.append(o)
r = sh(f"g++ {base} -c {HERE}/harness.cpp -o {o}")
if r.returncode: sys.exit("BUILD FAILED harness\n" + r.stderr)
exe = os.path.join(OUT, "harness")
r = sh(f"g++ --coverage {' '.join(objs)} -o {exe}")
if r.returncode: sys.exit("LINK FAILED\n" + r.stderr)

def run(scn, extra=""):
    r = sh(f"cd {OUT} && timeout 120 ./harness {scn} {extra}")
    return r.stdout

def frames(o):   return int(re.search(r"playing frames: (\d+)", o).group(1))
def exited(o):   return "exit message shown: yes" in o
def gameovers(o): return [int(x) for x in re.findall(r"GAME OVER shown, final score (\d+)", o)]
def gofr(o):     return [int(x) for x in re.findall(r"frames this game (\d+)", o)]

results = []
def check(id_, title, cond, known=False, note=""):
    results.append((id_, title, bool(cond), known, note))

def transcript(scn):
    return open(os.path.join(OUT, f"transcript_{scn}.txt"), errors="replace").read()

o = run("exit_menu");      check("ST-26a", "Esc on Menu exits", exited(o) and frames(o) == 0)
o = run("menu_invalid");   check("ST-27a", "Invalid keys on Menu ignored; Enter starts", exited(o) and o.count("menu: invalid key") == 3 and frames(o) == 15)
o = run("esc_playing");    check("ST-26b", "Esc while Playing exits", exited(o) and frames(o) == 3)
o = run("wall_restart")
check("ST-02/03", "Enter starts game: initial snake len 3 at (5,5), score 0", "game 1 first frame: score 0, head (5,5), len 3" in o)
check("ST-04", "Snake advances one cell per tick with no input (15 ticks to right wall)", gofr(o)[:1] == [15])
check("ST-26d", "Esc on Game Over screen exits", exited(o) and len(gameovers(o)) == 2)
check("ST-16", "Right wall: Game Over after 15 ticks, final score shown", gofr(o)[:1] == [15] and gameovers(o)[:1] == [0])
check("ST-21", "Game Over screen shows final score", len(gameovers(o)) == 2)
check("ST-25", "R restarts: snake/score reset", "game 2 first frame: score 0, head (5,5), len 3" in o and "pressed R" in o)
run("wall_up")
o = run("pause_resume")
check("ST-22/23/24", "P pauses, invalid key ignored while paused, P resumes at the same position",
      o.count("PAUSE screen shown") >= 2 and "pressed P to resume" in o and "invalid key x while paused" in o
      and "first frame: score 0, head (7,5)" in o)
check("ST-26c", "Esc on Paused screen exits", o.count("PAUSE screen shown") == 3 and exited(o))
check("ST-01", "Menu screen: title, 'Game State: MENU', 'Score: 0', controls, start/exit prompt",
      all(x in transcript("exit_menu") for x in ["SNAKE GAME", "Game State: MENU", "Score: 0",
          "Controls: Enter: Start | Esc: Exit", "Press ENTER to start or ESC to exit."]))
o = run("uppercase")
check("ST-22b", "Uppercase P (pause/resume) and R (restart) accepted",
      "pressed P (uppercase) to resume" in o and "pressed R (uppercase)" in o and "game 2 first frame: score 0, head (5,5), len 3" in o and exited(o))
t = transcript("pause_resume") + transcript("wall_up")
check("ST-33", "State + controls shown on Playing / Paused / Game Over screens (SG-F-017, SGNF-003)",
      all(x in t for x in ["Game State: PLAYING", "Controls: Arrow keys: Move | P: Pause | Esc: Exit",
          "Game State: PAUSED", "Controls: P: Resume | Esc: Exit", "PAUSED - Press P to resume or ESC to exit.",
          "Game State: GAME OVER", "Controls: R: Restart | Esc: Exit", "Final score: 0", "Press R to restart or ESC to exit."]))
o = run("reverse_single"); check("ST-09", "Single reverse key (Left while moving Right) ignored", frames(o) == 15 and gameovers(o) == [0])
o = run("double_turn");    check("ST-10", "Two turns in one tick (Up, Left) must not reverse the snake", frames(o) > 3, known=True,
                                  note=f"observed Game Over after {frames(o)} frames (SG-F-004 defect)")
o = run("wall_up");        check("ST-05/17", "Up arrow turns snake; top wall ends game", gofr(o) == [7])
o = run("wall_down");      check("ST-06/18", "Down arrow turns snake; bottom wall ends game", gofr(o) == [11])
o = run("wall_left");      check("ST-07/19", "Up then Left arrows; left wall ends game", gofr(o) == [9])
o = run("wasd");           check("ST-08", "WASD accepted ('w', 'A')", gofr(o) == [10])
o = run("invalid_keys");   check("ST-27b", "x z ? r R F1 ENTER during play ignored (no restart, no state change)", frames(o) == 15 and gameovers(o) == [0])
o = run("gameover_invalid"); check("ST-27c", "Invalid keys on Game Over ignored", len(gameovers(o)) == 4 and exited(o))
o = run("soak", "10")
check("ST-29a", "10 consecutive games via R: no crash, every game restarts from the initial state",
      exited(o) and len(gameovers(o)) == 10 and all(f == 15 for f in gofr(o))
      and o.count("first frame: score 0, head (5,5), len 3") == 10)
o = run("bot", "60")
bands = {int(s): (int(ms), int(n)) for s, ms, n in re.findall(r"score (\d+): mean tick (\d+) ms over (\d+) ticks", o)}
ok_speed = all(180 - s <= ms <= 180 - s + 15 for s, (ms, n) in bands.items() if n >= 3)
check("ST-11", "Food always inside board", "food always inside board: yes" in o)
check("ST-12/13", "Score rises exactly +10 per food; reaches 60", "every score step == +10: yes" in o and "last frame:  score 60" in o)
check("ST-14", "Tick interval follows 180 - score ms (within +15 ms scheduling overhead): " + ", ".join(f"{s}:{ms}ms" for s, (ms, n) in sorted(bands.items())), ok_speed and len(bands) >= 5)
o = run("uturn")
m = re.search(r"last frame:\s+score (\d+) head \((\d+),(\d+)\) len (\d+)", o)
interior = m and 1 <= int(m.group(2)) <= 18 and 1 <= int(m.group(3)) <= 13
check("ST-20", "Self-collision ends the game (head in board interior, not at a wall)", interior and len(gameovers(o)) == 1 and int(m.group(4)) >= 5)

w = max(len(r[0]) for r in results)
fails = known = 0
for id_, title, ok, kn, note in results:
    tag = "PASS" if ok else ("KNOWN-FAIL" if kn else "FAIL")
    fails += (not ok and not kn); known += (not ok and kn)
    print(f"{tag:10} {id_:{w}}  {title}" + (f"   [{note}]" if (not ok and note) else ""))
print(f"\n{sum(r[2] for r in results)} passed, {fails} failed, {known} known failure(s), {len(results)} total")

if COVER:
    print("\nGameController coverage from system scenarios:")
    print(sh(f"cd {ROOT} && python3 -m gcovr --root . --filter 'src/GameController' --txt-summary {OUT}").stdout)
sys.exit(1 if fails else 0)
