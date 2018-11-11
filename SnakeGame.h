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
    static const int DELAY = 250;
    static const int WINDOW_HEIGHT = 720;
    static const int WINDOW_WIDTH = WINDOW_HEIGHT;

    SnakeGame();
    ~SnakeGame();

    void loop();
private:
    static const SDL_Color FONT_COLOR;
    static const char FONT_LOCATION[];
    static const int GAME_OVER_FONT_SIZE = 1000;
    static const int SCORE_FONT_SIZE = 72;

    SnakeBoard *board_;
    TTF_Font *game_over_msg_font_;
    SDL_Surface *game_over_msg_surface_;
    SDL_Texture *game_over_msg_texture_;
    TTF_Font *score_msg_font_;
    SDL_Surface *score_msg_surface_;
    SDL_Texture *score_msg_texture_;
    bool paused_;
    SDL_Renderer *renderer_;
    int score_;
    int time_;
    SDL_Window *window_;

    bool checkUserInput();

    void displayGameOverMsg();

    void displayMsg(SDL_Texture*, SDL_Rect&);

    void displayScore();

    void gameOver();

    void update();
};


#endif //SNAKE_SNAKE_GAME_H
