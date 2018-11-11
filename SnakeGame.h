#ifndef SNAKE_SNAKE_GAME_H
#define SNAKE_SNAKE_GAME_H

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "SnakeBoard.h"

class SnakeGame
{
public:
    static const int DELAY = 100;

    SnakeGame();
    ~SnakeGame();

    void loop();
private:
    SnakeBoard *board_;
    bool paused_;
    int score_;
    int time_;

    bool checkUserInput(Snake::Direction &d);

    void gameOver();

    void update();
};


#endif //SNAKE_SNAKE_GAME_H
