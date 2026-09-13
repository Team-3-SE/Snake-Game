#ifndef SNAKE_MANAGER_H
#define SNAKE_MANAGER_H

#include <vector>

struct Position
{
    int x;
    int y;
};

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

class SnakeManager
{
private:
    std::vector<Position> body;
    Direction direction;

public:
    SnakeManager();

    void move();
    void grow();

    void setDirection(Direction newDirection);
    Direction getDirection() const;

    const std::vector<Position>& getBody() const;
    Position getHead() const;
};

#endif