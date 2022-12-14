/***********************************************************************
 * 
 * The I2C (TWI) bus scanner tests all addresses and detects chips and
 * modules that are connected to the SDA and SCL signals.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1, send one I2C (TWI) address every 33 ms,
 *           and test all combinations. Send information about scanning
 *           process to UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize I2C (TWI)
    twi_init();

    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure 16-bit Timer/Counter1 to test one I2C address
    // Set prescaler to 33 ms and enable interrupt
    TIM1_overflow_4s();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("Scan I2C bus for devices:\r\n");

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
 * Purpose:  Test one I2C Slave address from the range 8 to 119.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static uint8_t sla = 92;  // I2C Slave address 8-addresses, 92-temp, 87- clock
    uint8_t ack;             // ACK response from Slave
    uint8_t data;
    char string[3];          // String for converting numbers by itoa()

    struct Air_parameters_structure {
        uint8_t humid_int;
        uint8_t humid_dec;
        uint8_t temp_int;
        uint8_t temp_dec;
        uint8_t checksum;
    } air;
    /*
    struct Air_parameters_structure {
        uint8_t secs;
        uint8_t mins;
        uint8_t hours;
    } rtc;
    */

    // Start communication, transmit I2C Slave address, get result,
    // and Stop communication

    // Temperature and humid
    ack = twi_start(sla, TWI_WRITE);
    twi_write(0x00);
    twi_stop();

    ack = twi_start(sla, TWI_READ);
    air.humid_int = twi_read_nack();
    twi_stop();
    
    ack = twi_start(sla, TWI_WRITE);
    twi_write(0x01);
    twi_stop();

    ack = twi_start(sla, TWI_READ);
    air.humid_dec = twi_read_nack();
    twi_stop();

    ack = twi_start(sla, TWI_WRITE);
    twi_write(0x02);
    twi_stop();

    ack = twi_start(sla, TWI_READ);
    air.temp_int = twi_read_nack();
    twi_stop();

    ack = twi_start(sla, TWI_WRITE);
    twi_write(0x03);
    twi_stop();

    ack = twi_start(sla, TWI_READ);
    air.temp_dec = twi_read_nack();
    twi_stop();

    ack = twi_start(sla, TWI_WRITE);
    twi_write(0x04);
    twi_stop();

    ack = twi_start(sla, TWI_READ);
    air.checksum = twi_read_nack();
    twi_stop();
    
    itoa(air.humid_int,string,10);
    uart_puts(string);
    uart_puts(".");
    
    itoa(air.humid_dec,string,10);
    uart_puts(string);
    uart_puts("\r\n");

    itoa(air.temp_int,string,10);
    uart_puts(string);
    uart_puts(".");

    itoa(air.temp_dec,string,10);
    uart_puts(string);
    uart_puts("\r\n");

    itoa(air.checksum,string,10);
    uart_puts(string);
    uart_puts("\r\n\n");

    /* Device addresses
    if(ack == 0)
    {
    itoa(sla,string,10);
    uart_puts("Device on address: ");
    uart_puts(string);
    uart_puts("\r\n");
    }
    sla++;
    if(sla > 120)
    {
    sla = 0;
    uart_puts("\r\n\nReporting scan:\r\n");
    }
    */
    // Test ACK/NACK value obtained from I2C bus and send info to UART 
}