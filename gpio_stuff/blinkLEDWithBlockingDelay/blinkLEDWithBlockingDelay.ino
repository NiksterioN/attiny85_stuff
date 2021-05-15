/*
 *  Implementing a Blocking Delay on an ATTINY85.
 *  Practice code for implementing Timer/Counter Register as a blocking delay.
 *  For actual implementation, use built-in <avr/util> library
 *  
 *  Delay Calculation
 *  - TCNT0 Register can Increment up to 255.
 *  - Each Increment takes F_CPU / Prescaler (32uS)
 *  - Time to overflow TCNT0 Register : 255 * 32uS = 8.16 ms per overflow
 *  - Number of Overflows required for time delay : Seconds / 8.16 ms, convert seconds to milliseconds            
 *  NOTE: unsigned int has a max limit of 65536, therefore delay can work for atleast 1 minute max.  
 */
#include <avr/io.h>


#define F_CPU 8E6
#define PRESCALER 256
#define TIME_TO_OVERFLOW (255 / ((F_CPU / 1000) / PRESCALER ));

void delay_init()
{
  TCCR0A &= ~(3 << COM0A0); // Normal Port Operation
  TCCR0A &= ~(3 << WGM00); // Timer/Counter Operation Mode = Normal
  TCCR0B &= ~(1 << WGM02); // 
  
  TCCR0B |= (1<<CS02);   // 256 Prescaler
}

void delay_ms(unsigned int milliseconds)
{

  volatile unsigned int rep = milliseconds / TIME_TO_OVERFLOW; // Discards the decimal, effectively round down
  TCNT0 = 0; //Reset the Timer/Counter Register to 0;
  for (unsigned int i = 0; i <= rep; i++)
  {
    while ((TIFR >> TOV0 & 1) == 0); // Loop until Timer/Counter0 Register overflows
    TIFR = 1 << TOV0; // Clear the Overflow Flag. NOTE : use = instead of |= for clearing interrupt flags. See Atmel FAQs
  }
}

int main()
{
  delay_init();
  // Sets PB3 as General Purpose Output
  DDRB |= (1 << DDB3);
  // Initially Set PB3 as Logic HIGH
  PORTB |= (1 << DDB3);

  while(1)
  {
  // Toggle PB3 Logic
    PORTB ^= (1 << DDB3);
    delay_ms(10000);
  }
}
