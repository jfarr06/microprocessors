#ifndef NUCLEO_F031K6_SERIAL_H_
#define NUCLEO_F031K6_SERIAL_H_

#include <stdlib.h>

/**
 * Initializes serial communication.
 */
void init_nucleo_f031k6_serial(void);

/**
 * Writes a character to the serial output.
 *
 * @param c The character.
 */
void nucleo_f031k6_putc(char c);

/**
 * Writes n amount of bytes to the serial output.
 *
 * @param str The string
 */
void nucleo_f031k6_puts(char* str);

/**
 * Writes a formatted output to the serial port.
 *
 * @param format The format string.
 * @param ... The varags
 */
size_t nucleo_f031k6_printf(const char* format, ...);

#endif
