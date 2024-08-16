#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/MCAL/PWM/pwm.h"
#include "fan.h"
#include "avr/io.h"

// Initialize the fan control module
void Fan_Init(void) {
    // Initialize the PWM with a duty cycle of 0 to start the fan at a stopped state
    PWM_Timer0_Init(0);
}

// Set the fan speed based on the duty cycle (0-255)
void Fan_SetSpeed(uint8_t speed) {
    // Adjust the PWM duty cycle to control the fan speed
    PWM_Timer0_Init(speed);
}

// Stop the fan
void Fan_Stop(void) {
    // Set the duty cycle to 0 to stop the fan
    Fan_SetSpeed(0);
}
