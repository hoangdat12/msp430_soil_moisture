#include "msp430g2553.h"
#include "i2c_lcd.h"
#include "sensor.h"

int SOIL_MOISTURE;
int MOISTURE_THRESHOLD = 70;

#define SWITCH_1 BIT3
// #define SWITCH_2 BIT0 
#define LED_1 BIT0

void welcome()
{
    LCD_Init(0X27, 2, 16);//khoi tao LCD voi giao thuc i2c
    LCD_backlightOn();//cho phep bat den nen
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
 
     LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("Nhom 13");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("Chao Cac Ban");//in chuoi ra mang hinh
    __delay_cycles(2000000);
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("Chu de");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("He thong tuoi cay td");//in chuoi ra mang hinh
    __delay_cycles(2000000);
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
    LCD_Print("Welcome");//in chuoi ra mang hinh
    LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
    LCD_Print("Lady and Gentilement");//in chuoi ra mang hinh
    __delay_cycles(2000000);
}

void init_switch()
{
    P1REN |= SWITCH_1; //enable pull mode 
    P1OUT |= SWITCH_1; //pull up 
    P1IES |= SWITCH_1; //triggers when button pressed, high to low 
    P1IE |= SWITCH_1; //enable interrupt 
}

void init_led()
{
    P1DIR |= LED_1; // Set P1.0 to output direction
    P1OUT &= ~LED_1; // Turn off led
}

void main(void)
{
    WDTCTL =  WDTPW + WDTHOLD;
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    init_switch();
    
    init_led();
    
    welcome();
    
    init_sensor();
    
    LCD_Clear();//clear mang hinh de xoa ky tu vo dinh
    
     __enable_interrupt();

    
    while(1){
        LCD_SetCursor(0,0);//set vi tri lcd (cot, hang)
          //LCD_Print("Threshold: %s", SOIL_MOISTURE);//in chuoi ra mang  
        lcd_put_num(MOISTURE_THRESHOLD,0,0);//chuyen so thuc sang chuoi va in ra mang hinh    
        
        SOIL_MOISTURE = get_soil_moisture();
       
        LCD_SetCursor(0,1);//set vi tri lcd (cot, hang)
        lcd_put_num(SOIL_MOISTURE,0,0);//chuyen so thuc sang chuoi va in ra mang hinh
    
        __delay_cycles(100000);
        
        if (SOIL_MOISTURE > MOISTURE_THRESHOLD)
        {
          P1OUT |= BIT0; //toggle LED 
          // Turn on Reley
        }
        else {
          P1OUT &= ~BIT0; //toggle LED 
          // Turn of Reley
        }
    }
}

#pragma vector=PORT1_VECTOR // Ng?t t? port 1
__interrupt void Port_1(void) {
    if (P1IFG & SWITCH_1) {
        if (MOISTURE_THRESHOLD < 100)
        {
           MOISTURE_THRESHOLD++;
        }
        P1IFG &= ~SWITCH_1; // Xóa c? ng?t c?a SW1
    }
}