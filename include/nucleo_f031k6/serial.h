#ifndef NUCLEO_F031K6_SERIAL_H_
#define NUCLEO_F031K6_SERIAL_H_

#include <stdlib.h>

/**
 * Initializes serial communication.
 */
void init_serial();

/**
 * Writes a character to the serial output.
 *
 * @param c The character.
 */
void s_putc(char c);

/**
 * Writes n amount of bytes to the serial output.
 *
 * @param str The string
 */
void s_puts(char* str);

/**
 * Writes a formatted output to the serial port.
 *
 * @param format The format string.
 * @param ... The varags
 */
size_t s_printf(const char* format, ...);

#endif
