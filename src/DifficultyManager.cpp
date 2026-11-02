#include "DifficultyManager.h"

int DifficultyManager::getTickInterval(int score)
{
    if (score < 0)
        score = 0;

    const int reduction =
        (score / POINTS_PER_FOOD) * SPEEDUP_PER_FOOD_MS;

    const int interval = INITIAL_TICK_MS - reduction;

    return (interval < MINIMUM_TICK_MS)
               ? MINIMUM_TICK_MS
               : interval;
}