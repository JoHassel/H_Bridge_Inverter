#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>


// Initialize:
int init(){
    const int f_sine = 50;                         // Frequency of the generated sine wave
    const int pwm_periods = 10;                    // PWM Periods per half-wave
    const int f_pwm = pwm_periods*f_sine;          // PWM frequency
    const int scaler = 10;                         // ratio between f_pwm and f_switch
    const int f_switch = scaler * f_pwm;           // Switching frequency
    int cycles = 0;                                // iterated on every timer callback
    const int fin = scaler * pwm_periods * 2;      // cycle number at the end of a sine wave

    // Calculate Look-up table:
    int lut[pwm_periods];
    for (int r = 1; r <= pwm_periods; r++){
        lut[r] = int((scaler + (scaler*M_PI*sin(2*r/pwm_periods)))/2);
        std::cout << "Element at index " << r << ": " << lut[r] << std::endl;
    }
    // Activate pins: 
    // g1 = Pin(16, Pin.OUT)
    //g2 = Pin(17, Pin.OUT)
    return 0;
}

int main(int argc, char *argv[]){
    init();
    return 0;
}