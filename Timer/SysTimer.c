/*
	*Ahmed Wagdy Mohy
	*8/16/2024



*/
#define SYSTICK_BASE_ADDRESS  0xE000E010

#define SYSTICK_CTRL          (*((volatile uint32_t*)(SYSTICK_BASE_ADDRESS + 0x00)))
#define SYSTICK_LOAD          (*((volatile uint32_t*)(SYSTICK_BASE_ADDRESS + 0x04)))
#define SYSTICK_VAL           (*((volatile uint32_t*)(SYSTICK_BASE_ADDRESS + 0x08)))
#define SYSTICK_CALIB         (*((volatile uint32_t*)(SYSTICK_BASE_ADDRESS + 0x0C)))

// SysTick Control and Status Register (STK_CTRL) Bits
#define SYSTICK_CTRL_ENABLE   (1 << 0)
#define SYSTICK_CTRL_TICKINT  (1 << 1)
#define SYSTICK_CTRL_CLKSOURCE (1 << 2)
#define SYSTICK_CTRL_COUNTFLAG (1 << 16)

// SysTick Reload Value Register (STK_LOAD) Max Value
#define SYSTICK_MAX_RELOAD    0x00FFFFFF


#include <stdint.h>

// SysTick Configuration Function
void SysTick_Start(uint32_t ticks) {
    if (ticks > SYSTICK_MAX_RELOAD) {
        ticks = SYSTICK_MAX_RELOAD;  // Ensure the reload value does not exceed the maximum
    }

    SYSTICK_LOAD = ticks - 1;        // Set reload register
    SYSTICK_VAL = 0;                 // Clear current value register

    SYSTICK_CTRL = SYSTICK_CTRL_ENABLE     // Enable SysTick Timer
                 | SYSTICK_CTRL_TICKINT    // Enable SysTick exception request
                 | SYSTICK_CTRL_CLKSOURCE; // Set clock source to processor clock
}

// Function to stop SysTick Timer
void SysTick_Stop(void) {
    SYSTICK_CTRL = 0; // Disable SysTick Timer
}
