#include "../src/FoodManager.h"
#include <cassert>
#include <stdexcept>

int main()
{
    FoodManager foodManager(2, 2);
    std::vector<Position> fullSnake = {
        {0, 0}, {1, 0}, {0, 1}, {1, 1}
    };

    bool exceptionThrown = false;

    try
    {
        foodManager.generateFood(fullSnake);
    }
    catch (const std::runtime_error&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    return 0;
}