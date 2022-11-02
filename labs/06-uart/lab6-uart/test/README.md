# Lab 6: INSERT_YOUR_FIRSTNAME INSERT_YOUR_LASTNAME

### ASCII

1. Complete the table with selected ASCII codes.

   | **Char** | **Decimal** | **Hexadecimal** | **Binary** |
   | :-: | :-: | :-: | :-: |
   | `a` | 97 | 0x61 | `0b0110_0001` |
   | `b` | 98 | 0x62 | `0b0110_0010` |
   | `c` | 99 | 0x63 | `0b0110_0011` |
   | `0` | 48 | 0x30 | `0b0011_0000` |
   | `1` | 49 | 0x31 | `00110001` |
   | `2` | 50 | 0x32 | `00110010` |
   | `Esc` | 27 | 0x1b | `00011011` |
   | `Space` | 32 | 0x20 | `00100000` |
   | `Tab` | 9 | 0x09 | `00001001` |
   | `Backspace` | 127 | 0x7f | `01111111` |
   | `Enter` | 13 | 0x0d | `00001101` |

### UART communication

2. Draw timing diagram of the output from UART/USART when transmitting three character data `De2` in 4800 7O2 mode (7 data bits, odd parity, 2 stop bits, 4800&nbsp;Bd). The image can be drawn on a computer or by hand. Name all parts timing diagram.

   ![your figure]()

3. Draw a flowchart for function `uint8_t get_parity(uint8_t data, uint8_t type)` which calculates a parity bit of input 8-bit `data` according to parameter `type`. The image can be drawn on a computer or by hand. Use clear descriptions of individual algorithm steps.

   ![your figure]()