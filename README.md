# Physical Activity Tracking Bracelet

## Description
This project involves designing an embedded system simulating a connected bracelet capable of monitoring several physiological parameters in real-time:

- **Body temperature**  
- **Heart rate (BPM)**  
- **Step count**  

The system is based on **Arduino Uno** and simulated in **Proteus**.

## System Architecture
The device uses:

- **LM35** → temperature measurement  
- **Pulse Generator** → heart rate simulation  
- **Potentiometer** → movement (steps) simulation  
- **LCD 16x2** → data display  
- **Arduino UNO** → signal processing  

## Operation
- Each detected pulse simulates a heartbeat.  
- An analog threshold simulates a step.  
- Measurements are displayed in real-time on the LCD.  
- An alert is generated if BPM < 50 or > 120.

## Tools Used
- **Arduino IDE**  
- **Proteus**  

## Project Structure
- `hardware/` → Proteus schematics and simulation  
- `firmware/` → Arduino code  
- `libraries/` → Used libraries  
- `docs/` → Project report
