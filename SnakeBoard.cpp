#include <iostream>
#include <utility>

#include "SnakeGame.h"
#include "SnakeBoard.h"

SDL_Color SnakeBoard::GRID_LINE_COLOR = {0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE}; // white
SDL_Color SnakeBoard::FOOD_COLOR = {0xFF, 0xFF, 0x00, SDL_ALPHA_OPAQUE}; // yellow

const SDL_Color SnakeBoard::FONT_COLOR = {255, 255, 255}; // white
const char SnakeBoard::FONT_LOCATION[] = "resources/fonts/lato/Lato-Bold.ttf";

SnakeBoard::SnakeBoard() : snake_(new Snake(ROWS / 2, COLUMNS / 2))
{
    placeNewFood();
}

SnakeBoard::~SnakeBoard()
{
    delete snake_;
}

void SnakeBoard::drawBlock(SDL_Renderer *renderer, SDL_Color color, int row, int col)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect block
    {
            (SnakeGame::WINDOW_WIDTH / COLUMNS) * col,
            (SnakeGame::WINDOW_HEIGHT / ROWS) * row,
            SnakeGame::WINDOW_WIDTH / COLUMNS,
            SnakeGame::WINDOW_HEIGHT / ROWS
    };

    SDL_RenderFillRect(renderer, &block);
}


void SnakeBoard::draw(SDL_Renderer *renderer)
{
    // first clear the board
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    drawGrid(renderer);
    drawFood(renderer);
    snake_->draw(renderer);
    displayScore(renderer);

    SDL_RenderPresent(renderer);
}

void SnakeBoard::placeNewFood()
{
    bool placed = false;

    while (!placed)
    {
        foodCoords_ = std::make_pair(rand() % ROWS, rand() % COLUMNS);
        placed = !snake_->collidesWith(foodCoords_);
    }
}

void SnakeBoard::advanceSnake()
{
    snake_->move();
}

bool SnakeBoard::isGameOver()
{
    int headRow = snake_->getHeadRow();
    int headCol = snake_->getHeadColumn();
    return headRow >= ROWS    || headRow < 0 // game over if out of bounds
        || headCol >= COLUMNS || headCol < 0
        || snake_->headCollidesWithSelf();   // or if the snake collides with itself
}

void SnakeBoard::feedSnake()
{
    snake_->grow();
    placeNewFood();
}

bool SnakeBoard::snakeAte()
{
    return snake_->headCollidesWith(foodCoords_);
}

void SnakeBoard::setSnakeDirection(Snake::Direction d)
{
    snake_->setDirectionIfPossible(d);
}

void SnakeBoard::displayGameOverMsg(SDL_Renderer *renderer)
{
    if (game_over_msg_font_ != nullptr)
    {
        //TTF_SetFontHinting(game_over_msg_font_, TTF_HINTING_NORMAL);

        SDL_Rect msg_rect
                {
                        SnakeGame::WINDOW_WIDTH/4, // x
                        SnakeGame::WINDOW_HEIGHT/4,  // y
                        SnakeGame::WINDOW_WIDTH/2,   // width
                        SnakeGame::WINDOW_HEIGHT/2   // height
                };

        displayMsg(renderer, game_over_msg_texture_, msg_rect);

        SDL_Delay(2000);
    }
}

void SnakeBoard::displayMsg(SDL_Renderer *renderer, SDL_Texture *msg_texture, SDL_Rect &msg_rect)
{
    SDL_RenderCopy(renderer, msg_texture, nullptr, &msg_rect);
    SDL_RenderPresent(renderer);
}

void SnakeBoard::displayScore(SDL_Renderer *renderer)
{
    if (score_msg_font_ != nullptr)
    {
        SDL_Rect msg_rect
                {
                        0, // x
                        0,  // y
                        SnakeGame::WINDOW_WIDTH/COLUMNS * 3,   // width
                        SnakeGame::WINDOW_HEIGHT/ROWS * 1   // height
                };


        if (score_msg_surface_ != nullptr)
        {
            SDL_FreeSurface(score_msg_surface_);
        }

        if (score_msg_texture_ != nullptr)
        {
            SDL_DestroyTexture(score_msg_texture_);
        }

        score_msg_surface_ = TTF_RenderText_Blended(score_msg_font_, "Score: ", FONT_COLOR);
        score_msg_texture_ = SDL_CreateTextureFromSurface(renderer, score_msg_surface_);
        displayMsg(renderer, score_msg_texture_, msg_rect);

        //SDL_Delay(2000);
    }
}

void SnakeBoard::drawGrid(SDL_Renderer *renderer) const
{
    SDL_SetRenderDrawColor(renderer, GRID_LINE_COLOR.r, GRID_LINE_COLOR.g, GRID_LINE_COLOR.b, GRID_LINE_COLOR.a);

    for (int r = 1; r < ROWS; ++r) // start from 1 to avoid drawing grid lines around the edge of the window
    {
        SDL_RenderDrawLine(renderer,
                           0,                                     // left x
                           r * (SnakeGame::WINDOW_HEIGHT / ROWS), // left y
                           SnakeGame::WINDOW_WIDTH,               // right x
                           r * (SnakeGame::WINDOW_HEIGHT / ROWS)  // right y
        );
    }

    for (int c = 1; c < COLUMNS; ++c)
    {
        SDL_RenderDrawLine(renderer,
                           c * (SnakeGame::WINDOW_WIDTH / COLUMNS), // top x
                           0,                                       // top y
                           c * (SnakeGame::WINDOW_WIDTH) / COLUMNS, // bottom x
                           SnakeGame::WINDOW_HEIGHT                 // bottom y
        );
    }
}

void SnakeBoard::drawFood(SDL_Renderer *renderer) const
{
    drawBlock(renderer, FOOD_COLOR, foodCoords_.first, foodCoords_.second);
}

