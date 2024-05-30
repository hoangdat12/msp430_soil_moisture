#include "led.h"

#define EXTERNAL_LED BIT5 // If automate mode => turn on led, else turn off
#define LED_1 BIT0

void init_led()
{
    P1DIR |= LED_1; // Set P1.0 to output direction
    P1OUT |= LED_1; // Turn on led
    
    P2DIR |= EXTERNAL_LED; // Set P1.0 to output direction
    P2OUT |= EXTERNAL_LED; // Turn on led
}

void turn_on_led()
{
  P1OUT &= ~LED_1;       // Bat led o tren kit
}

void turn_off_led()
{
  P1OUT |= LED_1;      //toggle LED 
}

void turn_on_external_led()
{
  P2OUT |= EXTERNAL_LED;  // Bat led khi o che do automate 
}

void turn_off_external_led()
{
  P2OUT &= ~EXTERNAL_LED; // Tat led khi o che do manual
}