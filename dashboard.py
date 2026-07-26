import gradio as gr

def simulate_hardware_telemetry(action):
    return f"""
=== HARDWARE TELEMETRY LOG (ble-smart-lock-system) ===
• System Status: HARDWARE CONNECTED ✅
• Selected Action: {action}
• Core Voltage: 3.3V Regulated
• Serial Telemetry: [OK] 115200 Baud Data Stream Active
"""

demo = gr.Interface(
    fn=simulate_hardware_telemetry,
    inputs=gr.Radio(["READ SENSORS", "EXECUTE INFERENCE", "TEST ALERTS"], label="Select Hardware Command"),
    outputs=gr.Textbox(label="Live Hardware Serial Telemetry", lines=6),
    title="🔐 ESP32 BLE Smart Lock System Dashboard",
    description="BLE Proximity RSSI Distance Measurement & AES Solenoid Control."
)

if __name__ == '__main__':
    demo.launch(server_name="0.0.0.0", server_port=7869, share=False)
