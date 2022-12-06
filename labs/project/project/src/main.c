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

#define DT PD1   
#define CLK PD0
#define SW PD2

static uint16_t value = 0;
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
    lcd_init(LCD_DISP_ON_CURSOR_BLINK);

    uint8_t Play[24] = {
        0b00000,
        0b11000,
        0b10110,
        0b10001,
        0b10001,
        0b10110,
        0b11000,
        0b00000
    };
    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM)
                                     // ie to individual lines of character patterns
    for (uint8_t i = 0; i < 24; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(Play[i]);
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM)
    lcd_gotoxy(10, 0);
    lcd_putc(0x00);


    uint8_t Stop[24] = {
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b01010
    };
    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM)
                                     // ie to individual lines of character patterns
    for (uint8_t i = 0; i < 24; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(Stop[i]);
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM)
    lcd_gotoxy(10, 0);
    lcd_putc(0x01);

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"

    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);
    // Select input channel ADC0 (voltage divider pin)
    

    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));


    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM0_overflow_1ms();
    TIM0_overflow_interrupt_enable();

    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
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
ISR(TIMER0_OVF_vect)
{  
    if(value == 0)
    {
        ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3)); 
    }
    else if(value == 1)
    {
        ADMUX &= ~((1<<MUX1) | (1<<MUX2) | (1<<MUX3)); ADMUX |= (1<<MUX0);
    }
    // Start ADC conversion
    ADCSRA |= (1<<ADSC);
}
ISR(TIMER2_OVF_vect)
{
    static uint8_t no_of_overflows = 0;
    static uint8_t tenths = 0;  // Tenths of a second
    static uint8_t seconds = 0;
    static uint8_t minutes = 0;
    static uint16_t start = 0;
    static uint16_t encoder = 0;

    char string[2];             // String for converted numbers by itoa()
    start = GPIO_read(&PIND,SW);
    encoder = GPIO_read(&PIND,CLK);

    lcd_gotoxy(8, 0);
    itoa(encoder, string, 10);
    lcd_puts(string);  

    if (start == 1)
    {
        no_of_overflows ++; 
    }

    if (no_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        no_of_overflows = 0;
        tenths++;

        if(tenths>9)
        {
            tenths=0;
            seconds++;
            if (seconds>59)
            {
                seconds=0;
                minutes++;
            }
        }
        // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...


        itoa(tenths, string, 10);  // Convert decimal value to string
        lcd_gotoxy(6, 0);
        lcd_puts(string);
        
        itoa(seconds, string, 10);
        if (seconds<10)
        {
            lcd_gotoxy(4, 0);
            lcd_puts(string);
            lcd_gotoxy(3, 0);
            lcd_puts("0");
        }
        else 
        {
            lcd_gotoxy(3, 0);
            lcd_puts(string);
        }

        itoa(minutes, string, 10);
        if(minutes<10)
        {
            lcd_gotoxy(1, 0);
            lcd_puts(string);
            lcd_gotoxy(0, 0);
            lcd_puts("0");
        }
        else
        {
            lcd_gotoxy(0, 0);
            lcd_puts(string);
        }

        // Display "00:00.tenths"
        lcd_gotoxy(5, 0);
        lcd_puts(".");
        lcd_gotoxy(2, 0);
        lcd_puts(":");  
    }
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{   
    uint16_t nowerflows2 = 0;
    static uint16_t xValue;
    static uint16_t yValue;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC

    if(value == 1)
    {
        xValue = ADC;
        value = 0;
    }
    else if(value == 0)
    {
        yValue = ADC;
        value = 1;
    }
    //yValue = GPIO_read(&PINC,VRY);
    //stop = GPIO_read(&PIND,SW2);
    // Convert "value" to "string" and display it
    if (xValue < 400)
    {
        lcd_gotoxy(11, 0);
        lcd_puts("LEFT ");
    }
    else if (yValue < 400)
    {
        lcd_gotoxy(11, 0);
        lcd_puts("DOWN ");
    }
    else if (yValue > 600)
    {
        lcd_gotoxy(11, 0);
        lcd_puts("UP   ");
    }
    else if (xValue > 600)
    { 
        lcd_gotoxy(11, 0);
        lcd_puts("RIGHT");
    }
    else
    { 
        lcd_gotoxy(11, 0);
        lcd_puts("NONE ");
    }

    lcd_gotoxy(0, 1);
    lcd_puts("X=");
    itoa(xValue, string, 10);
    lcd_gotoxy(2, 1);
    lcd_puts("    ");
    lcd_gotoxy(2, 1);
    lcd_puts(string);  
    
    lcd_gotoxy(7, 1);
    lcd_puts("Y=");
    itoa(yValue, string, 10);
    lcd_gotoxy(9, 1);
    lcd_puts("    ");
    lcd_gotoxy(9, 1);
    lcd_puts(string); 

}