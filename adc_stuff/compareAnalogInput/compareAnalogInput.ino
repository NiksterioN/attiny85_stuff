/*
 * ATTINY85 Analog Comparator
 * if V+ > V-, Assert Analog Comparator Output Bit (ACO)
 * V+ : PB0
 * V- : PB1 is Default, 
 * 
*/

#include <avr/io.h>

#define F_CPU 8E6

#ifndef LOW
  #define LOW 0
#endif
#ifndef HIGH 
  #define HIGH 1
#endif

enum ANALOG_COMPARATOR_NEGATIVE_INPUT
{
  ADC0, // PB5
  ADC1, // PB2
  ADC2, // PB4
  ADC3  // PB3 
};

void initAnalogComparator(ANALOG_COMPARATOR_NEGATIVE_INPUT AINx)
{
  // Note: If ACME is NOT ASSERTED, V- = AIN1 (PB1) 
  ACSR = 0;
  ADCSRA &= ~(1 << ADEN); // Disable ADC, Enable ADCx as Negative Input to Comparator 
  ADCSRB |= 1 << ACME;    // Assert Analog Comparator Multiplexer Enable Bit
  ADMUX |= AINx << MUX0;  // Select V-, Analog Comparator Negative Input
}

int main(void)
{
  initAnalogComparator(ADC1);
  
  DDRB |= 1 << PB4;
  while (1)
  {
    _Bool outComparator = (ACSR >> ACO) & 1;  // Check ACO Comparator Output Bit
    if (outComparator == HIGH)
    {
      PORTB |= 1 << PB4;    
    }
    else
    {
      PORTB &= ~(1 << PB4);
    }
  }
}
