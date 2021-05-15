/*
 * The Hello World of the Embedded Systems!
 * An ATTINY85 Code to toggle an LED on PB3 with a 1 second delay
 */
#include <avr/io.h>
#include <util/delay.h>

int main()
{
  // Sets PB3 as General Purpose Output
  DDRB |= (1 << DDB3);
  // Initially Set PB3 as Logic HIGH
  PORTB |= (1 << DDB3);

  while (1)
  {
    // Toggle PB3 Logic
    PORTB ^= (1 << DDB3);
    _delay_ms(1000);
  }

}
