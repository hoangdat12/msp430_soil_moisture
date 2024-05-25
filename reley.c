#include "reley.h"

#define RELAY_PIN BIT4
#define LED_1 BIT0

void init_reley(void)
{
  P1DIR |= RELAY_PIN;
  P1OUT &= ~RELAY_PIN;
}

void turn_on_reley(void)
{
  P1OUT |= LED_1; //toggle LED 
  P1OUT |= RELAY_PIN; // Turn on Reley
}

void turn_off_reley(void)
{
  P1OUT &= ~LED_1; //toggle LED 
  P1OUT &= ~RELAY_PIN; // Turn of Reley
}