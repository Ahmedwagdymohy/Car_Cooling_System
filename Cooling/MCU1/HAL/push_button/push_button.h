#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

#include <stdint.h>

// Constants for button state
#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 0

// Function prototypes
void PushButton_Init(void);
uint8_t PushButton_Read(void);

#endif /* PUSH_BUTTON_H_ */
