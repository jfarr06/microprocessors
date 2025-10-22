#include <debug.h>
#include <notes.h>
#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/common.h>

#define SOUND_PORT   GPIOB
#define SOUND_PINOUT GPIO_MODER_MODER1_Pos

#define SOUND_PRESCALER_BASFREQ (48000000UL) // 48MHz 
#define SOUND_PRESCALER_CLKFREQ (0x10000UL)  // 65kHz
#define SOUND_PRESCALER         (SOUND_PRESCALER_BASFREQ / SOUND_PRESCALER_CLKFREQ)

void init_sound()
{
    DBG_INFO("Initializing sound on PB1, base_freq = %p, clk_freq = %p", SOUND_PRESCALER_BASFREQ, SOUND_PRESCALER_CLKFREQ);

    pin_mode(SOUND_PORT, SOUND_PINOUT, PINMODE_ALTERNATE); // select alternative function for PB1

    SOUND_PORT->AFR[0] &= ~(0b1111 << 4); // T14_CH1 = AF0
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	TIM14->CCMR1 = TIM_CCMR1_OC1M_2 + TIM_CCMR1_OC1M_1;
	TIM14->CCER |= TIM_CCER_CC1E;

    TIM14->CR1 = 0;
	TIM14->PSC = SOUND_PRESCALER;
	TIM14->ARR = (SOUND_PRESCALER_BASFREQ / TIM14->PSC) / C4;
	TIM14->CCR1 = TIM14->ARR / 2;	
	TIM14->CNT = 0;
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0) TIM14->CR1 &= ~TIM_CR1_CEN; // disable the counter

	TIM14->ARR = SOUND_PRESCALER_CLKFREQ / frequency; 
	TIM14->CCR1 = TIM14->ARR / 2;	

	TIM14->CNT = 0; // set the count to zero initially
	TIM14->CR1 |= TIM_CR1_CEN; // and enable the counter 
}

