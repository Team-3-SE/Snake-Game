

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "GameStateManager.h"

class GameController
{
private:
    GameStateManager stateManager;


public:
    GameController();

    void run();
};

#endif
