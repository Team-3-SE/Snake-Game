
#include "../src/FoodManager.h"
#include <cassert>
#include <stdexcept>

int main()
{
    bool zeroWidthRejected = false;
    bool negativeHeightRejected = false;

    try
    {
        FoodManager foodManager(0, 10);
    }
    catch (const std::invalid_argument&)
    {
        zeroWidthRejected = true;
    }

    try
    {
        FoodManager foodManager(10, -5);
    }
    catch (const std::invalid_argument&)
    {
        negativeHeightRejected = true;
    }

    assert(zeroWidthRejected);
    assert(negativeHeightRejected);

    return 0;
}
