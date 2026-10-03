# Getting-good-at-C

This folder represents my favorite projects to work on using C. All of the embedded programs were written for the Il Matto development board (ATmega644P at 12 MHz) given to ECS students at the university of Southampton.


- `c04-caesar-cipher/caesar_cipher.c`:  Caesar cipher with encipher and decipher functions + a code breaker that scores all 26 shifts against English letter frequencies and picks the best.

- `c09-adc-sensing/heart_rate.c` :

  - photoplethysmography heart rate monitor: 

It is a heart-rate monitor uses a TCRT1010 reflective optical sensor. The signal passes through a band-pass filter before being read by an ADC channel.
I sample the ADC every 50 ms and convert the reading to voltage using the 3.3 V reference. A heartbeat is detected when the signal crosses 1.8 V on the rising edge.
BPM is calculated from the time between detected beats: BPM = 60000 / time between beats 
Readings outside 40–200 BPM are ignored and the valid readings are sent over the UART and briefly turn on the LED.

