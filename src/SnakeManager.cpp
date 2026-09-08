#include "SnakeManager.h"

SnakeManager::SnakeManager()
{
    // Initial snake has 3 segments
    body.push_back({5, 5});
    body.push_back({4, 5});
    body.push_back({3, 5});

    // Initial movement direction
    direction = Direction::Right;
}

void SnakeManager::move()
{
    Position newHead = body.front();

    switch (direction)
    {
        case Direction::Up:
            newHead.y--;
            break;

        case Direction::Down:
            newHead.y++;
            break;

        case Direction::Left:
            newHead.x--;
            break;

        case Direction::Right:
            newHead.x++;
            break;
    }

    body.insert(body.begin(), newHead);

    // Remove the tail after moving
    body.pop_back();
}

void SnakeManager::grow()
{
    // Add one new segment at the current tail position
    body.push_back(body.back());
}

void SnakeManager::setDirection(Direction newDirection)
{
    direction = newDirection;
}

Direction SnakeManager::getDirection() const
{
    return direction;
}

const std::vector<Position>& SnakeManager::getBody() const
{
    return body;
}

Position SnakeManager::getHead() const
{
    return body.front();
}