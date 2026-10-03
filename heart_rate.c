#include <util/delay.h>
#include <stdio.h>
#include "debug.h"

#define VREF 3.3


void init_adc(void)
{
	//we need to initialise ADC 
	ADMUX= 0x00;
	
	//Then enable it with prescalar 64 
	ADCSRA|= _BV(ADPS2) | _BV(ADPS1) | _BV(ADEN);
		
}

uint16_t read_adc(void)
{	// we need to start single conversion
	ADCSRA|= _BV(ADSC);
	
	//and then wait until conversion is complete
	while(ADCSRA & _BV(ADSC));

	return ADC;
}

void channel_adc(uint8_t n)
{   
	ADMUX= n; //to select a channel n
}

int main(void)
{
	uint16_t result;
	double voltage;
	
	const double THRESHOLD = 1.8;
	
	uint8_t above_threshold = 0;      // Tracks if currently above threshold
	uint32_t last_peak_time = 0;      // Time of last detected peak 
	uint32_t current_time = 0;        // Current time 
	uint32_t time_between_peaks;      // Interval between peaks
	double bpm;                       // Calculated beats per minute
	
	uint8_t led_timer = 0;            // Timer for LED pulse
	
	
	
	init_debug_uart0();
	init_adc();
	
	//Decided on B7 to be output 
	DDRB |= _BV(PINB7);

	
	//Channel 2 for filtered heart rate
	channel_adc(2);
	
	for (;;) 
	{
		result= read_adc();
		voltage= (result/1024.0) * VREF;
		
		// Define a treshhold 
		if (!above_threshold && (voltage > THRESHOLD)) {
			above_threshold = 1;
			
			// Calculate BPM if this is not the first peak
			if (last_peak_time > 0) {
				time_between_peaks = current_time - last_peak_time;
				
				// Calculate heart rate in BPM
				bpm = 60000.0 / time_between_peaks;
				
				// Only display withing threshold 
				if (bpm >= 40.0 && bpm <= 200.0) {
					printf("Interval: %5lu ms | Heart Rate: %5.1f BPM\n", 
					       time_between_peaks, bpm);
				// turn on led
					PORTB |= (1 << PB7);
					led_timer = 1;  // Start LED timer
				}
				/*else {
					//printf("Invalid reading: %.1f BPM (ignored)\n", bpm);
				}*/
			}
			
			// update last peak time
			last_peak_time = current_time;
		}
		
		// Dwe need to detect the falling edge 
		if (above_threshold && (voltage < THRESHOLD)) {
			above_threshold = 0;
		}
		
		if (led_timer > 0) {
			led_timer++;
			if (led_timer >= 2) {  // At 50ms sampling, 2 samples = 100ms
				PORTB &= ~(1 << PB7);  // Turn LED off
				led_timer = 0;
			}
		}
		
	
		_delay_ms(50);
		current_time += 50;
	}
}

//avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os -Wl,-u,vfprintf -lprintf_flt -lm heartrate1.c -o heartrate1.elf
//avr-objcopy -O ihex heartrate1.elf heartrate1.hex
//avrdude -c usbasp -p m644p -U flash:w:heartrate1.hex
