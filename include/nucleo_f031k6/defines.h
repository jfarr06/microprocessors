/**
 * @file: nucleo_f031k6/defines.h
 * @desc: Pin mode and pull mode definitions for STM32F031K6
 * @auth: James Farrelly (C24402114)
 */

#ifndef NUCLEO_F031K6_DEFINES_H_
#define NUCLEO_F031K6_DEFINES_H_

#define F031K6_UNUSED(x) (void)(x)

//================ PINMODE

#define PINMODE_INPUT     (0x0) // This pin takes input.
#define PINMODE_OUTPUT    (0x1) // This pin takes output.
#define PINMODE_ALTERNATE (0x2) // This pin produces special data.
#define PINMODE_ANALOG    (0x3) // This pin produces analogue data.

//================ PULL MODE

#define PULLMODE_NOPULL   (0x0) // No pull-up, or pull-down
#define PULLMODE_PULLUP   (0x1) // Pull-up
#define PULLMODE_PULLDOWN (0x2) // Pull-down

//================ INPUT MODE

#define INPUTMODE_PULLDOWN (0x0) // The input is a pulldown resistor.
#define INPUTMODE_PULLUP   (0x1) // The input is a pullup resitor.

//================ REG defines (the ones that stm32l031xx.h don't supply)

#define ADC_CCR_VLCDEN_Pos           (24U)      
#define ADC_CCR_VLCDEN_Msk           (0x1UL << ADC_CCR_VLCDEN_Pos)                  /*!< 0x01000000 */
#define ADC_CCR_VLCDEN               ADC_CCR_VLCDEN_Msk                            /*!< Vlcd enable */

#define RCC_AHBENR_B17EN_Pos         (17U)
#define RCC_AHBENR_B17EN_Msk         (0x1UL << RCC_AHBENR_B17EN_Pos)
#define RCC_AHBENR_B17EN             RCC_AHBENR_B17EN_Msk

#define SPI_CR2_8BIT_Pos             (8U)
#define SPI_CR2_8BIT_Msk             (0b111UL << SPI_CR2_8BIT_Pos)
#define SPI_CR2_8BIT                 SPI_CR2_8BIT_Msk

#define FLASH_ACR_LATENCY1_Pos         (1U)
#define FLASH_ACR_LATENCY1_Msk         (0x1UL << FLASH_ACR_LATENCY1_Pos)
#define FLASH_ACR_LATENCY1             FLASH_ACR_LATENCY1_Msk

#define FLASH_ACR_LATENCY2_Pos           (2U)
#define FLASH_ACR_LATENCY2_Msk           (0x1UL << FLASH_ACR_LATENCY2_Pos)
#define FLASH_ACR_LATENCY2               FLASH_ACR_LATENCY2_Msk

#endif
