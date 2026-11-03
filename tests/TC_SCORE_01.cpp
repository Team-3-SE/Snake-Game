
#include "../src/ScoreManager.h"
#include <cassert>

int main()
{
    ScoreManager scoreManager;

    // Test 1: Initial score should be zero.
    assert(scoreManager.getScore() == 0);

    // Test 2: Score should increase by 10 after collecting food.
    scoreManager.addFoodScore();
    assert(scoreManager.getScore() == 10);

    // Test 3: Collecting another food should add another 10 points.
    scoreManager.addFoodScore();
    assert(scoreManager.getScore() == 20);

    // Test 4: Score should continue increasing correctly.
    scoreManager.addFoodScore();
    assert(scoreManager.getScore() == 30);

    return 0;
}
