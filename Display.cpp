#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "constants.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    bool initialized;
    
    void drawBorder() {
        display.drawRect(
            GAME_AREA_X - 1, 
            GAME_AREA_Y - 1, 
            GAME_AREA_WIDTH + 2, 
            GAME_AREA_HEIGHT + 2, 
            SSD1306_WHITE
        );
    }
    
    void drawSnakeSegment(int gridX, int gridY, bool isHead) {
        int pixelX = GAME_AREA_X + (gridX * CELL_SIZE);
        int pixelY = GAME_AREA_Y + (gridY * CELL_SIZE);
        
        if (isHead) {
            display.fillRect(pixelX, pixelY, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
        } else {
            display.drawRect(pixelX, pixelY, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
        }
    }
    
    void drawFood(int gridX, int gridY) {
        int pixelX = GAME_AREA_X + (gridX * CELL_SIZE) + (CELL_SIZE / 2);
        int pixelY = GAME_AREA_Y + (gridY * CELL_SIZE) + (CELL_SIZE / 2);
        display.fillCircle(pixelX, pixelY, CELL_SIZE / 2, SSD1306_WHITE);
    }
    
public:
    DisplayManager() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {
        initialized = false;
    }
    
    bool begin() {
        Wire.begin();
        if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
            return false;
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.display();
        initialized = true;
        return true;
    }
    
    void clear() {
        if (!initialized) return;
        display.clearDisplay();
    }
    
    void update() {
        if (!initialized) return;
        display.display();
    }
    
    void drawSplash() {
        if (!initialized) return;
        clear();
        display.setTextSize(2);
        display.setCursor(20, 20);
        display.print(F("SNAKE"));
        display.setCursor(20, 40);
        display.print(F("GAME"));
        display.setTextSize(1);
        display.setCursor(30, 55);
        display.print(F("Version 1.0"));
        update();
    }
    
    void drawGameplay(
        const Point* snake,
        uint8_t snakeLength,
        const Point& food,
        uint16_t score,
        uint16_t highScore
    ) {
        if (!initialized) return;
        clear();
        
        display.setCursor(0, 0);
        display.print(F("S:"));
        display.print(score);
        display.setCursor(70, 0);
        display.print(F("H:"));
        display.print(highScore);
        
        drawBorder();
        drawFood(food.x, food.y);
        
        for (int i = snakeLength - 1; i >= 0; i--) {
            drawSnakeSegment(snake[i].x, snake[i].y, (i == snakeLength - 1));
        }
        
        update();
    }
    
    void drawGameOver(
        uint16_t score,
        uint16_t highScore,
        bool isNewHighScore
    ) {
        if (!initialized) return;
        clear();
        
        display.setTextSize(2);
        display.setCursor(15, 10);
        display.print(F("GAME OVER"));
        
        display.setTextSize(1);
        display.setCursor(30, 30);
        display.print(F("Score: "));
        display.print(score);
        display.setCursor(30, 40);
        display.print(F("High: "));
        display.print(highScore);
        
        if (isNewHighScore) {
            display.setCursor(20, 50);
            display.print(F("NEW HIGH SCORE!"));
        } else {
            display.setCursor(15, 50);
            display.print(F("Press to restart"));
        }
        
        update();
    }
    
    bool isReady() { return initialized; }
};