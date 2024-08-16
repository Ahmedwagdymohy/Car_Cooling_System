/*
 ================================================================================================
 Name        : MC1.c
 Author      : Mohamed Tarek
 Description : MC1 Code in Challenge
 Date        : 25/4/2014
 ================================================================================================
 */


#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/MCAL/ADC/adc.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/HAL/Fan/fan.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/HAL/push_button/push_button.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/MCAL/UART/uart.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/MCAL/GPIO/gpio.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/MCU1/MCAL/PWM/pwm.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/LIB/common_macros.h"
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/LIB/std_types.h"
#include <util/delay.h> /* For the delay functions */




#define NORMAL_STATE      0
#define EMERGENCY_STATE   1
#define ABNORMAL_STATE    2

#define TEMPERATURE_THRESHOLD_1 20
#define TEMPERATURE_THRESHOLD_2 40
#define TEMPERATURE_THRESHOLD_3 50

#define FAN_MAX_SPEED     255
#define FAN_MIN_SPEED     0

#define SHUTDOWN_CODE     0x01

void APP_Init(void);
void APP_Run(void);
void HandleFanControl(uint8 temperature);
void HandleStateTransition(uint8 temperature);

int main(void)
{
    APP_Init();
    APP_Run();
}

void APP_Init(void)
{
    ADC_init();            // Initialize the ADC
    Fan_Init();            // Initialize the fan (PWM)
    PushButton_Init();     // Initialize the push button
    UART_init(9600);       // Initialize UART with baud rate 9600
}

void APP_Run(void)
{
    uint8 temperature;
    uint8 state = NORMAL_STATE;

    // Read the previous state from EEPROM
    //state = EEPROM_readState();

    while(1)
    {
        temperature = LM35_getTemperature();

        // Handle fan control based on temperature
        HandleFanControl(temperature);

        // Handle state transitions based on temperature and button press
        HandleStateTransition(temperature);

        // Check if the push button is pressed and temperature is in the shutdown range
        if(PushButton_Read() == BUTTON_PRESSED &&
           temperature >= TEMPERATURE_THRESHOLD_2 &&
           temperature < TEMPERATURE_THRESHOLD_3)
        {
            // Send shutdown code to MCU2
            UART_sendByte(SHUTDOWN_CODE);
        }
    }
}

void HandleFanControl(uint8 temperature)
{
    uint8 fan_speed = FAN_MIN_SPEED;

    if(temperature < TEMPERATURE_THRESHOLD_1)
    {
        Fan_Stop();  // Stop the fan
    }
    else if(temperature < TEMPERATURE_THRESHOLD_2)
    {
        // Calculate fan speed proportional to temperature
        fan_speed = ((uint32)(temperature - TEMPERATURE_THRESHOLD_1) * FAN_MAX_SPEED) /
                    (TEMPERATURE_THRESHOLD_2 - TEMPERATURE_THRESHOLD_1);
        Fan_SetSpeed(fan_speed);
    }
    else if(temperature < TEMPERATURE_THRESHOLD_3)
    {
        Fan_SetSpeed(FAN_MAX_SPEED);  // Set fan to maximum speed
    }
    else
    {
        // In case of temperature >= 50°C
        Fan_SetSpeed(FAN_MAX_SPEED);  // Emergency state, max speed
    }
}

void HandleStateTransition(uint8 temperature)
{
    static uint8 state = NORMAL_STATE;
    static uint16 emergency_counter = 0;

    switch(state)
    {
        case NORMAL_STATE:
            if(temperature >= TEMPERATURE_THRESHOLD_3)
            {
                state = EMERGENCY_STATE;
                emergency_counter = 0;  // Reset emergency counter
            }
            break;

        case EMERGENCY_STATE:
            emergency_counter++;
            if(emergency_counter >= 14)  // Check temperature 14 times with 500ms delay (7 seconds)
            {
                //EEPROM_writeState(ABNORMAL_STATE);  // Write abnormal state to EEPROM
                // Activate watchdog timer to reset MCU1
               // WatchdogTimer_Enable();
                state = ABNORMAL_STATE;
            }
            else if(temperature < TEMPERATURE_THRESHOLD_3)
            {
                state = NORMAL_STATE;  // Return to normal if temperature drops below 50°C
            }
            break;

        case ABNORMAL_STATE:
            // Keep the fan at max speed and remain in abnormal state
            Fan_SetSpeed(FAN_MAX_SPEED);
            break;

        default:
            break;
    }
}
