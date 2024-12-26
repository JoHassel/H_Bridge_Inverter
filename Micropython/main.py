# Load libraries:
from machine import Pin
from math import sin, pi
from machine import Timer

# Initialize:
f_sine = 50                         # Frequency of the generated sine wave
pwm_periods = 10                    # PWM Periods per half-wave
f_pwm = int(pwm_periods*f_sine)     # PWM frequency
scaler = 10                         # ratio between f_pwm and f_switch
f_switch = scaler * f_pwm           # Switching frequency
cycles = 0                          # iterated on every timer callback
fin = scaler * pwm_periods * 2      # cycle number at the end of a sine wave

# Calculate Look-up table:
r = range(1,pwm_periods+1)
sinewave = sin
lut = [int((scaler + (scaler*sin(2*pi*x/pwm_periods)))/2) for x in r]
print(lut)

# Activate pins: 
g1 = Pin(16, Pin.OUT)
g2 = Pin(17, Pin.OUT)

# Timer Callback:
def clock_cb(timer):
    global cycles
    num = cycles % scaler                   # cycle number within a PWM period
    i = int((cycles%100) / scaler)          # number of PWM period

    if num == 0:
        if cycles == fin:
            cycles = 0        
        g1.high()
        g2.low()
        print("Pins 1&4 HIGH, Pins 2&3 LOW")

    if num == (lut[i]):
        g1.low()
        g2.high()
        print("Pins 1&4 LOW, Pins 2&3 HIGH")
    
    cycles +=1
    print(f"Callback, Cycle: {cycles}, Num: {num}, i: {i} Lut[i]: {lut[i]}")
    

# Timer Initialization:
tim_clock = Timer()
tim_clock.init(freq=f_switch, mode=Timer.PERIODIC, callback=clock_cb)

# Main loop:
while True:
    try:
        running = True
    except KeyboardInterrupt:
        running = False
        tim_clock.deinit()
        break