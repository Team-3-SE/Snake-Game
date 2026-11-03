
#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "SnakeManager.h"

struct Position;

class CollisionManager
{
public:
    // SG-F-009: Detect collision with the board boundary.
    static bool checkWallCollision(
        const SnakeManager& snake,
        int boardWidth,
        int boardHeight
    );

    // SG-F-010: Detect collision between the head and body.
    static bool checkSelfCollision(const SnakeManager& snake);

    // SG-F-006: Detect when the snake's head reaches the food.
    static bool checkFoodCollision(
        const SnakeManager& snake,
        const Position& food
    );
};

#endif
