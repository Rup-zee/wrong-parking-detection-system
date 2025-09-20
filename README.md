# wrong-parking-detection-system
This project is a smart parking monitoring system that detects vehicles and checks whether they are correctly parked inside a parking slot using ESP32, ultrasonic sensors, and an IR sensor. It provides real-time feedback via LEDs and a web interface hosted on the ESP32.
# 🚗 Wrong Parking Management System using ESP32, IR & Ultrasonic Sensors

A **smart parking monitoring system** built using **ESP32**, **ultrasonic sensors**, and an **IR sensor** to detect vehicles and verify whether they are parked correctly.  
The system gives **real-time feedback** via LEDs and a **web dashboard** hosted on ESP32.

---

## ✨ Features
- **Vehicle Detection** – IR sensor identifies if a car is present.
- **Correct Parking Check** – Dual ultrasonic sensors measure left/right distances and calculate total width.
- **LED Indicators**
  - ✅ **Green LED** → Correctly Parked  
  - ❌ **Red LED** → Wrongly Parked  
- **Web Dashboard** – View parking status from any device connected to the same Wi-Fi network.
- **Serial Debugging** – View live distance data and status in Serial Monitor.

---

## 🛠️ Hardware Requirements
- ESP32 Development Board  
- 2 × Ultrasonic Sensors (HC-SR04)  
- 1 × IR Sensor Module  
- 1 × Green LED, 1 × Red LED  
- Breadboard / PCB  
- Jumper Wires  

---

## ⚙️ Circuit Connections

| Component        | ESP32 Pin |
|------------------|-----------|
| Ultrasonic (Left) TRIG | GPIO 5 |
| Ultrasonic (Left) ECHO | GPIO 18 |
| Ultrasonic (Right) TRIG | GPIO 15 |
| Ultrasonic (Right) ECHO | GPIO 4 |
| IR Sensor Output | GPIO 19 |
| Green LED        | GPIO 2 |
| Red LED          | GPIO 21 |

---

## 📜 How It Works
1. **Detect Car Presence:**  
   The IR sensor checks if a vehicle is present in the slot.
2. **Measure Distances:**  
   Left and right ultrasonic sensors measure distances from the car sides to slot boundaries.
3. **Validate Parking:**  
   - If total width is between **5–8 cm** → **Correctly Parked** ✅  
   - If outside range → **Wrongly Parked** ❌  
   - If no car → LEDs turn off
4. **Display Results:**  
   - LEDs indicate parking status in real time  
   - Web dashboard shows live status via Wi-Fi  

---

## 🌐 Web Interface Preview
The ESP32 hosts a lightweight HTML page with the current parking status:

