/***********************************************************************
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

/* Define parameters and pins for servo ------------------------------------------------*/
# define servo1 PB1 
# define servo2 PB2
# define min 124
# define mid 192
# define max 260

volatile uint32_t position1 = mid;
volatile uint32_t position2 = mid;

static uint16_t value = 0;

int main(void)
{   
    lcd_init(LCD_DISP_ON);

    /* ----------------------------- Timer for SERVO -----------------------------------*/
    // Configure 8-bit Timer/Counter0 to control servo motors
    // Set prescaler to 16 ms and enable overflow interrupt
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();

    /* ------------------------- Set pins with gpio library ----------------------------*/
    GPIO_mode_output(&DDRB, servo1);        // Servo rotor 1
    GPIO_mode_output(&DDRB, servo2);        // Servo rotor 2

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

    // Set 10. waveform generation mode (1010)
    TCCR1A |= (1 << WGM11);                  
    TCCR1B |= (1 << WGM13);

    // Set compare output mode
    TCCR1A |= (1 << COM0A1) | (1 << COM0B1); 

     // Set TOP value
    ICR1 = 1250;
                         
    // Set duty cycle
    OCR1A = position1;
    OCR1B = position2;
    
    // Set prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10 ); 

    PCICR |= (1<<PCIE0);                    // Any change of any enable PCINT[7:0] pins will cause an interrupt
    PCMSK0 |= (1<<PCINT0);                  // Enable PCINT0 change interrupt  

    // Enables interrupts by setting the global interrupt mask
    sei();
    
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}

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

ISR(ADC_vect)
{   
    static uint16_t xValue;
    static uint16_t yValue;
    char string[4];  // String for converted numbers by itoa()
 
    if(value == 1)      // Reading xValue on joystick
    {
        xValue = ADC;
        value = 0;
    }
    else if(value == 0) // Reading yValue on joystick
    {
        yValue = ADC;
        value = 1;
    }
    
    /* ------------------------- Rotors movement ----------------------------*/
    if (xValue < 400)
    {
        for(position1 = mid; position1 <= max; position1 += 1)
        {
            OCR1A = position1;
        }
    }
     else if (xValue > 600)
    { 
        for(position1 = mid; position1 >= min; position1 -= 1)
        {   
            OCR1A = position1;
        }
    }
    else if (yValue < 400)
    {
        for(position2 = mid; position2 <= max; position2 += 1)
        {
            OCR1B = position2;
        }
    }
    else if (yValue > 600)
    {
        for(position2 = mid; position2 >= min; position2 -= 1)
        {
            OCR1B = position2;
        }
    }
    else
    {   
        position1 = mid;
        position2 = mid;
        OCR1A = position1;
        OCR1B = position2;
    }

    /* ---- Writing positions of rotors on display ----*/
    lcd_gotoxy(0, 0);
    lcd_puts("Rotor 1:");
    lcd_gotoxy(0, 1);
    lcd_puts("Rotor 2:");
 
    if (position1 > (mid + 30))
    {
        lcd_gotoxy(9, 0);
        lcd_puts("max");
    }  
    else if (position1 < (mid - 30))
    {
        lcd_gotoxy(9, 0);
        lcd_puts("min");
    }   
    else
    {
        lcd_gotoxy(9, 0);
        lcd_puts("mid");
    } 
    
    if (position2 > (mid + 30))
    {
        lcd_gotoxy(9, 1);
        lcd_puts("max");
    }  
    else if (position2 < (mid - 30))
    {
        lcd_gotoxy(9, 1);
        lcd_puts("min");
    }
    else
    {
        lcd_gotoxy(9, 1);
        lcd_puts("mid");
    } 
}