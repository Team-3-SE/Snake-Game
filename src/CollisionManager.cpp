
#include "CollisionManager.h"
#include <vector>

bool CollisionManager::checkWallCollision(
    const SnakeManager& snake,
    int boardWidth,
    int boardHeight)
{
    if (boardWidth <= 0 || boardHeight <= 0)
        return true;

    const Position head = snake.getHead();

    return head.x < 0 ||
           head.x >= boardWidth ||
           head.y < 0 ||
           head.y >= boardHeight;
}

bool CollisionManager::checkSelfCollision(const SnakeManager& snake)
{
    const std::vector<Position>& body = snake.getBody();

    if (body.size() < 2)
        return false;

    const Position head = body.front();

    // Check the head against every body segment except itself.
    for (std::size_t i = 1; i < body.size(); ++i)
    {
        if (head.x == body[i].x && head.y == body[i].y)
            return true;
    }

    return false;
}

bool CollisionManager::checkFoodCollision(
    const SnakeManager& snake,
    const Position& food)
{
    const Position head = snake.getHead();

    return head.x == food.x &&
           head.y == food.y;
}
