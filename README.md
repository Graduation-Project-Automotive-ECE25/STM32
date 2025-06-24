# 🚗 STM32F103C6T6 - ADAS Control Firmware

This repository contains the complete firmware developed for the STM32F103C6T6 microcontroller as part of an Advanced Driver Assistance System (ADAS) graduation project. It features a custom Real-Time Operating System (RTOS) and supports several key ADAS features essential to modern automotive safety systems.

---

## 📦 Features

- **🛑 Automatic Emergency Braking (AEB)**  
  Real-time obstacle detection using ultrasonic sensors with active braking control.

- **🕶️ Blind Spot Detection**  
  Left-side detection with LED indicators using threshold-based distance logic.

- **🚘 Adaptive Cruise Control (ACC)**  
  Maintains vehicle speed and safe following distance using PWM and ultrasonic feedback.

- **📍 GPS Location Sharing**  
  Transmits location data via UART to a Raspberry Pi for GUI visualization.

- **📡 Bluetooth-Based Vehicle Control**  
  Supports manual movement commands like forward, backward, left, and right.

- **⚙️ Custom RTOS**  
  Lightweight real-time scheduler supporting task creation, prioritization, waiting, and termination.

---

## 📁 Project Structure

```text
STM32/
├── RTOS/
│   ├── Src/
│   │   ├── CortexMx_OS_porting.c   # ARM-specific low-level context switching
│   │   ├── Scheduler.c             # Task creation, states, and scheduling
│   │   └── MY_RTOS_FIFO.c          # FIFO structure for managing ready tasks
│   └── Inc/
│       ├── CortexMx_OS_porting.h
│       ├── Scheduler.h
│       └── MY_RTOS_FIFO.h
│
├── Drivers/
│   ├── ACC/
│   │   ├── ACC.c                   # Adaptive Cruise Control implementation
│   │   └── ACC.h
│   ├── BlindSpot&AEB/
│   │   ├── BlindSpot&AEB.c         # Blind spot detection & emergency braking
│   │   └── BlindSpot&AEB.h
│   ├── GPS/
│   │   ├── GPS.c                   # GPS data transmission logic
│   │   └── GPS.h
│   ├── GPIO/
│   │   ├── GPIO.c                  # GPIO pin configuration and control
│   │   └── GPIO.h
│   ├── RCC/
│   │   ├── RCC.c                   # Reset and clock control
│   │   └── RCC.h
│   └── UART/
│       ├── UART.c                  # UART configuration and data transmission
│       └── UART.h
│
├── main.c                          # Main application: initializes hardware, creates and starts tasks
```




---

## 🛠️ Technologies Used

### Hardware

* **Microcontroller:** STM32F103C6T6 (ARM Cortex-M3)

* **Sensors:** Ultrasonic Sensors (HC-SR04)

* **Communication Modules:** GPS Module (NEO-6M), Bluetooth Module Bluetooth (HC-05)

* **Actuators:** DC Motors (controlled via PWM)

### Software

* **Development Environment:** STM32CubeIDE

* **Programming Language:** C

* **Operating System:** Custom Real-Time Operating System (RTOS) (written from scratch)

* **Communication Protocols:** UART (for GPS, Bluetooth)

* **Peripherals:** GPIO, TIM (Timers)
---

## 🚦 System Overview

### 🧠 Task Scheduling

- The RTOS supports:
  - Task creation with a defined stack size and priority
  - Task waiting (delay-based blocking)
  - Cooperative multitasking with PendSV-based context switching

### 📡 Communication Flow

| Module       | Direction | Protocol | Target           |
|--------------|-----------|----------|------------------|
| GPS          | →         | UART     | Raspberry Pi 4   |
| Speed Data   | →         | UART     | Raspberry Pi GUI |
| Bluetooth    | ←         | UART     | Mobile App       |
| Ultrasonic   | -         | GPIO + Timer Input Capture | MCU-internal |

---

## 🚀 Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/Graduation-Project-Automotive-ECE25/STM32.git
```

### 2. Open with STM32CubeIDE

Import the project into **STM32CubeIDE** and flash it to your **STM32F103C6T6** development board.

### 3. Hardware Setup

- Connect **ultrasonic sensors** to appropriate GPIO pins  
- Connect **GPS module** to **USART1**  
- Connect **motor driver** (H-Bridge) to **PWM** output channels  
- Pair **Bluetooth module** via **USART2**

---

## 🧠 Main Tasks in `main.c`

| Task Name               | Description                                     |
|------------------------|-------------------------------------------------|
| `GPS()`                | Sends GPS coordinates over UART periodically    |
| `UltraSonic_Triggering()` | Triggers all ultrasonic sensors at fixed intervals |
| `BlindSpot_and_AEB()`  | Processes ultrasonic readings for blind spot & emergency braking logic |
| `move()`               | Responds to Bluetooth commands and adjusts motor speed/direction accordingly |
