#include "reley.h"

#define RELAY_PIN BIT4

void init_reley(void)
{
  P1DIR |= RELAY_PIN;   // Cau hinh p1.4 lam dau ra 
  P1OUT &= ~RELAY_PIN;  // Dat chan p1.4 o muc thap
}

void turn_on_reley(void)
{
  P1OUT |= RELAY_PIN;   // Turn on Reley
}

void turn_off_reley(void)
{
  P1OUT &= ~RELAY_PIN;  // Turn of Reley
}