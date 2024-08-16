#include "push_button.h"  // Include the push button header file
#include "C:/Users/study/OneDrive/Desktop/Cooling_system/LIB/common_macros.h"
#include "avr/io.h"
#include <util/delay.h>  // For debouncing delay

// Initialize the push button module
void PushButton_Init(void) {
    // Set the push button pin as input
    CLEAR_BIT(DDRD, PD2); // Assuming PD2 is the push button pin
    // Enable the internal pull-up resistor
    SET_BIT(PORTD, PD2);
}

// Read the push button state with debouncing
uint8_t PushButton_Read(void) {
    // Read the button state
    if (BIT_IS_CLEAR(PIND, PD2)) {
        // Debouncing: wait for a short period and check again
        _delay_ms(20);
        if (BIT_IS_CLEAR(PIND, PD2)) {
            return BUTTON_PRESSED;
        }
    }
    return BUTTON_RELEASED;
}
