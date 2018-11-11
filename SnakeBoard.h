#ifndef SNAKE_SNAKE_BOARD_H
#define SNAKE_SNAKE_BOARD_H

#include <utility>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "Snake.h"

class SnakeBoard
{
public:
    /**
     * An enum representing the four directions in which a Snake can travel.
     */

    static const int COLUMNS = 10;
    static const int ROWS = 10;

    static const int WINDOW_HEIGHT = 720;
    static const int WINDOW_WIDTH = WINDOW_HEIGHT;

    SnakeBoard();

    ~SnakeBoard();

    /**
     * Advances the Snake in the given direction.
     * @param d the direction in which to move
     */
    void advanceSnake();

    void displayGameOverMsg();

    void draw();

    static void drawBlock(SDL_Renderer *renderer, SDL_Color color, int row, int col);

    void feedSnake();

    bool isGameOver();

    void placeNewFood();

    void reset();

    void setSnakeDirection(Snake::Direction);

    bool snakeAte();

private:
    static const SDL_Color GRID_LINE_COLOR;
    static const SDL_Color FOOD_COLOR;
    static const SDL_Color FONT_COLOR;
    static const char FONT_LOCATION[];
    static const int GAME_OVER_FONT_SIZE = 1000;
    static const int SCORE_FONT_SIZE = 72;

    std::pair<int, int> foodCoords_; // row and column of the food (0 <= c < COLUMNS, 0 <= r < ROWS)

    TTF_Font *game_over_msg_font_;
    SDL_Surface *game_over_msg_surface_;
    SDL_Texture *game_over_msg_texture_;

    SDL_Renderer *renderer_;

    TTF_Font *score_msg_font_;
    SDL_Surface *score_msg_surface_;
    SDL_Texture *score_msg_texture_;

    Snake *snake_;

    SDL_Window *window_;

    void displayMsg(SDL_Texture*, SDL_Rect&);

    void displayScore();

    void drawFood() const;

    void drawGrid() const;

    void makeSnake();
};


#endif //SNAKE_SNAKE_BOARD_H
