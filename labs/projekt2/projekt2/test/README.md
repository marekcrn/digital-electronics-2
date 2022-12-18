# Project 2

Precise generation of several PWM channels. Application of two (or more) Servo motors SG90.

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 12th and 13th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester, ie PlatformIO and not in the Arduino-style. No other development tools are allowed.

## Recommended GitHub repository structure

   ```c
   YOUR_PROJECT        // PlatfomIO project
   ├── include         // Included files
   │   └── timer.h
   ├── lib             // Libraries
   │   ├── gpio        // gpio library folder
   │   │   ├── gpio.c  
   │   │   └── gpio.h  
   │   └── lcd         // lcd library folder
   │        ├── lcd_definitions.h
   │        ├── lcd.c
   │        └── lcd.h
   ├── src             // Source file(s)
   │   └── main.c
   ├── test            // No need this
   │   └── README.md   // Report of this project
   └── platformio.ini  // Project Configuration File
   ```

## Recommended README.md file structure

### Team members

* Marek Černý
* Mikuláš Fyman

## Hardware description

#### Schéma zapojení v simulide

![Schema](Pictures/schema.png)

## Software description

## Flowchart

![Flowchart](Pictures/flowchart.png)

### Source files & libraries

1. **Timer:** [timer.h](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/include/timer.h)
2. **Gpio:** [gpio.c](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/lib/gpio/gpio.c), [gpio.h](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/lib/gpio/gpio.h)
3. **LCD:** [lcd.c](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/lib/lcd/lcd.c), [lcd.h](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/lib/lcd/lcd.h), [lcd_definitions.h](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/lib/lcd/lcd_definitions.h)
4. **Source:** [main.c](https://github.com/marekcrn/digital-electronics-2/blob/main/labs/projekt2/projekt2/src/main.c)

## Video



## References

1. https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf
2. https://navody.dratek.cz/arduino-projekty/servo-motor.html
3. https://www.ee-diary.com/2021/07/phase-correct-pwm-with-atmega328p.html?fbclid=IwAR24t4myL5OQH8FrCGZd9OGGwccg384H3I16n2MI2PBwJB_tLiVsiZRMhus
4. https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
