
#include "../src/SnakeManager.h"
#include "../src/FoodManager.h"
#include "../src/CollisionManager.h"
#include "../src/ScoreManager.h"

#include <cassert>
#include <iostream>

int main()
{
    std::cout << "Starting TC-MAINT-01...\n";

    // Test 1: SnakeManager operates independently
    SnakeManager snake;

    const auto initialSize = snake.getBody().size();
    assert(initialSize > 0);

    snake.move();
    assert(snake.getBody().size() == initialSize);

    snake.grow();
    assert(snake.getBody().size() == initialSize + 1);

    std::cout << "SnakeManager: PASSED\n";

    // Test 2: ScoreManager operates independently
    ScoreManager score;

    score.reset();
    assert(score.getScore() == 0);

    score.addFoodScore();
    assert(score.getScore() > 0);

    score.reset();
    assert(score.getScore() == 0);

    std::cout << "ScoreManager: PASSED\n";

    // Test 3: FoodManager operates independently
    FoodManager food(20, 15);

    food.generateFood(snake.getBody());

    Position foodPosition = food.getFood();

    assert(foodPosition.x >= 0 && foodPosition.x < 20);
    assert(foodPosition.y >= 0 && foodPosition.y < 15);

    std::cout << "FoodManager: PASSED\n";

    // Test 4: CollisionManager uses SnakeManager's interface
    const Position head = snake.getHead();

    const bool foodCollision =
        CollisionManager::checkFoodCollision(snake, foodPosition);

    const bool expectedCollision =
        head.x == foodPosition.x && head.y == foodPosition.y;

    assert(foodCollision == expectedCollision);

    std::cout << "CollisionManager: PASSED\n";

    std::cout << "\nTC-MAINT-01 PASSED\n";

    return 0;
}
