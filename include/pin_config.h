#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// ESP32 Hardware Pin Assignments for Smart Lock System
#define SOLENOID_RELAY_PIN  26  // High-active relay for lock solenoid
#define BUZZER_PIN          27  // Piezo buzzer for audio feedback
#define STATUS_LED_GREEN    14  // Unlock indicator LED
#define STATUS_LED_RED      12  // Lock / Access Denied LED

#define OLED_SDA            21  // SSD1306 OLED Display SDA
#define OLED_SCL            22  // SSD1306 OLED Display SCL

#define MANUAL_OVERRIDE_BTN  4  // Tactile button for manual inside unlock

#endif // PIN_CONFIG_H
