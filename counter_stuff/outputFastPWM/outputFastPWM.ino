/*
 * ATTINY85 Fast PWM
 * Uses Timer/Counter and CTC Mode
 * PWN Pins : PB0 = OC0A 
 *            PB1 = 0C0B 
 *            PB2 = 0C1A 
 *            PB3 = 0C1B
 * 
 *  PWM Frequency :
 *  f_PWM = f_CPU / (PRESCALER_VALUE * 256)
 */

#include <avr/io.h>

#define F_CPU 8E6

#define roundup(num) num < 0 ? num - 0.5 : num + 0.5
#define DUTY_CYCLE(PERCENT) roundup((PERCENT * 255 / 100) + 0.5)

#define DUTY_CYCLE1 25
#define DUTY_CYCLE2 50

enum PRESCALER_VALUE 
{
  NO_PRESCALER = 1,
  DIVIDE_BY_8,
  DIVIDE_BY_64,
  DIVIDE_BY_256,
  DIVIDE_BY_1024,
  
};

int main(void)
{
  DDRB  |= (1 << PB0) | ( 1 << PB1);  // Initialize PB3 and PB4 as Output
    
  TCCR0A = 0x00;
  TCCR0B = 0x00;

  TCCR0A |= 3 << WGM00;   // Fast PWM
  //TCCR0B |= 1 << WGM02;
  
  TCCR0A |= 2 << COM0A0;  // Set OC0x on Compare Match (TCNT0 == OCR0x), clear OC0x at BOTTOM (non-inverting)
  TCCR0A |= 2 << COM0B0;
  
  
  TCCR0B |= DIVIDE_BY_256 << CS00; // Set Prescaler
  TCNT0 = 0;

  OCR0A =  DUTY_CYCLE(DUTY_CYCLE1);
  OCR0B =  DUTY_CYCLE(DUTY_CYCLE2); 
  
  while (1)
  {
    
  }
}
