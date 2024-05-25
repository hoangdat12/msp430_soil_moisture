#include "msp430g2553.h"
#include "i2c_lcd.h"
#include "sensor.h"
#include <stdbool.h>
#include "reley.h"
#include "switches.h"
#include <string.h>

int SOIL_MOISTURE;
int MOISTURE_THRESHOLD = 70;
int MOISTURE_THRESHOLD_MIN = 50;
int MOISTURE_THRESHOLD_MAX = 100;
bool IS_AUTOMATE = true; // Change mode
int IS_ENABLE_MANUAL = 0; // Start/stop when IS_AUTOMATE = 0 (manual)

#define SWITCH_1 BIT3
#define LED_1 BIT0
#define SW_INCREASE_START BIT2
#define SW_DECREASE_STOP BIT3
#define SW_CHANGE_MODE BIT4
#define EXTERNAL_LED BIT5 // If automate mode => turn on led, else turn off

void welcome()
{
    LCD_Init(0X27, 2, 16);//khoi tao LCD voi giao thuc i2c
    LCD_backlightOn();//cho phep bat den nen
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
 
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("MON HOC");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("KT VI XU LY");//in chuoi ra mang hinh
    __delay_cycles(3000000);
    
     LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("NHOM 13");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("CHAO MOI NGUOI");//in chuoi ra mang hinh
    __delay_cycles(3000000);
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("CHU DE");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("HT TUOI CAY");//in chuoi ra mang hinh
    __delay_cycles(3000000);
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("GVHD");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("HO VIET VIET");//in chuoi ra mang hinh
    __delay_cycles(3000000);
   
}

void init_led()
{
    P1DIR |= LED_1; // Set P1.0 to output direction
    P1OUT &= ~LED_1; // Turn off led
    
    P2DIR |= EXTERNAL_LED; // Set P1.0 to output direction
    P2OUT |= EXTERNAL_LED; // Turn on led
}

void init_serial()
{
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // *Initialize USCI state machine*
    IE2 |= UCA0RXIE;
}

 
void main(void)
{
    WDTCTL =  WDTPW + WDTHOLD;
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
   
    init_serial();
    
    init_led();
    
    welcome();
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    
    init_sensor();
    
    init_reley();
    
    init_switch();
    
     __enable_interrupt();


    
    while(1){
        SOIL_MOISTURE = get_soil_moisture();
       
        LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
        lcd_put_num(SOIL_MOISTURE,0,0);//chuyen so thuc sang chuoi va in ra mang hinh
        
        
        LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
        lcd_put_num(MOISTURE_THRESHOLD,0,0);//chuyen so thuc sang chuoi va in ra mang 
       
        if (IS_AUTOMATE)
        {
          P2OUT |= EXTERNAL_LED; // Turn on led
          handle_automate_water(SOIL_MOISTURE, MOISTURE_THRESHOLD);
          LCD_SetCursor(15,0);//set vi tri lcd (cot, hang)
          LCD_Print("A");//in chuoi ra mang hinh
        } 
        else {
          handle_manual_water(IS_ENABLE_MANUAL);
          P2OUT &= ~EXTERNAL_LED; // Turn off led
          LCD_SetCursor(15,0);//set vi tri lcd (cot, hang)
          LCD_Print("M");//in chuoi ra mang hinh
        }
    }
}

#pragma vector=PORT2_VECTOR // Interrupt vector for Port 2
__interrupt void Port_2(void) {
    if (P2IFG & SW_INCREASE_START) {
        handle_click_sw_increase_start(IS_AUTOMATE, &MOISTURE_THRESHOLD, &IS_ENABLE_MANUAL);
        P2IFG &= ~SW_INCREASE_START; // Clear the interrupt flag for SW_INCREASE_START
    } 
    else if (P2IFG & SW_DECREASE_STOP) {
        handle_click_sw_decrease_stop(IS_AUTOMATE, &MOISTURE_THRESHOLD, &IS_ENABLE_MANUAL);
        P2IFG &= ~SW_DECREASE_STOP; // Clear the interrupt flag for SW_DECREASE_STOP
    }
    else if (P2IFG & SW_CHANGE_MODE) {
        handle_change_mode(&IS_AUTOMATE, &IS_ENABLE_MANUAL);
        P2IFG &= ~SW_CHANGE_MODE; // Clear the interrupt flag for SW_CHANGE_MODE
    }
}