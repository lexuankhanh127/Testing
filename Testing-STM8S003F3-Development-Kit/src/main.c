#include "stm8s.h"

void delay_ms(uint16_t ms) {
    // TIM4 configuration for HSE = 16 MHz
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125); // 1 ms per count
    // Prescaler = 128 -> 16 MHz / 128 = 125 kHz (8 µs per tick)
    // ARR = 125 -> 125 * 8 µs = 1 ms
    while (ms--) {
        TIM4_SetCounter(0); // Reset counter
        TIM4_Cmd(ENABLE);   // Enable Timer
        while (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == RESET);
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);
        TIM4_Cmd(DISABLE);
    }
}

void main() {
    // Configure system clock to use HSE 16 MHz
    CLK_HSECmd(ENABLE); // Enable HSE
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); // No division, CPU = 16 MHz

    // Configure GPIO
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    // Enable clock for TIM4
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);

    // Main loop
    while (1) {
        GPIO_WriteHigh(GPIOB, GPIO_PIN_5); // Turn LED off
        delay_ms(100); // Delay 100 ms
        GPIO_WriteLow(GPIOB, GPIO_PIN_5);  // Turn LED on
        delay_ms(100); // Delay 100 ms
    }
}