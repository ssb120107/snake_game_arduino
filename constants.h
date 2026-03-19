#ifndef CONSTANTS_H    
#define CONSTANTS_H 
#include <Arduino.h>


// OLED Display (I2C)
#define OLED_ADDRESS    0x3D
#define OLED_SDA        A4    
#define OLED_SCL        A5

// Joystick Module --(joystick X and Y axis ,analog)
#define JOYSTICK_X_PIN  A0        
#define JOYSTICK_Y_PIN  A1       
#define JOYSTICK_SW_PIN 2         

// Buzzer--passive buzzer for sound effects
#define BUZZER_PIN      3

// LEDs
#define LED_RED_PIN     5
#define LED_GREEN_PIN   6
#define LED_BLUE_PIN    7

        //Game constants

// Display dimensions
const int SCREEN_WIDTH    = 128;       // OLED width in pixels
const int SCREEN_HEIGHT   = 64;        // OLED height in pixels

// Game area (inside borders)
const int GAME_AREA_X = 16;        // Left margin
const int GAME_AREA_Y = 16;        // Top margin
const int GAME_AREA_WIDTH = 96;        // Play area width
const int GAME_AREA_HEIGHT= 48;       // Play area height

// Grid size (each cell is 4x4 pixels)
const int CELL_SIZE = 4;
const int GRID_WIDTH = (GAME_AREA_WIDTH / CELL_SIZE);   // 24 cells
const int GRID_HEIGHT = (GAME_AREA_HEIGHT / CELL_SIZE);  // 12 cells

// Snake limits
const int MAX_SNAKE_LENGTH = 100;  // Maximum snake length

// Game timing
const int SPLASH_DURATION = 2000;      // 2 seconds splash screen
const int MOVE_DELAY = 200;       // 200ms between moves (medium speed)
const int GAME_OVER_DELAY = 3000;      // 3 seconds game over screen
const int BUTTON_DEBOUNCE = 50;        // 50ms button debounce

// Direction enum-- creating a custom datatype with fixed set of names values.
/*
             1(Up)
    3(Left)        4(Right)
            2(Down)
*/
enum Direction {
    DIR_NONE = 0,          
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

enum GameState {
    STATE_SPLASH,       //0
    STATE_GAMEPLAY,     //1
    STATE_GAME_OVER     //2
};

// Point structure for positions
struct Point {
    int8_t x;                     // Grid X coordinate (0-23),24 cells
    int8_t y;                 // Grid Y coordinate (0-11),11 cells
};

#endif