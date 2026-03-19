#include <Arduino.h>
#include "constants.h"

class AudioManager {
private:
    bool enabled;
    const int NOTE_FOOD = 1000;
    const int NOTE_GAME_OVER = 200;
    const int NOTE_NEW_HIGH = 1200;
    
public:
    AudioManager() {
        enabled = true;
    }
    
    void begin() {
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, LOW);
    }
    
    void enable(bool en) { 
        enabled = en; 
    }
    
    void playFoodEaten() {
        if (!enabled) return;
        tone(BUZZER_PIN, NOTE_FOOD, 50);
        delay(60);
        noTone(BUZZER_PIN);
    }
    
    void playGameOver() {
        if (!enabled) return;
        tone(BUZZER_PIN, NOTE_GAME_OVER, 300);
        delay(350);
        tone(BUZZER_PIN, NOTE_GAME_OVER / 2, 500);
        delay(550);
        noTone(BUZZER_PIN);
    }
    
    void playNewHighScore() {
        if (!enabled) return;
        for (int i = 0; i < 3; i++) {
            tone(BUZZER_PIN, NOTE_NEW_HIGH, 100);
            delay(120);
            tone(BUZZER_PIN, NOTE_NEW_HIGH * 2, 100);
            delay(120);
        }
        noTone(BUZZER_PIN);
    }
};
