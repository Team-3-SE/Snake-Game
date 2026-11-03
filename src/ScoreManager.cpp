#include "ScoreManager.h"

ScoreManager::ScoreManager()
    : score(0)
{
}

void ScoreManager::reset()
{
    score = 0;
}

void ScoreManager::addFoodScore()
{
    score += 10;
}

int ScoreManager::getScore() const
{
    return score;
}