 /******************************************************************************
 *
 * Module: Common - Macros
 *
 * File Name: Common_Macros.h
 *
 * Description: Commonly used Macros
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef COMMON_MACROS
#define COMMON_MACROS

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num) ( REG= (REG>>num) | (REG<<(8-num)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(8-num)) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

#define GET_BIT(REG,BIT) ( ( REG & (1<<BIT) ) >> BIT )




// Define commands received from MCU1
#define MAX_FAN_SPEED_COMMAND      0xFF  // Maximum fan speed command from MCU1
#define SHUTDOWN_COMMAND           0x01  // Shutdown command when the temperature is between 40°C and 50°C
#define EMERGENCY_STATE_COMMAND    0x02  // Emergency state command
#define ABNORMAL_STATE_COMMAND     0x03  // Abnormal state command
#define NORMAL_STATE_COMMAND       0x04  // Normal state command



#endif
