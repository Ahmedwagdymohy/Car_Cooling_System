#ifndef FAN_CONTROL_H_
#define FAN_CONTROL_H_

#include <stdint.h>

// Function prototypes
void Fan_Init(void);
void Fan_SetSpeed(uint8_t speed);
void Fan_Stop(void);

#endif /* FAN_CONTROL_H_ */
