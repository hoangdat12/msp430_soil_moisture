#include "msp430g2553.h"
#include "reley.h"
#include <stdbool.h>
#include "i2c_lcd.h"

void init_switch(void);

// Function to handle increasing the moisture threshold or starting manual mode
void handle_click_sw_increase_start(bool IS_AUTOMATE, int *MOISTURE_THRESHOLD, int *IS_ENABLE_MANUAL);

// Function to handle decreasing the moisture threshold or stopping manual mode
void handle_click_sw_decrease_stop(bool IS_AUTOMATE, int *MOISTURE_THRESHOLD, int *IS_ENABLE_MANUAL);

// Function to handle changing the mode (automate/manual)
void handle_change_mode(bool *IS_AUTOMATE, int *IS_ENABLE_MANUAL);


