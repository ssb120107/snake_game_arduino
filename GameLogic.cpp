#include <Arduino.h>
#include "constants.h"

class GameLogic {
private:
    Point snake[MAX_SNAKE_LENGTH];
    uint8_t snakeLength;
    Direction currentDir;
    Direction nextDir;
    Point food;
    uint16_t score;
    bool gameOver;
    
    bool isOpposite(Direction dir1, Direction dir2) {
        return (dir1 == DIR_UP && dir2 == DIR_DOWN) ||
               (dir1 == DIR_DOWN && dir2 == DIR_UP) ||
               (dir1 == DIR_LEFT && dir2 == DIR_RIGHT) ||
               (dir1 == DIR_RIGHT && dir2 == DIR_LEFT);
    }
    
    bool isPositionInSnake(int x, int y, int ignoreIndex = -1) {
        for (int i = 0; i < snakeLength; i++) {
            if (i == ignoreIndex) continue;
            if (snake[i].x == x && snake[i].y == y) {
                return true;
            }
        }
        return false;
    }
    
    void generateRandomFood() {
        for (int attempts = 0; attempts < 100; attempts++) {
            food.x = random(0, GRID_WIDTH);
            food.y = random(0, GRID_HEIGHT);
            
            if (!isPositionInSnake(food.x, food.y)) {
                return;
            }
        }
        
        // Linear scan if random fails
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (!isPositionInSnake(x, y)) {
                    food.x = x;
                    food.y = y;
                    return;
                }
            }
        }
        
        // Grid completely full - player wins! Reset game
        reset();
    }
    
    bool checkCollision() {
        Point& head = snake[snakeLength - 1];
        
        if (head.x < 0 || head.x >= GRID_WIDTH ||
            head.y < 0 || head.y >= GRID_HEIGHT) {
            return true;
        }
        
        for (int i = 0; i < snakeLength - 1; i++) {
            if (snake[i].x == head.x && snake[i].y == head.y) {
                return true;
            }
        }
        
        return false;
    }
    
public:
    GameLogic() {
        reset();
    }
    
    void reset() {
        snakeLength = 3;
        snake[0].x = 10; snake[0].y = 6;
        snake[1].x = 11; snake[1].y = 6;
        snake[2].x = 12; snake[2].y = 6;
        
        currentDir = DIR_RIGHT;
        nextDir = DIR_RIGHT;
        score = 0;
        gameOver = false;
        
        generateRandomFood();
    }
    
    void setDirection(Direction dir) {
        if (dir != DIR_NONE && !isOpposite(dir, currentDir)) {
            nextDir = dir;
        }
    }
    
    void update() {
        if (gameOver) return;
        
        if (nextDir != DIR_NONE) {
            currentDir = nextDir;
            nextDir = DIR_NONE;
        }
        
        Point newHead = snake[snakeLength - 1];
        
        switch (currentDir) {
            case DIR_UP:    newHead.y--; break;
            case DIR_DOWN:  newHead.y++; break;
            case DIR_LEFT:  newHead.x--; break;
            case DIR_RIGHT: newHead.x++; break;
            default: break;
        }
        
        bool foodEaten = (newHead.x == food.x && newHead.y == food.y);
        
        if (foodEaten) {
            snake[snakeLength] = newHead;
            snakeLength++;
            score++;
            generateRandomFood();
        } else {
            for (int i = 0; i < snakeLength - 1; i++) {
                snake[i] = snake[i + 1];
            }
            snake[snakeLength - 1] = newHead;
        }
        
        if (checkCollision()) {
            gameOver = true;
        }
    }
    
    const Point* getSnake() const { return snake; }
    uint8_t getSnakeLength() const { return snakeLength; }
    const Point& getFood() const { return food; }
    uint16_t getScore() const { return score; }
    bool isGameOver() const { return gameOver; }
};
