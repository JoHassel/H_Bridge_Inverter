#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define HI 1
#define LO 0
#define g1 16          // GPIO for Gate 1
#define g2 17          // GPIO for Gate 2
#define f_sine 50      // Frequency of the generated sine wave
#define pwm_periods 20 // PWM Periods per half-wave
#define scaler 20      // ratio between f_pwm and f_switch

// Global Variables:
const int f_switch = pwm_periods * f_sine * scaler * 2; // Switching frequency
const int T_switch = (int)(1000000 / f_switch);         // Timer period in us
int cycles = 0;                                         // iterated on every timer callback
const int fin = scaler * pwm_periods * 2;               // cycle number at the end of a sine wave
int lut[pwm_periods];                                   // Look-up table for sine PWM

// Timer Callback
bool repeating_timer_callback(__unused struct repeating_timer *t)
{
    int num = cycles % scaler; // cycle number within a PWM period

    if (num == 0)
    {
        if (cycles == fin)
        {
            cycles = 0;
        }
        gpio_put(g1, HI);
        gpio_put(g2, LO);
        // printf("Gate 1&4 HIGH, Gate 2&3 LOW\n");
    }

    int i = (int)(cycles / (scaler * 2)); // number of PWM period
    if (num == (lut[i]))
    {
        gpio_put(g2, HI);
        gpio_put(g1, LO);
        // printf("Gate 1&4 LOW, Gate 2&3 HIGH\n");
    }

    cycles += 1;
    // printf("Callback, Cycle: %i, Num: %i, i: %i Lut[i]: %i\n", cycles, num, i, lut[i]);

    return true;
}

// GPIO initialisation:
int pins_init(void)
{
    gpio_init(g1);
    gpio_set_dir(g1, GPIO_OUT);
    gpio_init(g2);
    gpio_set_dir(g2, GPIO_OUT);
    return PICO_OK;
}

int main()
{
    stdio_init_all();
    pins_init();

    // Calculate Look-up table:
    for (int r = 1; r <= pwm_periods; r++)
    {
        lut[r - 1] = (int)((scaler + (scaler * sin(2 * M_PI * r / pwm_periods))) / 2);
        // printf("lut[%d] = %d\n", r, lut[r - 1]);
    }

    // Timer Initialization:
    struct repeating_timer timer;
    add_repeating_timer_us(T_switch, repeating_timer_callback, NULL, &timer);

    // Infinte Loop:
    while (true)
    {
        tight_loop_contents();
    }
}