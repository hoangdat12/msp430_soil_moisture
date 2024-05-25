#include "switches.h"

extern int MOISTURE_THRESHOLD_MIN;
extern int MOISTURE_THRESHOLD_MAX;

#define SW_INCREASE_START BIT2
#define SW_DECREASE_STOP BIT3
#define SW_CHANGE_MODE BIT4

void init_switch() {
    P2REN |= (SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE); // Enable pull-up/pull-down resistors
    P2OUT |= (SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE); // Pull-up resistors (set the output high)
    P2IES |= (SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE); // Interrupt edge select: high-to-low transition
    P2IFG &= ~(SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE); // Clear any existing interrupt flags
    P2IE |= (SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE);  // Enable interrupts for the switches
    P2DIR &= ~(SW_INCREASE_START | SW_DECREASE_STOP | SW_CHANGE_MODE); // Configure pins as input
}

void handle_click_sw_increase_start(bool IS_AUTOMATE, int *MOISTURE_THRESHOLD, int *IS_ENABLE_MANUAL)
{
    // Increase
    if (IS_AUTOMATE) 
    {
        if (*MOISTURE_THRESHOLD < MOISTURE_THRESHOLD_MAX)
        {
            *MOISTURE_THRESHOLD += 5;
        } 
        else 
        {
            *MOISTURE_THRESHOLD = 70;
            LCD_SetCursor(0,0);
            lcd_put_num(*MOISTURE_THRESHOLD, 0, 0);
        }
    } 
    // Start
    else 
    {
        *IS_ENABLE_MANUAL = 1;
    }
}

void handle_click_sw_decrease_stop(bool IS_AUTOMATE, int *MOISTURE_THRESHOLD, int *IS_ENABLE_MANUAL)
{
    // Decrease
    if (IS_AUTOMATE)
    {
        if (*MOISTURE_THRESHOLD > MOISTURE_THRESHOLD_MIN + 5)
        {
            *MOISTURE_THRESHOLD -= 5;
        } 
    }
    // Stop
    else 
    {
        *IS_ENABLE_MANUAL = 0;
    }
}

void handle_change_mode(bool *IS_AUTOMATE, int *IS_ENABLE_MANUAL)
{
    *IS_AUTOMATE = !(*IS_AUTOMATE);
    *IS_ENABLE_MANUAL = 0;
}

void handle_automate_water(int SOIL_MOISTURE, int MOISTURE_THRESHOLD)
{
    if (SOIL_MOISTURE > MOISTURE_THRESHOLD)
    {
        turn_on_reley();
    }
    else 
    {
        turn_off_reley();
    }
}

void handle_manual_water(int IS_ENABLE_MANUAL)
{
    if (!IS_ENABLE_MANUAL)
    {
        turn_on_reley();
    }
    else 
    {
        turn_off_reley();
    }
}
