#include "sensor.h"


void init_sensor(void) {
 
  ADC10CTL0 = REFON + ADC10SHT_2 + ADC10ON;  // Bat nguon tham chieu dien ap ADC,  Chon Xung MCLK, Bat ADC
 
  __delay_cycles(30);           // Tao do tre de tham chieu dien ap on dinh
  
  ADC10CTL1 = INCH_5;           // Chon kenh dau vao la INCH_5 (port 1.5);
  
  ADC10AE0 |= BIT5;             // Bat A5 Cho ADC
}

int get_soil_moisture(void) {
  ADC10CTL0 |= ENC + ADC10SC; // Bat va bat dau chuyen doi
  
  __delay_cycles(1000);
  
  return (unsigned long) (99 - (ADC10MEM - 450) / 6.4);
}
