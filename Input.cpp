#include <Arduino.h>
#include "constants.h"

class InputManager {
private:
    int xCenter;
    int yCenter;
    unsigned long lastDebounceTime;
    int lastButtonState;
    int buttonState;
    const int DEADZONE = 150;
    
public:
    InputManager() {
        xCenter = 512;
        yCenter = 512;
        lastDebounceTime = 0;
        lastButtonState = HIGH;
        buttonState = HIGH;
    }
    
    void begin() {
        pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);
        calibrate();
    }
    
    void calibrate() {
        long sumX = 0;
        long sumY = 0;
        
        for (int i = 0; i < 10; i++) {
            sumX += analogRead(JOYSTICK_X_PIN);
            sumY += analogRead(JOYSTICK_Y_PIN);
            delay(10);
        }
        
        xCenter = sumX / 10;
        yCenter = sumY / 10;
    }
    
    Direction getDirection() {
        int xValue = analogRead(JOYSTICK_X_PIN);
        int yValue = analogRead(JOYSTICK_Y_PIN);
        
        int xDiff = xValue - xCenter;
        int yDiff = yValue - yCenter;
        
        if (abs(xDiff) > abs(yDiff)) {
            if (abs(xDiff) > DEADZONE) {
                return (xDiff > 0) ? DIR_RIGHT : DIR_LEFT;
            }
        } else {
            if (abs(yDiff) > DEADZONE) {
                return (yDiff > 0) ? DIR_DOWN : DIR_UP;
            }
        }
        
        return DIR_NONE;
    }
    
    bool isButtonPressed() {
        int reading = digitalRead(JOYSTICK_SW_PIN);
        
        if (reading != lastButtonState) {
            lastDebounceTime = millis();
        }
        
        if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE) {
            if (reading != buttonState) {
                buttonState = reading;
                if (buttonState == LOW) {
                    lastButtonState = reading;
                    return true;
                }
            }
        }
        
        lastButtonState = reading;
        return false;
    }
    
    void waitForButtonPress() {
        while (digitalRead(JOYSTICK_SW_PIN) == HIGH) {
            delay(10);
        }
        delay(BUTTON_DEBOUNCE);
        while (digitalRead(JOYSTICK_SW_PIN) == LOW) {
            delay(10);
        }
    }
};
