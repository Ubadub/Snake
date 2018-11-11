#include <iostream>
#include <utility>

#include "SnakeGame.h"
#include "SnakeBoard.h"

const SDL_Color SnakeBoard::FOOD_COLOR = {0xFF, 0xFF, 0x00, SDL_ALPHA_OPAQUE}; // yellow
const SDL_Color SnakeBoard::FONT_COLOR = {255, 255, 255}; // white
const char SnakeBoard::FONT_LOCATION[] = "resources/fonts/lato/Lato-Bold.ttf";
const SDL_Color SnakeBoard::GRID_LINE_COLOR = {0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE}; // white

SnakeBoard::SnakeBoard()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error("SDL_INIT_VIDEO failed!");

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0 /*SDL_WINDOW_FULLSCREEN_DESKTOP*/, &window_, &renderer_);
    SDL_RenderSetLogicalSize(renderer_, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (TTF_Init() != 0)
        std::cout << "TTF failed to initialize! Error: " << TTF_GetError() << std::endl;

    game_over_msg_font_ = TTF_OpenFont(FONT_LOCATION, GAME_OVER_FONT_SIZE);

    if(game_over_msg_font_ == nullptr)
    {
        std::cout << "Failed to load game over font! SDL_ttf Error: " << TTF_GetError() << std::endl; }
    else
    {
        game_over_msg_surface_ = TTF_RenderText_Blended(game_over_msg_font_, "GAME OVER", FONT_COLOR);
        game_over_msg_texture_ = SDL_CreateTextureFromSurface(renderer_, game_over_msg_surface_);
    }

    score_msg_font_ = TTF_OpenFont(FONT_LOCATION, SCORE_FONT_SIZE);

    if(score_msg_font_ == nullptr)
    {
        std::cout << "Failed to load score font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    else
    {
        score_msg_surface_ = TTF_RenderText_Blended(score_msg_font_, "Score: ", FONT_COLOR);
        score_msg_texture_ = SDL_CreateTextureFromSurface(renderer_, score_msg_surface_);
    }

    makeSnake();
    placeNewFood();
}

SnakeBoard::~SnakeBoard()
{
    delete snake_;

    TTF_CloseFont(game_over_msg_font_);
    SDL_FreeSurface(game_over_msg_surface_);
    SDL_DestroyTexture(game_over_msg_texture_);

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}

void SnakeBoard::advanceSnake()
{
    snake_->move();
}

void SnakeBoard::displayGameOverMsg()
{
    if (game_over_msg_font_ != nullptr)
    {
        SDL_Rect msg_rect
        {
            WINDOW_WIDTH/4,  // x
            WINDOW_HEIGHT/4, // y
            WINDOW_WIDTH/2,  // width
            WINDOW_HEIGHT/2  // height
        };

        displayMsg(game_over_msg_texture_, msg_rect);

        SDL_Delay(2000);
    }
}

void SnakeBoard::draw()
{
    // first clear the board
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    drawGrid();
    drawFood();
    snake_->draw(renderer_);
    //displayScore();

    SDL_RenderPresent(renderer_);
}

void SnakeBoard::drawBlock(SDL_Renderer *renderer, SDL_Color color, int row, int col)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect block
            {
                    (WINDOW_WIDTH / COLUMNS) * col,
                    (WINDOW_HEIGHT / ROWS) * row,
                    WINDOW_WIDTH / COLUMNS,
                    WINDOW_HEIGHT / ROWS
            };

    SDL_RenderFillRect(renderer, &block);
}

void SnakeBoard::feedSnake()
{
    snake_->grow();
    placeNewFood();
}

bool SnakeBoard::isGameOver()
{
    int headRow = snake_->getHeadRow();
    int headCol = snake_->getHeadColumn();
    return headRow >= ROWS    || headRow < 0 // game over if out of bounds
           || headCol >= COLUMNS || headCol < 0
           || snake_->headCollidesWithSelf();   // or if the snake collides with itself
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

void SnakeBoard::reset()
{
    delete snake_;
    makeSnake();
}

void SnakeBoard::setSnakeDirection(Snake::Direction d)
{
    snake_->setDirectionIfPossible(d);
}

bool SnakeBoard::snakeAte()
{
    return snake_->headCollidesWith(foodCoords_);
}

void SnakeBoard::displayMsg(SDL_Texture *msg_texture, SDL_Rect &msg_rect)
{
    SDL_RenderCopy(renderer_, msg_texture, nullptr, &msg_rect);
    SDL_RenderPresent(renderer_);
}

void SnakeBoard::displayScore()
{
    if (score_msg_font_ != nullptr)
    {
        SDL_Rect msg_rect
        {
            0, // x
            0,  // y
            WINDOW_WIDTH/COLUMNS * 3, // width
            WINDOW_HEIGHT/ROWS        // height
        };

//        if (score_msg_surface_ != nullptr)
//        {
//            SDL_FreeSurface(score_msg_surface_);
//        }
//
//        if (score_msg_texture_ != nullptr)
//        {
//            SDL_DestroyTexture(score_msg_texture_);
//        }

        displayMsg(score_msg_texture_, msg_rect);

        //SDL_Delay(2000);
    }
}

void SnakeBoard::drawFood() const
{
    drawBlock(renderer_, FOOD_COLOR, foodCoords_.first, foodCoords_.second);
}

void SnakeBoard::drawGrid() const
{
    SDL_SetRenderDrawColor(renderer_, GRID_LINE_COLOR.r, GRID_LINE_COLOR.g, GRID_LINE_COLOR.b, GRID_LINE_COLOR.a);

    for (int r = 1; r < ROWS; ++r) // start from 1 to avoid drawing grid lines around the edge of the window
    {
        SDL_RenderDrawLine(renderer_,
                           0,                                     // left x
                           r * (WINDOW_HEIGHT / ROWS), // left y
                           WINDOW_WIDTH,               // right x
                           r * (WINDOW_HEIGHT / ROWS)  // right y
        );
    }

    for (int c = 1; c < COLUMNS; ++c)
    {
        SDL_RenderDrawLine(renderer_,
                           c * (WINDOW_WIDTH / COLUMNS), // top x
                           0,                                       // top y
                           c * (WINDOW_WIDTH) / COLUMNS, // bottom x
                           WINDOW_HEIGHT                 // bottom y
        );
    }
}

void SnakeBoard::makeSnake()
{
    snake_ = new Snake(ROWS / 2, COLUMNS / 2);
}