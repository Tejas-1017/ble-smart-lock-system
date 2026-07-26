/*
 * ESP32 BLE Proximity Smart Lock System Sketch
 * Author: Tejas Kharkar
 * Microcontroller: ESP32-WROOM-32
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "include/pin_config.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  pinMode(SOLENOID_RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(STATUS_LED_GREEN, OUTPUT);
  pinMode(STATUS_LED_RED, OUTPUT);

  digitalWrite(SOLENOID_RELAY_PIN, LOW);
  digitalWrite(STATUS_LED_RED, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("ESP32 SMART LOCK");
  display.println("LOCKED // BLE READY");
  display.display();

  Serial.println(F("[BLE SMART LOCK] Initializing NimBLE & Proximity RSSI Engine..."));
}

void loop() {
  delay(1000);
}
