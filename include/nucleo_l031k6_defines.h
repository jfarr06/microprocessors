#ifndef NUCLEO_L031K6_DEFINES_H_
#define NUCLEO_L031K6_DEFINES_H_

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

#endif
