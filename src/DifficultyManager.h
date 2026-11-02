#ifndef DIFFICULTY_MANAGER_H
#define DIFFICULTY_MANAGER_H

class DifficultyManager
{
private:
    static constexpr int INITIAL_TICK_MS = 180;
    static constexpr int MINIMUM_TICK_MS = 70;
    static constexpr int SPEEDUP_PER_FOOD_MS = 10;
    static constexpr int POINTS_PER_FOOD = 10;

public:
    static int getTickInterval(int score);
};

#endif