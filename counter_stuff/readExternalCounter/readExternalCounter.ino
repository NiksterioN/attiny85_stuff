/*
 * An ATTINY85 Code on incrementing the internal Timer/Count Register based on External Trigger
 * Toggles between Red and Blue LED based on number of button taps
 * Connections :
 *  PB3 - Red LED
 *  PB4 - Blue LED
 *  PB2 - Button (Timer0 via PB2)
 */
#include <avr/io.h>

#define F_CPU 8000000UL

unsigned int i = 0;

void counter_init()
{
  // Normal Operation
  TCCR0A = 0x00;
  TCCR0B &= ~(1 << WGM02) ;
  
  // Set External Clock Source on T0 for incrementing Timer/Count Register
  // Trigger on Falling Edge
  TCCR0B |= 6 << CS00;
  TCNT0 = 0; 
}

void reset_counter()
{
  i = 0;
  TCNT0 = 0;
}

int main()
{
  // Set 
  DDRB |= (1 << PB3) | (1 << PB4);
  DDRB &= ~(1 << PB2);

  PORTB |= 1 << PB2; //Pull-Up Resistor
  counter_init();

  PORTB &= ~(1 << PB3);
  PORTB &= ~(1 << PB4);
  
  while (1)
  {
    i = TCNT0;                  //Counter initially set to 0
    if (i < 10)                 // Button Press > 10, Turn on Red, Turn off Blue
    {
      PORTB |= 1 << PB3;
      PORTB &= ~(1 << PB4);
    }
    
    else if (i >= 10 && i <= 20) // Button Press >= 10 but <= 20, Flip LED state
    {
      PORTB &= ~(1 << PB3);
      PORTB |= 1 << PB4;
    }     
    else if (i > 20)            // Button Press > 20, Reset Counter to 0
    {
      reset_counter();
    }
  }
}
