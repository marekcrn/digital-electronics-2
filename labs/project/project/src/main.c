/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 3)
 *     D7   - PD7 (Data bit 2)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

/* Defines ----------------------------------------------------------*/ 
#define VRX PC0 
#define VRY PC1   
#define SW PD2 
#define DT PD1   
#define CLK PD0
*/

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");  // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");  // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");  // Put button name here

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));
    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;
    // Convert "value" to "string" and display it
    itoa(value, string, 10);
    lcd_gotoxy(8, 0);
    lcd_puts("      ");
    lcd_gotoxy(8, 0);
    lcd_puts(string);

    itoa(value, string, 16);
    lcd_gotoxy(13, 0);
    lcd_puts("      ");
    lcd_gotoxy(13, 0);
    lcd_puts(string);

    if ((value == 0x3ff) | (value == 0x3fe))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("NONE");
    }
    else if ((value == 0x27f) | (value == 0x280))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("SELECT");
    }
    else if ((value == 0x199) | (value == 0x19a))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("LEFT");
    }
    else if ((value == 0x63) | (value == 0x62))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("UP");
    }
    else if ((value == 0x100) | (value == 0x101))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("DOWN");
    }
    if ((value == 0x0f))
    {
        lcd_gotoxy(8, 1);
        lcd_puts("      ");
        lcd_gotoxy(8, 1);
        lcd_puts("RIGHT");
    }
}