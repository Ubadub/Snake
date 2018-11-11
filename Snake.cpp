#include <iostream>
#include "Snake.h"
#include "SnakeBoard.h"

SDL_Color Snake::SNAKE_COLOR = {0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE}; // green

Snake::Snake(int row, int col) : Snake(UP, row, col) {}

Snake::Snake(Snake::Direction d, int row, int col) :
head_(new Block(row, col)),
tail_(head_),
toGrow_(0),
direction_(d)
{}

Snake::~Snake()
{
    Block *block = tail_;

    while (block != nullptr)
    {
        Block *prev = block->getPrev();
        delete block;
        block = prev;
    }
}

bool Snake::collidesWith(std::pair<int, int> coords) const
{
    Block *block = head_;

    while (block != nullptr)
    {
        if (block->collidesWith(coords))
            return true;
        block = block->getNext_();
    }

    return false;
}

void Snake::draw(SDL_Renderer *renderer) const
{
    Block *block = head_;

    while (block != nullptr)
    {
        SnakeBoard::drawBlock(renderer, Snake::SNAKE_COLOR, block->getRow(), block->getColumn());
        block = block->getNext_();
    }
}

int Snake::getHeadRow() const
{
    return head_->getRow();
}

int Snake::getHeadColumn() const
{
    return head_->getColumn();
}

void Snake::grow()
{
    grow(1);
}

void Snake::grow(int n)
{
    toGrow_ += n;
}

bool Snake::headCollidesWith(std::pair<int, int> coords) const
{
    return head_->collidesWith(coords);
}

bool Snake::headCollidesWithSelf() const
{
    Block *b = tail_;
    while (b != head_)
    {
        if (b->collidesWith(*head_))
            return true;

        b = b->getPrev();
    }
    return false;
}

void Snake::move()
{
    newHead();
    if (toGrow_) // if we need to grow the Snake, we do so by not deleting the tail
    {
        toGrow_--;
    }
    else
    {
        deleteTail();
    }
}

bool Snake::setDirectionIfPossible(Snake::Direction d)
{
    if ((direction_ == UP   && d == DOWN)  || (direction_ == DOWN  && d == UP)
    ||  (direction_ == LEFT && d == RIGHT) || (direction_ == RIGHT && d == LEFT))
    {
        return false;
    }
    else
    {
        direction_ = d;
        return true;
    }
}

int Snake::size() const
{
    return head_->size();
}

void Snake::deleteTail()
{
    Block *oldTail = tail_;
    tail_ = tail_->getPrev();
    delete oldTail;
}

std::pair<int, int> Snake::getNewRowAndColumn()
{
    int r = head_->getRow();
    int c = head_->getColumn();

    switch (direction_)
    {
        case UP:
            r -= 1;
            break;
        case DOWN:
            r += 1;
            break;
        case LEFT:
            c -= 1;
            break;
        case RIGHT:
            c += 1;
            break;
    }

    return std::make_pair(r, c);
}

void Snake::newHead()
{
    std::pair<int, int> coords = getNewRowAndColumn();
    auto *newHead = new Block(coords.first, coords.second, head_);
    head_->setPrev(newHead);
    head_ = newHead;
}

Snake::Block::Block(int row, int col) : Block(row, col, nullptr) {}

Snake::Block::Block(int row, int col, Block *next):
        row_(row),
        col_(col),
        prev_(nullptr),
        next_(next)
{}

Snake::Block::~Block()
{
    if (prev_ != nullptr)
    {
        prev_->next_ = nullptr;
    }
    if (next_ != nullptr)
    {
        next_->prev_ = nullptr;
    }
}

// Accessors
int Snake::Block::getRow() const
{
    return row_;
}

int Snake::Block::getColumn() const
{
    return col_;
}

Snake::Block* Snake::Block::getPrev() const
{
    return prev_;
}

/**
 * Get the Block ahead of this one (i.e. the one immediately closer to the tail).
 *
 * @return a pointer to the Block which comes after this one.
 */
Snake::Block* Snake::Block::getNext_() const
{
    return next_;
}


void Snake::Block::setPrev(Snake::Block *const pBlock)
{
    prev_ = pBlock;
}

void Snake::Block::setNext(Snake::Block *const pBlock)
{
    next_ = pBlock;
}

bool Snake::Block::collidesWith(std::pair<int, int> coords) const
{
    return (row_ == coords.first && col_ == coords.second);
}

bool Snake::Block::collidesWith(const Block& otherBlock) const
{
    return (row_ == otherBlock.row_ && col_ == otherBlock.col_);
}

int Snake::Block::size() const
{
    if (next_ == nullptr)
        return 1;
    else
        return 1 + next_->size();
}


