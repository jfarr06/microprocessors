/**
 * @file: nucleo_f031k6/serial.c
 * @desc: UART serial communication implementation for debug output
 * @auth: James Farrelly (C24402114)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <nucleo_f031k6/common.h>
#include <nucleo_f031k6/serial.h>

#define USART_TX_PORT GPIOA
#define USART_RX_PORT GPIOA

#define USART_TX_PIN GPIO_MODER_MODER2_Pos
#define USART_RX_PIN GPIO_MODER_MODER15_Pos

#define USART_TX_AFR (USART_TX_PIN > 0xE)
#define USART_RX_AFR (USART_RX_PIN > 0xE)

#define USART_TX_AFN 0b0001
#define USART_RX_AFN 0b0001

#define USART_ICR_CLR ( USART_ICR_PECF   | \
                        USART_ICR_FECF   | \
                        USART_ICR_NCF    | \
                        USART_ICR_ORECF  | \
                        USART_ICR_IDLECF | \
                        USART_ICR_TCCF   | \
                        USART_ICR_LBDCF  | \
                        USART_ICR_CTSCF  | \
                        USART_ICR_EOBCF  | \
                        USART_ICR_CMCF   | \
                        USART_ICR_WUCF )

#define USART_CLOCK_SPEED 48000000 // 48MHz
#define USART_BAUD_RATE   9600

void init_nucleo_f031k6_serial(void) 
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Turn on USART1

    set_nucleo_f031k6_pin_mode(USART_TX_PORT, USART_TX_PIN, PINMODE_ALTERNATE); // enable alternative function (UART tx)
    set_nucleo_f031k6_pin_mode(USART_RX_PORT, USART_RX_PIN, PINMODE_ALTERNATE); // enable alternative function (UART rx)

    // Select UART2 for ports
    volatile uint32_t *afrTx = &USART_TX_PORT->AFR[USART_TX_AFR];
    volatile uint32_t *afrRx = &USART_RX_PORT->AFR[USART_RX_AFR];

    // select uart 1 alternative function for PA2 and PA15
    *afrTx &= ~(0b1111 << USART_TX_PIN * 2);
    *afrTx |= (USART_TX_AFN << USART_TX_PIN * 2);

    *afrRx &= ~(0b1111 << (USART_RX_PIN - 0x10) * 2);
    *afrRx |= (USART_RX_AFN << (USART_RX_PIN - 0x10) * 2);

    RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;

    USART1->CR1 = 0;
    USART1->BRR = USART_CLOCK_SPEED / USART_BAUD_RATE;

    USART1->CR3 |= USART_CR3_OVRDIS; // disable over-run errors
    USART1->CR1 |= USART_CR1_RE | USART_CR1_TE;
    USART1->CR1 |= USART_CR1_UE;
}

void nucleo_f031k6_putc(char c)
{
    while (!(USART1->ISR & USART_ISR_TC)); // Wait for transmission to finish.

    USART1->ICR |= USART_ICR_CLR; // clear any interrupts
    USART1->TDR = c; // Transmit byte
}

void nucleo_f031k6_puts(char *str)
{
    while (*str) {
        nucleo_f031k6_putc(*str);

        str++;
    }
}

size_t nucleo_f031k6_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    size_t len = vsnprintf(NULL, 0, format, args);
    if (len <= 0) return 0;

    len++;
    char* buf = malloc(len);
    vsnprintf(buf, len, format, args);

    va_end(args);

    nucleo_f031k6_puts(buf);
    free(buf);

    return len - 1;
}
