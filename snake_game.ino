#include "constants.h"

class DisplayManager;
class InputManager;
class GameLogic;
class StorageManager;
class AudioManager;


#include "Display.cpp"
#include "Input.cpp"
#include "GameLogic.cpp"
#include "Storage.cpp"
#include "Audio.cpp"

DisplayManager display;
InputManager input;
GameLogic game;
StorageManager storage;
AudioManager audio;


GameState currentState = STATE_SPLASH;
unsigned long stateStartTime = 0;
unsigned long lastMoveTime = 0;
uint16_t highScore = 0;
bool isNewHighScore = false;

void setup() {
    Serial.begin(9600);
    Serial.println(F("Snake Game Starting..."));
    
    randomSeed(analogRead(A3));
    
    input.begin();
    audio.begin();
    storage.begin();
    
    if (!display.begin()) {
        Serial.println(F("Display initialization failed!"));
    }
    
    highScore = storage.loadHighScore();
    
    // Start with splash screen
    currentState = STATE_SPLASH;
    stateStartTime = millis();
    display.drawSplash();
    
    Serial.println(F("System Ready!"));
}

void loop() {
    unsigned long currentTime = millis();
    
    switch (currentState) {
        case STATE_SPLASH:
            if (currentTime - stateStartTime >= SPLASH_DURATION) {
                game.reset();
                currentState = STATE_GAMEPLAY;
                lastMoveTime = currentTime;
                Serial.println(F("Starting game..."));
            }
            break;
            
        case STATE_GAMEPLAY:
            {
                Direction dir = input.getDirection();
                if (dir != DIR_NONE) {
                    game.setDirection(dir);
                }
                
                if (currentTime - lastMoveTime >= MOVE_DELAY) {
                    game.update();
                    lastMoveTime = currentTime;
                    
                    static uint16_t lastScore = 0;
                    if (game.getScore() > lastScore) {
                        audio.playFoodEaten();
                        lastScore = game.getScore();
                    }
                    
                    if (game.isGameOver()) {
                        currentState = STATE_GAME_OVER;
                        stateStartTime = currentTime;
                        
                        uint16_t finalScore = game.getScore();
                        if (finalScore > highScore) {
                            highScore = finalScore;
                            storage.saveHighScore(highScore);
                            isNewHighScore = true;
                            audio.playNewHighScore();
                        } else {
                            isNewHighScore = false;
                            audio.playGameOver();
                        }
                        
                        Serial.print(F("Game Over! Score: "));
                        Serial.println(finalScore);
                    }
                }
                
                display.drawGameplay(
                    game.getSnake(),
                    game.getSnakeLength(),
                    game.getFood(),
                    game.getScore(),
                    highScore
                );
            }
            break;
            
        case STATE_GAME_OVER:
            display.drawGameOver(
                game.getScore(),
                highScore,
                isNewHighScore
            );
            
            if (input.isButtonPressed() || 
                (currentTime - stateStartTime >= GAME_OVER_DELAY)) {
                
                game.reset();
                currentState = STATE_GAMEPLAY;
                lastMoveTime = currentTime;
                Serial.println(F("Restarting game..."));
            }
            break;
    }
    
    delay(10);
}