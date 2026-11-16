// System-level harness: drives the REAL src/GameController.cpp with scripted keys.
// Replaces _getch/_kbhit/Sleep/system with stubs and captures the game's console output,
// so the Windows-only controller can run (with real timing) on Linux/macOS/WSL.
// Usage: harness <scenario> [targetScore]   -- see run_system_tests.py for the scenarios.
#include "GameController.h"
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
using Clock = std::chrono::steady_clock;

static std::ostringstream g_buf;              // current frame text
static std::string g_cur;                      // snapshot used for decisions
static int g_sys = 0, g_lastFrameSys = -1;
static std::deque<int> g_keys;
static Clock::time_point g_start = Clock::now();
static std::string g_scn;
static std::ofstream g_log;

struct Frame { double t; int score; int hx, hy, fx, fy; int len; std::string state; };
static std::vector<Frame> g_frames;
static std::vector<std::string> g_events;
static int g_promptCount = 0;

extern "C" int stub_system(const char*) { g_log << g_buf.str(); g_buf.str(""); g_buf.clear(); ++g_sys; return 0; }

static double now() { return std::chrono::duration<double>(Clock::now() - g_start).count(); }
void Sleep(unsigned long ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

static bool parseFrame(const std::string& s, Frame& f) {
    if (s.find("Game State: PLAYING") == std::string::npos || s.find("Arrow keys/WASD") == std::string::npos) return false;
    f.state = "PLAYING"; f.score = 0; f.hx=f.hy=f.fx=f.fy=-1; f.len = 0;
    auto p = s.find("Score: "); if (p != std::string::npos) f.score = std::stoi(s.substr(p + 7));
    std::istringstream in(s); std::string line; int row = 0;
    while (std::getline(in, line)) {
        if (line.size() == 43 && line[0] == '|') {
            for (int x = 0; x < 20; ++x) { char c = line[2 + 2 * x];
                if (c == 'O') { f.hx = x; f.hy = row; ++f.len; }
                else if (c == 'o') ++f.len;
                else if (c == '*') { f.fx = x; f.fy = row; } }
            ++row;
        }
    }
    return row == 15;
}
static const int UP[2]={224,72}, DOWN[2]={224,80}, LEFT[2]={224,75}, RIGHT[2]={224,77};
static void push(const int* k) { g_keys.push_back(k[0]); g_keys.push_back(k[1]); }
static void pushArrow(int dir) { push(dir==0?UP:dir==1?DOWN:dir==2?LEFT:RIGHT); }
// dir: 0 up 1 down 2 left 3 right
static int g_heading = 3; // bot's belief of current heading
static int cw(int d){ return d==0?3: d==3?1: d==1?2:0; }
static int opposite(int d){ return d==0?1: d==1?0: d==2?3:2; }

static int g_uturnStage = -1; static int g_uturnHeading = 3; static bool g_selfHit = false;
static int g_frameCount = 0; static int g_gameNo = 1; static int g_framesThisGame = 0;
static int g_targetScore = 40; static bool g_done = false;

static void onFrame(const Frame& f) {
    g_frames.push_back(f); ++g_frameCount; ++g_framesThisGame;
    const int n = g_framesThisGame;
    if (n == 1) g_events.push_back("game " + std::to_string(g_gameNo) + " first frame: score " + std::to_string(f.score) + ", head (" + std::to_string(f.hx) + "," + std::to_string(f.hy) + "), len " + std::to_string(f.len));
    if (g_scn == "pause_resume") {
        if (n == 3) { g_keys.push_back('p'); g_events.push_back("pressed P at frame 3"); }
        if (n == 4 && g_gameNo == 1 && g_promptCount >= 1) { g_keys.push_back(27); }
    } else if (g_scn == "reverse_single") {
        if (n == 2) { push(LEFT); g_events.push_back("pressed LEFT (reverse) at frame 2"); }
    } else if (g_scn == "double_turn") {
        if (n == 2) { push(UP); push(LEFT); g_events.push_back("pressed UP+LEFT in same tick at frame 2"); }
    } else if (g_scn == "uppercase") {
        static bool pausedOnce = false;
        if (n == 3 && !pausedOnce) { pausedOnce = true; g_keys.push_back('P'); g_events.push_back("pressed P (uppercase) at frame 3"); }
    } else if (g_scn == "wasd") { if (n == 2) { g_keys.push_back('w'); g_events.push_back("pressed w at frame 2"); } if (n == 4) { g_keys.push_back('A'); } }
    else if (g_scn == "wall_up") { if (n == 2) { push(UP); } }
    else if (g_scn == "wall_down") { if (n == 2) { push(DOWN); } }
    else if (g_scn == "wall_left") { if (n == 2) { push(UP); } if (n == 3) { push(LEFT); } }
    else if (g_scn == "invalid_keys") {
        if (n == 2) { g_keys.push_back('x'); g_keys.push_back('z'); g_keys.push_back('?'); g_keys.push_back('r'); g_keys.push_back('R');
                      g_keys.push_back(0); g_keys.push_back(59); g_keys.push_back(13); g_events.push_back("sent x z ? r R F1 ENTER during play"); }
    } else if (g_scn == "esc_playing") { if (n == 3) g_keys.push_back(27); }
    else if (g_scn == "bot" || g_scn == "uturn") {
        // greedy food seeker
        if (g_uturnStage >= 0) {                       // executing 3 clockwise turns
            if (g_uturnStage < 3) { g_uturnHeading = cw(g_uturnHeading); pushArrow(g_uturnHeading); g_heading = g_uturnHeading; ++g_uturnStage; }
            return;
        }
        if (g_scn == "uturn" && f.score >= 20 && f.hx >= 4 && f.hx <= 15 && f.hy >= 4 && f.hy <= 10) {
            g_events.push_back("starting 3 clockwise turns at score " + std::to_string(f.score) + " len " + std::to_string(f.len)
                               + " head (" + std::to_string(f.hx) + "," + std::to_string(f.hy) + ")");
            g_uturnHeading = g_heading; g_uturnStage = 0;
            g_uturnHeading = cw(g_uturnHeading); pushArrow(g_uturnHeading); g_heading = g_uturnHeading; g_uturnStage = 1; return;
        }
        if (g_scn == "bot" && f.score >= g_targetScore) { g_keys.push_back(27); g_done = true; return; }
        int dx = f.fx - f.hx, dy = f.fy - f.hy; int want = g_heading;
        if (dx != 0 && !(dx > 0 && g_heading == 2) && !(dx < 0 && g_heading == 3)) want = dx > 0 ? 3 : 2;
        else if (dy != 0 && !(dy > 0 && g_heading == 0) && !(dy < 0 && g_heading == 1)) want = dy > 0 ? 1 : 0;
        else if (dx != 0 || dy != 0) want = (g_heading == 2 || g_heading == 3) ? 1 : 3; // sidestep to avoid reversing
        if (want != g_heading && want != opposite(g_heading)) { pushArrow(want); g_heading = want; }
    }
}

int _kbhit() {
    if (now() > 120) { g_keys.push_back(27); g_events.push_back("WATCHDOG"); }
    g_cur = g_buf.str(); Frame f;
    if (g_sys != g_lastFrameSys && parseFrame(g_cur, f)) { g_lastFrameSys = g_sys; f.t = now(); onFrame(f); }
    return !g_keys.empty();
}
int _getch() {
    if (!g_keys.empty()) { int k = g_keys.front(); g_keys.pop_front(); return k; }
    g_cur = g_buf.str(); ++g_promptCount;
    auto has = [&](const char* s){ return g_cur.find(s) != std::string::npos; };
    if (has("Press ENTER to start")) { g_events.push_back("MENU shown"); if (g_scn=="exit_menu") return 27; if (g_scn=="menu_invalid" && g_promptCount < 4) { g_events.push_back("menu: invalid key"); return 'p'; } g_framesThisGame = 0; return 13; }
    if (has("Final score:")) {
        auto p = g_cur.find("Final score: "); int fs = std::stoi(g_cur.substr(p + 13));
        g_events.push_back("GAME OVER shown, final score " + std::to_string(fs) + ", frames this game " + std::to_string(g_framesThisGame));
        if (g_scn == "uppercase" && g_gameNo == 1) { ++g_gameNo; g_framesThisGame = 0; g_events.push_back("pressed R (uppercase)"); return 'R'; }
        if (g_scn == "soak" && g_gameNo < g_targetScore) { ++g_gameNo; g_framesThisGame = 0; return 'r'; }
        if (g_scn == "wall_restart" && g_gameNo == 1) { ++g_gameNo; g_framesThisGame = 0; g_events.push_back("pressed R"); return 'r'; }
        if (g_scn == "gameover_invalid" && g_promptCount < 99) { static int c=0; if (c++ < 3) { g_events.push_back("gameover: invalid key"); return 'x'; } }
        return 27;
    }
    if (has("PAUSED - Press P")) {
        g_events.push_back("PAUSE screen shown");
        if (g_scn == "uppercase") { g_events.push_back("pressed P (uppercase) to resume"); g_framesThisGame = 0; return 'P'; }
        if (g_scn == "pause_resume" && g_gameNo == 1) { static int pc = 0; ++pc; if (pc == 1) { g_events.push_back("invalid key x while paused"); return 'x'; } if (pc == 2) { g_events.push_back("pressed P to resume"); g_framesThisGame = 0; return 'p'; } }
        return 27;
    }
    g_events.push_back("UNKNOWN PROMPT"); return 27;
}

int main(int argc, char** argv) {
    g_scn = argc > 1 ? argv[1] : "exit_menu";
    if (argc > 2) g_targetScore = std::atoi(argv[2]);
    g_log.open("transcript_" + g_scn + ".txt");
    auto* old = std::cout.rdbuf(g_buf.rdbuf());
    GameController game; game.run();
    std::string tail = g_buf.str();
    std::cout.rdbuf(old); g_log << tail;
    std::cout << "SCENARIO " << g_scn << "\n";
    for (auto& e : g_events) std::cout << "  event: " << e << "\n";
    std::cout << "  exit message shown: " << (tail.find("Thanks for playing Snake!") != std::string::npos ? "yes" : "NO") << "\n";
    std::cout << "  playing frames: " << g_frames.size() << "\n";
    if (!g_frames.empty()) {
        // tick intervals per score
        std::cout << "  first frame: score " << g_frames.front().score << " head (" << g_frames.front().hx << "," << g_frames.front().hy << ") len " << g_frames.front().len
                  << " food (" << g_frames.front().fx << "," << g_frames.front().fy << ")\n";
        std::cout << "  last frame:  score " << g_frames.back().score << " head (" << g_frames.back().hx << "," << g_frames.back().hy << ") len " << g_frames.back().len << "\n";
        std::vector<std::pair<int,std::vector<double>>> bands;
        for (size_t i = 1; i < g_frames.size(); ++i) {
            double dt = (g_frames[i].t - g_frames[i-1].t) * 1000.0; if (dt > 400) continue;
            int sc = g_frames[i-1].score;
            auto it = std::find_if(bands.begin(), bands.end(), [&](auto& b){return b.first == sc;});
            if (it == bands.end()) { bands.push_back({sc, {dt}}); } else it->second.push_back(dt);
        }
        for (auto& b : bands) { double s = 0; for (double d : b.second) s += d; std::cout << "  score " << b.first << ": mean tick " << (int)(s / b.second.size()) << " ms over " << b.second.size() << " ticks\n"; }
        // food validity & growth
        int bad = 0; int maxLen = 0; int prevScore = 0, prevLen = 3; bool growOK = true, scoreStep = true;
        for (auto& f : g_frames) { if (f.fx < 0 || f.fx > 19 || f.fy < 0 || f.fy > 14) ++bad; maxLen = std::max(maxLen, f.len);
            if (f.score != prevScore) { if (f.score - prevScore != 10) scoreStep = false; if (f.len != prevLen + 1 && f.len != prevLen) growOK = growOK; prevScore = f.score; }
            prevLen = f.len; }
        std::cout << "  food always inside board: " << (bad == 0 ? "yes" : "NO") << "; every score step == +10: " << (scoreStep ? "yes" : "NO") << "; max drawn length " << maxLen << "\n";
    }
    return 0;
}
