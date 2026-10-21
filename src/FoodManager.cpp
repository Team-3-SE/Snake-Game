
#include "FoodManager.h"
#include <random>
#include <stdexcept>

FoodManager::FoodManager(int width, int height)
    : food{0, 0}, boardWidth(width), boardHeight(height)
{
    if (width <= 0 || height <= 0)
    {
        throw std::invalid_argument("Board dimensions must be positive");
    }
}

void FoodManager::generateFood(const std::vector<Position>& snakeBody)
{
    if (snakeBody.size() >=
        static_cast<std::size_t>(boardWidth) *
        static_cast<std::size_t>(boardHeight))
    {
        throw std::runtime_error("No free position available for food");
    }

    static std::mt19937 generator(std::random_device{}());

    std::uniform_int_distribution<int> xDistribution(0, boardWidth - 1);
    std::uniform_int_distribution<int> yDistribution(0, boardHeight - 1);

    while (true)
    {
        Position candidate{
            xDistribution(generator),
            yDistribution(generator)
        };

        bool occupied = false;

        for (const Position& segment : snakeBody)
        {
            if (candidate.x == segment.x && candidate.y == segment.y)
            {
                occupied = true;
                break;
            }
        }

        if (!occupied)
        {
            food = candidate;
            return;
        }
    }
}

Position FoodManager::getFood() const
{
    return food;
}
