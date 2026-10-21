
#ifndef FOOD_MANAGER_H
#define FOOD_MANAGER_H

#include "SnakeManager.h"
#include <vector>

class FoodManager
{
private:
    Position food;
    int boardWidth;
    int boardHeight;

public:
    FoodManager(int width, int height);

    void generateFood(const std::vector<Position>& snakeBody);
    Position getFood() const;
};

#endif
