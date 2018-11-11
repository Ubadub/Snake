#include <SDL2_ttf/SDL_ttf.h>
#include "SnakeGame.h"

SnakeGame::SnakeGame():
board_(new SnakeBoard),
paused_(false),
time_(SDL_GetTicks()),
score_(0)
{
    srand(time(NULL));

    board_->draw();
}

SnakeGame::~SnakeGame()
{
    delete board_;
}

void SnakeGame::loop()
{
    Snake::Direction d = Snake::DIRECTION_NONE;
    do
    {
        if (!paused_ && SDL_GetTicks() > time_ + DELAY)
        {
            if (d != Snake::DIRECTION_NONE)
            {
                board_->setSnakeDirection(d);
            }
            update();
            if (board_->isGameOver())
            {
                gameOver();
            }
            time_ = SDL_GetTicks();
        }
    } while(checkUserInput(d));
}

bool SnakeGame::checkUserInput(Snake::Direction &d)
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_KEYDOWN:
            {
                if (!paused_)
                {

                    switch (e.key.keysym.sym)
                    {
                        case SDLK_UP:
                            d = Snake::DIRECTION_UP;
                            break;
                        case SDLK_DOWN:
                            d = Snake::DIRECTION_DOWN;
                            break;
                        case SDLK_LEFT:
                            d = Snake::DIRECTION_LEFT;
                            break;
                        case SDLK_RIGHT:
                            d = Snake::DIRECTION_RIGHT;
                            break;
                        case SDLK_p:
                            paused_ = true;
                            break;
                    }
                }
                // If the game is paused, ignore all user input except unpause
                else
                {
                    paused_ = e.key.keysym.sym != SDLK_p;
                }
            }
            break;

            case SDL_QUIT:
                return false;

           default:
               break;
       }
    }

    return true;
}

void SnakeGame::gameOver()
{
    board_->displayGameOverMsg();
    board_->reset();
}

void SnakeGame::update()
{
    if (board_->snakeAte())
    {
        board_->feedSnake();
        score_++;
    }
    board_->advanceSnake();
    board_->draw();
}