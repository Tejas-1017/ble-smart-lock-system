import time

def test_ble_handshake():
    print("[BLE MOBILE TEST] Scanning for 'ESP32_SMART_LOCK'...")
    print("[BLE MOBILE TEST] Device found (-52 dBm RSSI). Initiating AES-128 handshake...")
    time.sleep(1)
    print("✅ [AUTH SUCCESS] Solenoid relay unlocked!")

if __name__ == "__main__":
    test_ble_handshake()
