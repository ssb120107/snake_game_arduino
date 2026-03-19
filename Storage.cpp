#include <Arduino.h>
#include <EEPROM.h>

class StorageManager {
private:
    const int EEPROM_ADDR = 0;  // Address for high score (uses 2 bytes)
    
public:
    StorageManager() {}
    
    void begin() {
        // For Arduino Uno/Nano, EEPROM doesn't need begin()
    }
    
    uint16_t loadHighScore() {
        uint16_t highScore = 0;
        
        // Read two bytes from EEPROM
        highScore = EEPROM.read(EEPROM_ADDR);
        highScore |= (EEPROM.read(EEPROM_ADDR + 1) << 8);
        
        // Validate (if EEPROM is uninitialized or corrupted)
        if (highScore > 1000) {
            highScore = 0;
        }
        
        return highScore;
    }
    
    bool saveHighScore(uint16_t score) {
        uint16_t currentHigh = loadHighScore();
        
        // Only save if new score is higher
        if (score > currentHigh) {
            EEPROM.write(EEPROM_ADDR, score & 0xFF);           // Low byte
            EEPROM.write(EEPROM_ADDR + 1, (score >> 8) & 0xFF); // High byte
            return true;
        }
        
        return false;
    }
    
    void resetHighScore() {
        // Reset to 0
        EEPROM.write(EEPROM_ADDR, 0);
        EEPROM.write(EEPROM_ADDR + 1, 0);
    }
};
