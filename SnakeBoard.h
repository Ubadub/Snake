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

    SnakeBoard();

    ~SnakeBoard();

    static void drawBlock(SDL_Renderer *renderer, SDL_Color color, int row, int col);

    void draw(SDL_Renderer *renderer);

    void placeNewFood();

    /**
     * Advances the Snake in the given direction.
     * @param d the direction in which to move
     */
    void advanceSnake();

    void feedSnake();

    bool isGameOver();

    bool snakeAte();

    void setSnakeDirection(Snake::Direction);

    void displayGameOverMsg(SDL_Renderer*);

    void displayMsg(SDL_Renderer*, SDL_Texture*, SDL_Rect&);

    void displayScore(SDL_Renderer*);


private:
    static SDL_Color GRID_LINE_COLOR;
    static SDL_Color FOOD_COLOR;

    static const SDL_Color FONT_COLOR;
    static const char FONT_LOCATION[];
    static const int GAME_OVER_FONT_SIZE = 1000;
    static const int SCORE_FONT_SIZE = 72;

    TTF_Font *game_over_msg_font_;
    SDL_Surface *game_over_msg_surface_;
    SDL_Texture *game_over_msg_texture_;

    TTF_Font *score_msg_font_;
    SDL_Surface *score_msg_surface_;
    SDL_Texture *score_msg_texture_;

    std::pair<int, int> foodCoords_; // row and column of the food (0 <= c < COLUMNS, 0 <= r < ROWS)
    Snake *snake_;

    void drawGrid(SDL_Renderer *renderer) const;

    void drawFood(SDL_Renderer *renderer) const;
};


#endif //SNAKE_SNAKE_BOARD_H
