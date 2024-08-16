# Cooling System

This project demonstrates a system implemented using two AVR microcontrollers (MCU1 and MCU2) to manage various states based on temperature readings and user inputs. The system controls a fan, a machine, LEDs, a buzzer, and a servo motor, simulating an environment that reacts to temperature changes and user interactions.

## System Overview

### MCU1 Components:
![image](https://github.com/user-attachments/assets/49cbdc66-aa3e-48cf-afb1-dea95b40f4d2)

- **Push Button**
- **Temperature Sensor**
- **DC Motor (as Fan)**

### MCU2 Components:
![image](https://github.com/user-attachments/assets/0e33184c-6d1b-4fa9-b13f-399baa9a1cd8)

- **Servo Motor**
- **3 LEDs**
- **Potentiometer**
- **DC Motor (as Machine)**
- **Buzzer**

### System Description:

#### MCU1:
1. **EEPROM State Management:** 
   - Reads the previous state from EEPROM before assessing the temperature to determine if it's an abnormal state.
   
2. **Temperature Reading and Fan Control:** 
   - Reads the temperature and initially stores the state as normal in EEPROM.
   - Controls the fan (DC motor) speed based on the state.
   - Sends temperature data to MCU2 via UART.

3. **Push Button Interaction:** 
   - If the user presses the push button when the temperature is between 40°C and 50°C, MCU1 sends a shutdown command to MCU2 via UART.

#### MCU2:
1. **UART Communication:** 
   - Receives messages from MCU1 and acts accordingly.

2. **Potentiometer Control:** 
   - Controls the speed of the machine (DC motor) using the potentiometer.

### States:

1. **Normal State:**
   - **< 20°C:** Fan stops; MCU2 lights green LED.
   - **20°C - 40°C:** Fan speed increases with temperature; MCU2 lights yellow LED.
   - **40°C - 50°C:** Fan runs at max speed; MCU2 lights red LED and stops the machine.
   - **> 50°C:** Emergency state is triggered.

2. **Emergency State:**
   - Fan runs at max speed.
   - MCU2 lights red LED and activates the buzzer.
   - Emergency state is activated for 7 seconds.

3. **Abnormal State:**
   - Fan runs at max speed.
   - MCU2 lights red LED, opens the servo to 90 degrees, stops the machine, and runs the buzzer.
   - If the emergency state persists for 7 seconds:
     1. Write abnormal state in EEPROM.
     2. Activate watchdog timer to reset MCU1.
     3. MCU2 follows the abnormal state for 5 seconds.

## Corner Cases to Demonstrate:

1. **Temperature Sensor and Fan Function:** 
   - Show the fan's response to temperature changes using an oscilloscope.
   
2. **Potentiometer and Machine Function:** 
   - Demonstrate the machine's speed control using an oscilloscope.
   
3. **Emergency State:** 
   - Show the system's response when the temperature exceeds 50°C.
   
4. **Push Button Function:** 
   - Demonstrate the effect of pressing the push button between 40°C and 50°C.
   
5. **Normal States:** 
   - Show system behavior in the normal temperature range.

6. **Emergency State Timeout:** 
   - Show the transition from emergency to abnormal state if the temperature decreases before 7 seconds.

## Getting Started

### Prerequisites
- AVR Microcontroller Development Board
- Programming Environment for AVR (e.g., Atmel Studio, AVR-GCC)

### Setup
1. Connect the components to the corresponding pins on MCU1 and MCU2.
2. Program MCU1 and MCU2 using the provided code.
3. Monitor the system's response using the oscilloscope and observe the behavior based on temperature changes and user inputs.

## Simulation 
![image](https://github.com/user-attachments/assets/71442d29-75cf-4a32-bcdc-fd88b373ba5f)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

