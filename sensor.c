
#include "sensor.h"

void init_sensor(void){
  ADC10CTL0 = REFON + ADC10SHT_2 + ADC10ON;
  __delay_cycles(30);
  ADC10CTL1 = INCH_5;
  ADC10AE0 |= BIT5;
}

int get_soil_moisture(void){
   ADC10CTL0 |= ENC + ADC10SC;
   __delay_cycles(1000);
   return (unsigned long) (100-(ADC10MEM - 450)/6.4);
}