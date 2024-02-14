#include <Arduino.h>
#include "ble_security.h"

#define RELAY_PIN 23

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Lock engaged by default
  
  Serial.println("[BLE SMART LOCK] Initializing ESP32 NimBLE GATT Server...");
  Serial.println("[SECURITY] AES-128 Proximity Authentication Enabled.");
}

void loop() {
  // Simulated authentication event
  delay(3000);
  Serial.println("[BLE] Client connected. Performing challenge handshake...");
  if (BLESecurityAuth::verifyToken("SECURE_AUTH_PASS_KEY_9981")) {
    Serial.println("🔓 [AUTHENTICATED] Unlocking solenoid relay for 5 seconds.");
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("🔒 Lock re-engaged.");
  }
}
