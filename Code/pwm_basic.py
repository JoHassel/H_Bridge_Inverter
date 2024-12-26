# Load libraries:
from machine import Pin
from machine import PWM

# Activate pin for PWM: 
g1 = Pin(16)


while True:
    try:
        pwm1 = PWM(g1, freq=50, duty_u16=2**16//2)
    except KeyboardInterrupt:
        break
