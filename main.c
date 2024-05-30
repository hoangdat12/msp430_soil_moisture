#include "msp430g2553.h"
#include <stdbool.h>
#include <string.h>
#include "i2c_lcd.h"
#include "sensor.h"
#include "reley.h"
#include "switches.h"
#include "led.h"

int SOIL_MOISTURE;
int MOISTURE_THRESHOLD = 70;
int MOISTURE_THRESHOLD_MIN = 35;
int MOISTURE_THRESHOLD_MAX = 100;
bool IS_AUTOMATE = true; // Change mode
int IS_ENABLE_MANUAL = 0; // Start/stop when IS_AUTOMATE = 0 (manual)

#define SWITCH_1 BIT3
#define SW_INCREASE_START BIT2
#define SW_DECREASE_STOP BIT3
#define SW_CHANGE_MODE BIT4
#define EXTERNAL_LED BIT5 // If automate mode => turn on led, else turn off
#define LED_1 BIT0

void lcd_show_str(int row, int column, char str[])
{
    LCD_SetCursor(row,column);          //set vi tri lcd (cot, hang)
    LCD_Print(str);                     //in chuoi ra mang hinh
}

void lcd_show_num(int row, int column, int num)
{
    LCD_SetCursor(row, column);         //set vi tri lcd (cot, hang)
    lcd_put_num(num,0,0);               //chuyen so thuc sang chuoi va in ra mang hinh  
}

void lcd_welcome(char str1[], char str2[])
{
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    lcd_show_str(0,0, str1);            // In chu ra LCD        
    lcd_show_str(0,1, str2);            // In chu ra LCD
    __delay_cycles(3000000);
}

void welcome()
{
    LCD_Init(0X27, 2, 16);              //khoi tao LCD voi giao thuc i2c
    LCD_backlightOn();                  //cho phep bat den nen
    LCD_Clear();                        //clear mang hinh de xoa ky tu vo dinh
 
    lcd_welcome("MON HOC", "KT VI XU LY");
 
    lcd_welcome("NHOM 10", "CHAO MOI NGUOI");
    
    lcd_welcome("CHU DE", "HT TUOI CAY");
    
    lcd_welcome("GVHD", "HO VIET VIET");
   
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


void handle_automate_water(int SOIL_MOISTURE, int MOISTURE_THRESHOLD)
{
    turn_on_external_led();
    if (SOIL_MOISTURE > MOISTURE_THRESHOLD)
    {
        turn_on_reley();
        turn_on_led();
    }
    else 
    {
        turn_off_reley();
        turn_off_led();
    }
}

void handle_manual_water(int IS_ENABLE_MANUAL)
{
    turn_off_external_led();
    if (!IS_ENABLE_MANUAL)
    {
        turn_on_reley();
        turn_on_led();
    }
    else 
    {
        turn_off_reley();
        turn_off_led();
    }
}

 
void main(void)
{
    WDTCTL =  WDTPW + WDTHOLD;
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
   
    init_serial();
    
    init_led();
    
    welcome();
    
    LCD_Clear();                                //clear mang hinh de xoa ky tu vo dinh
    
    init_sensor();                              // Khoi tao Cam bien
      
    init_reley();                               // Khoi tao reley
    
    init_switch();                              // Khoi tao 3 nut bam
    
     __enable_interrupt();                      // Cho phep ngat


    
    while(1){
        SOIL_MOISTURE = get_soil_moisture();    // Doc do am tu cam bien        
       
        lcd_show_num(0, 0, MOISTURE_THRESHOLD); // Hien thi nguong do am hien tai
        
        lcd_show_num(0, 1, SOIL_MOISTURE);      // Hien thi do am do duoc
       
        if (IS_AUTOMATE)
        {
          handle_automate_water(SOIL_MOISTURE, MOISTURE_THRESHOLD); // Xu ly bat tat reley
          lcd_show_str(15, 0, "A");            // Hien thi Ky tu "A" o che do automate
        } 
        else {
          handle_manual_water(IS_ENABLE_MANUAL);// Xu ly bat tat reley
          lcd_show_str(15, 0, "M");             // Hien thi Ky tu "M" o che do Manual
        }
        
        // Deley man hinh khoi nhay
        if (60 <= SOIL_MOISTURE <= 70)
        {
          __delay_cycles(500000);
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