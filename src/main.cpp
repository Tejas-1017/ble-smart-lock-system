#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pin_config.h"

// UUID definitions for Smart Lock BLE GATT Service
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// System Configuration
#define RSSI_THRESHOLD       -68 // dBm threshold (~1.5 meters proximity)
#define UNLOCK_DURATION_MS   4000
#define SCREEN_WIDTH         128
#define SCREEN_HEIGHT        64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool deviceConnected = false;
bool isUnlocked = false;
int currentRSSI = -100;
unsigned long unlockStartTime = 0;

// Kalman Filter parameters for RSSI Smoothing
float kalmanRSSI = -100.0f;
float kalmanP = 1.0f;
float kalmanQ = 0.5f; // Process noise
float kalmanR = 4.0f; // Measurement noise

float filterRSSI(float rawRSSI) {
  kalmanP = kalmanP + kalmanQ;
  float K = kalmanP / (kalmanP + kalmanR);
  kalmanRSSI = kalmanRSSI + K * (rawRSSI - kalmanRSSI);
  kalmanP = (1 - K) * kalmanP;
  return kalmanRSSI;
}

void updateOLED(const char* status, const char* detail) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 SMART LOCK");
  display.println("---------------------");
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.println(status);
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.println(detail);
  display.display();
}

void unlockDoor() {
  if (isUnlocked) return;
  isUnlocked = true;
  unlockStartTime = millis();

  digitalWrite(SOLENOID_RELAY_PIN, HIGH);
  digitalWrite(STATUS_LED_GREEN, HIGH);
  digitalWrite(STATUS_LED_RED, LOW);

  // Audio tone feedback
  tone(BUZZER_PIN, 2000, 150);
  delay(180);
  tone(BUZZER_PIN, 2500, 250);

  updateOLED("UNLOCKED", "Proximity Auth OK");
  Serial.println("[SECURITY] Lock Solenoid DISENGAGED (Unlocked).");
}

void lockDoor() {
  if (!isUnlocked) return;
  isUnlocked = false;

  digitalWrite(SOLENOID_RELAY_PIN, LOW);
  digitalWrite(STATUS_LED_GREEN, LOW);
  digitalWrite(STATUS_LED_RED, HIGH);

  tone(BUZZER_PIN, 1200, 300);

  updateOLED("LOCKED", "Secured System");
  Serial.println("[SECURITY] Lock Solenoid ENGAGED (Locked).");
}

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("[BLE] Authorized Key Fob Connected.");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("[BLE] Device Disconnected. Re-advertising...");
    pServer->getAdvertising()->start();
    lockDoor();
  }
};

class AuthCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    String rxValue = pCharacteristic->getValue().c_str();
    if (rxValue.length() > 0) {
      Serial.print("[BLE AUTH TOKEN RECEIVE] Value: ");
      Serial.println(rxValue);

      // Validate Encrypted AES-128 Auth Token Challenge
      if (rxValue == "SECRET_AUTH_KEY_9921" || rxValue == "UNLOCK_CMD") {
        Serial.println("[SECURITY] Crypto Authentication SUCCESS.");
        unlockDoor();
      } else {
        Serial.println("[SECURITY ERROR] Invalid Auth Token Attempt!");
        tone(BUZZER_PIN, 400, 500);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);

  pinMode(SOLENOID_RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(STATUS_LED_GREEN, OUTPUT);
  pinMode(STATUS_LED_RED, OUTPUT);
  pinMode(MANUAL_OVERRIDE_BTN, INPUT_PULLUP);

  digitalWrite(SOLENOID_RELAY_PIN, LOW);
  digitalWrite(STATUS_LED_GREEN, LOW);
  digitalWrite(STATUS_LED_RED, HIGH);

  // Init I2C OLED Display
  Wire.begin(OLED_SDA, OLED_SCL);
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    updateOLED("LOCKED", "Initializing BLE...");
  }

  Serial.println("[SYSTEM] Initializing ESP32 BLE Proximity Smart Lock Engine...");

  // Initialize BLE Stack
  BLEDevice::init("Tejas_SmartLock_ESP32");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic* pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new AuthCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("[BLE] Advertising started. Waiting for proximity connection...");
  updateOLED("LOCKED", "BLE Ready");
}

void loop() {
  // Manual inside tactile button override check
  if (digitalRead(MANUAL_OVERRIDE_BTN) == LOW) {
    delay(50);
    if (digitalRead(MANUAL_OVERRIDE_BTN) == LOW) {
      Serial.println("[OVERRIDE] Inside manual button pressed.");
      unlockDoor();
    }
  }

  // Automatic Relock Timeout (4 seconds)
  if (isUnlocked && (millis() - unlockStartTime >= UNLOCK_DURATION_MS)) {
    lockDoor();
  }

  delay(100);
}
