# Lab 1: YOUR_FIRSTNAME LASTNAME

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

```c
int main(void)
{
    uint8_t led_value = LOW;  // Local variable to keep LED status

    // Set pin where on-board LED is connected as output
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    // Infinite loop
    while (1)
    {
        // Change LED value
        if (led_value == LOW)
            led_value = HIGH;
        else
            led_value = LOW;

        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);

        // Turn ON/OFF on-board LED
        digitalWrite(LED_GREEN, led_value);
        digitalWrite(LED_RED, led_value);

        if (led_value == LOW)
            led_value = HIGH;
        else
            led_value = LOW;

        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);
        
        // Turn ON/OFF on-board LED
        digitalWrite(LED_GREEN, led_value);
        digitalWrite(LED_RED, led_value);

        if (led_value == LOW)
            led_value = HIGH;
        else
            led_value = LOW;

        // Pause several milliseconds
        _delay_ms(LONG_DELAY);
        
        // Turn ON/OFF on-board LED
        digitalWrite(LED_GREEN, led_value);
        digitalWrite(LED_RED, led_value);

        if (led_value == LOW)
            led_value = HIGH;
        else
            led_value = LOW;

        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);
        
        // Turn ON/OFF on-board LED
        digitalWrite(LED_GREEN, led_value);
        digitalWrite(LED_RED, led_value);
    }

    // Will never reach this
    return 0;
}
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()
