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
    Direction movedDirection;

public:
    SnakeManager();

    void move();
    void grow();

    void setDirection(Direction newDirection);
    Direction getDirection() const;
    // Direction used by the most recent move(); input is validated against it.
    Direction getMovedDirection() const;

    const std::vector<Position>& getBody() const;
    Position getHead() const;
};

#endif