// Initialize:
int f_sine = 50;                         // Frequency of the generated sine wave
int pwm_periods = 10;                    // PWM Periods per half-wave
int f_pwm = pwm_periods*f_sine;          // PWM frequency
int scaler = 10;                         // ratio between f_pwm and f_switch
int f_switch = scaler * f_pwm;           // Switching frequency
int cycles = 0;                          // iterated on every timer callback
int fin = scaler * pwm_periods * 2;      // cycle number at the end of a sine wave


