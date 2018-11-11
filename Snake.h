#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <utility>
#include <SDL2/SDL.h>

class Snake
{
public:
    /**
     * An enum representing the four directions in which a Snake can travel.
     */
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Snake(int row, int col);

    Snake(Direction, int row, int col);

    ~Snake();

    bool collidesWith(std::pair<int, int>) const;

    void draw(SDL_Renderer *) const;

    int getHeadRow() const;

    int getHeadColumn() const;

    void grow();

    void grow(int);

    bool headCollidesWith(std::pair<int, int>) const;

    bool headCollidesWithSelf() const;

    void move();

    bool setDirectionIfPossible(Direction);

    int size() const;

private:
    class Block;

    static SDL_Color SNAKE_COLOR;

    Block *head_;
    Block *tail_;
    int toGrow_;
    Direction direction_;

    void deleteTail();

    std::pair<int, int> getNewRowAndColumn();

    void newHead();

    class Block
    {
    public:
        Block(int row, int col);

        Block(int row, int col, Block *next);

        ~Block();

        // Accessors
        int getRow() const;
        int getColumn() const;

        Block *getPrev() const;

        /**
         * Get the SnakeBlock ahead of this one (i.e. the one immediately closer to the tail).
         * @return a pointer to the SnakeBlock which comes after this one.
         */
        Block *getNext_() const;

        void setPrev(Block *const pBlock);

        void setNext(Block *const pBlock);

        /**
         * Checks if this Block collides with the given coordinates.
         *
         * @param the coordinates (row, column) to check
         * @return true if this Block overlaps with the given coordinates, or false otherwise
         */
        bool collidesWith(std::pair<int, int>) const;

        bool collidesWith(const Block& otherBlock) const;

        int size() const;

    private:
        // The 0th row is at the top of the window
        int row_;
        // The 0th column is at the left of the window
        int col_;

        // prev_ is the Block immediately closer to the head
        Block *prev_;

        // next_ is the Block immediately closer to the tail
        Block *next_;
    };
};


#endif //SNAKE_SNAKE_H
