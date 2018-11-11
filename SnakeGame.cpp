#include <SDL2_ttf/SDL_ttf.h>
#include "SnakeGame.h"

const SDL_Color SnakeGame::FONT_COLOR = {255, 255, 255}; // white
const char SnakeGame::FONT_LOCATION[] = "resources/fonts/lato/Lato-Bold.ttf";

SnakeGame::SnakeGame():
board_(new SnakeBoard),
paused_(false),
time_(SDL_GetTicks()),
score_(0)
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
        std::cout << "Failed to load game over font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
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
        score_msg_surface_ = nullptr;
        score_msg_texture_ = nullptr;
    }

    srand(time(NULL));
}

SnakeGame::~SnakeGame()
{
    delete board_;

    TTF_CloseFont(game_over_msg_font_);
    SDL_FreeSurface(game_over_msg_surface_);
    SDL_DestroyTexture(game_over_msg_texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
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

void SnakeGame::displayGameOverMsg()
{
    if (game_over_msg_font_ != nullptr)
    {
        //TTF_SetFontHinting(game_over_msg_font_, TTF_HINTING_NORMAL);

        SDL_Rect msg_rect
        {
            WINDOW_WIDTH/4, // x
            WINDOW_HEIGHT/4,  // y
            WINDOW_WIDTH/2,   // width
            WINDOW_HEIGHT/2   // height
        };

        displayMsg(game_over_msg_texture_, msg_rect);

        SDL_Delay(2000);
    }
}

void SnakeGame::displayMsg(SDL_Texture *msg_texture, SDL_Rect &msg_rect)
{
    SDL_RenderCopy(renderer_, msg_texture, nullptr, &msg_rect);
    SDL_RenderPresent(renderer_);
}

void SnakeGame::displayScore()
{
    if (score_msg_font_ != nullptr)
    {
        SDL_Rect msg_rect
        {
            0, // x
            0,  // y
            WINDOW_WIDTH/SnakeBoard::COLUMNS * 3,   // width
            WINDOW_HEIGHT/SnakeBoard::ROWS * 1   // height
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
        score_msg_texture_ = SDL_CreateTextureFromSurface(renderer_, score_msg_surface_);
        displayMsg(score_msg_texture_, msg_rect);

        //SDL_Delay(2000);
    }
}

void SnakeGame::gameOver()
{
    board_->displayGameOverMsg(renderer_);
    delete board_;
    board_ = new SnakeBoard;

    //displayGameOverMsg();
}

void SnakeGame::update()
{
    if (board_->snakeAte())
    {
        board_->feedSnake();
        score_++;
    }
    board_->advanceSnake();

    board_->draw(renderer_);
    //displayScore();
}