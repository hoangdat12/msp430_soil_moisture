#define BAUDRATE 9600

void UART_init() {
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;
}

void UART_sendString(char* str) {
    while (*str != '\0') {
        while (!(IFG2 & UCA0TXIFG)); // Ch? d?n khi TX buffer r?ng
        UCA0TXBUF = *str; // G?i ký t?
        str++;
    }
}

void UART_sendCommand(char* cmd) {
    UART_sendString(cmd); // G?i l?nh AT
    UART_sendString("\r\n"); // G?i d?u xu?ng dòng
}

void connectToWiFi() {
    // G?i các l?nh AT d? thi?t l?p k?t n?i WiFi
    UART_sendCommand("AT+RST"); // Kh?i d?ng l?i ESP8266
    __delay_cycles(1000000); // Delay 1s
    UART_sendCommand("AT+CWMODE=1"); // Ch? d? cài d?t là Station mode (1)
    __delay_cycles(1000000); // Delay 1s
    UART_sendCommand("AT+CWJAP=\"your_ssid\",\"your_password\""); // K?t n?i vào m?ng WiFi
    __delay_cycles(5000000); // Delay 5s (d?i cho k?t n?i)
}