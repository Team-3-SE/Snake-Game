#include "../src/DifficultyManager.h"
#include <cassert>

int main()
{
    // Test 1: Initial speed.
    assert(DifficultyManager::getTickInterval(0) == 180);

    // Test 2: Speed should remain unchanged below 10 points.
    assert(DifficultyManager::getTickInterval(9) == 180);

    // Test 3: Speed should increase after reaching 10 points.
    assert(DifficultyManager::getTickInterval(10) == 170);

    // Test 4: Speed should increase further at 50 points.
    assert(DifficultyManager::getTickInterval(50) == 130);

    // Test 5: Minimum tick interval should be maintained.
    assert(DifficultyManager::getTickInterval(110) == 70);

    // Test 6: Negative scores should use the initial speed.
    assert(DifficultyManager::getTickInterval(-10) == 180);

    return 0;
}
