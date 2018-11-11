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
    do
    {
        if (!paused_ && SDL_GetTicks() > time_ + DELAY)
        {
            update();
            if (board_->isGameOver())
            {
                gameOver();
            }
            time_ = SDL_GetTicks();
        }
    } while(checkUserInput());
}

bool SnakeGame::checkUserInput()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_KEYDOWN:
            {
                // If the game is paused, ignore all user input except unpause
                if (paused_ && e.key.keysym.sym == SDLK_p)
                {
                    paused_ = false;
                }
                else if (!paused_)
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_UP:
                            board_->setSnakeDirection(Snake::UP);
                            break;
                        case SDLK_DOWN:
                            board_->setSnakeDirection(Snake::DOWN);
                            break;
                        case SDLK_LEFT:
                            board_->setSnakeDirection(Snake::LEFT);
                            break;
                        case SDLK_RIGHT:
                            board_->setSnakeDirection(Snake::RIGHT);
                            break;
                        case SDLK_p:
                            paused_ = true;
                            break;
                    }
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