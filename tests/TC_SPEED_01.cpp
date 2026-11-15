
#include "../src/DifficultyManager.h"
#include <cassert>

int main()
{
    // Test 1: Initial speed.
    assert(DifficultyManager::getTickInterval(0) == 180);

    // Test 2: Speed remains unchanged below 10 points.
    assert(DifficultyManager::getTickInterval(9) == 180);

    // Test 3: Speed increases after reaching 10 points.
    assert(DifficultyManager::getTickInterval(10) == 170);

    // Test 4: Speed increases further at 50 points.
    assert(DifficultyManager::getTickInterval(50) == 130);

    // Test 5: Interval is above the minimum at 100 points.
    assert(DifficultyManager::getTickInterval(100) == 80);

    // Test 6: Interval equals the minimum at 110 points.
    assert(DifficultyManager::getTickInterval(110) == 70);

    // Test 7: Minimum interval is enforced above 110 points.
    assert(DifficultyManager::getTickInterval(120) == 70);

    // Test 8: Negative scores use the initial speed.
    assert(DifficultyManager::getTickInterval(-10) == 180);

    return 0;
}
