/*
 ================================================================================================
 Name        : MC2.c
 Author      : Mohamed Tarek
 Description : MC2 Code in Challenge
 Date        : 25/4/2014
 ================================================================================================
 */

#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/MCAL/ADC/adc.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/HAL/Fan/fan.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/HAL/push_button/push_button.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/MCAL/UART/uart.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/MCAL/GPIO/gpio.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/MCU2/MCAL/PWM/pwm.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/LIB/common_macros.h"
#include "C:/Users/study/OneDrive/Documents/Eclipse_ws/Cooling_System2/LIB/std_types.h"
#include <util/delay.h>





#define NORMAL_STATE_COMMAND       0x01
#define EMERGENCY_STATE_COMMAND    0x02
#define ABNORMAL_STATE_COMMAND     0x03
#define MAX_FAN_SPEED_COMMAND      0x04
#define STOP_FAN_COMMAND           0x05
#define ADJUST_FAN_SPEED_COMMAND   0x06





/* Declare the LM35_getTemperature function */
uint8 LM35_getTemperature(void);

/* Define the GPIO pins for the components */
#define GREEN_LED_PORT PORTC_ID
#define GREEN_LED_PIN  PIN0_ID

#define YELLOW_LED_PORT PORTC_ID
#define YELLOW_LED_PIN  PIN1_ID

#define RED_LED_PORT PORTC_ID
#define RED_LED_PIN  PIN2_ID

#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN  PIN3_ID

#define SERVO_PORT PORTD_ID
#define SERVO_PIN  PIN5_ID

#define MACHINE_PORT PORTD_ID
#define MACHINE_PIN  PIN6_ID

#define POTENTIOMETER_CHANNEL_ID  0

/* Function Prototypes */
void MCU2_init(void);
void handleNormalState(uint8 temperature);
void handleEmergencyState(void);
void handleAbnormalState(void);

/* Global Variables */
uint8 received_command = 0;

void MCU2_init(void) {
    /* Initialize UART for communication with MCU1 */
    UART_init(9600);

    /* Initialize GPIO pins for LEDs */
    GPIO_setupPinDirection(GREEN_LED_PORT, GREEN_LED_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(YELLOW_LED_PORT, YELLOW_LED_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(RED_LED_PORT, RED_LED_PIN, PIN_OUTPUT);

    /* Initialize GPIO pin for Buzzer */
    GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

    /* Initialize GPIO pin for Servo Motor */
    GPIO_setupPinDirection(SERVO_PORT, SERVO_PIN, PIN_OUTPUT);

    /* Initialize GPIO pin for Machine (DC Motor) */
    GPIO_setupPinDirection(MACHINE_PORT, MACHINE_PIN, PIN_OUTPUT);

    /* Initialize ADC for Potentiometer */
    ADC_init();
}

void handleNormalState(uint8 temperature) {
    if (temperature < 20) {
        /* Turn off fan on MCU1 by sending command */
        UART_sendByte(STOP_FAN_COMMAND);
        /* Light Green LED */
        GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_HIGH);
        GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
        GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
    } else if (temperature < 40) {
        /* Adjust fan speed based on temperature */
        UART_sendByte(ADJUST_FAN_SPEED_COMMAND);
        /* Light Yellow LED */
        GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
        GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_HIGH);
        GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
    } else if (temperature < 50) {
        /* Run fan at max speed */
        UART_sendByte(MAX_FAN_SPEED_COMMAND);
        /* Light Red LED */
        GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
        GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
        GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
        /* Stop the machine */
        GPIO_writePin(MACHINE_PORT, MACHINE_PIN, LOGIC_LOW);
    }
}

void handleEmergencyState(void) {
    /* Light Red LED and Run Buzzer */
    GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
    GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);

    /* Run fan at max speed */
    UART_sendByte(MAX_FAN_SPEED_COMMAND);

    /* Stop the machine */
    GPIO_writePin(MACHINE_PORT, MACHINE_PIN, LOGIC_LOW);
}

void handleAbnormalState(void) {
    /* Light Red LED */
    GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);

    /* Open servo motor to 90 degrees */
    GPIO_writePin(SERVO_PORT, SERVO_PIN, LOGIC_HIGH); // Assuming the servo motor is driven by a PWM signal

    /* Run Buzzer */
    GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);

    /* Stop the machine */
    GPIO_writePin(MACHINE_PORT, MACHINE_PIN, LOGIC_LOW);
}

int main(void) {
    /* Initialize MCU2 */
    MCU2_init();

    /* Main loop */
    while (1) {
        /* Receive the command from MCU1 */
        received_command = UART_recieveByte();

        switch (received_command) {
            case NORMAL_STATE_COMMAND:
                handleNormalState(LM35_getTemperature());
                break;
            case EMERGENCY_STATE_COMMAND:
                handleEmergencyState();
                break;
            case ABNORMAL_STATE_COMMAND:
                handleAbnormalState();
                break;
            default:
                /* Do nothing */
                break;
        }

        /* Control machine speed using potentiometer */
        uint16 pot_value = ADC_readChannel(POTENTIOMETER_CHANNEL_ID);
        PWM_Timer0_Init(pot_value); // Assuming PWM is used to control machine speed
    }
    return 0;
}
