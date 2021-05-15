/*
 * ATTINY85 Phase Correct PWM
 * Uses Timer/Counter and CTC Mode
 * PWN Pins : PB0 = OC0A 
 *            PB1 = 0C0B 
 *            PB2 = 0C1A 
 *            PB3 = 0C1B
 * 
 * Operation:
 * COM0x1 COM0x0  Description
 *   1      0     Clear 0C0x on Compare Match, set 0C0x at BOTTOM (non-inverting)
 *   1      1     Set 0C0x on Compare Match, clear 0C0x at BOTTOM (inverting)
 * 
 * Non-inverting:
 * Duty_cycle = (OCR0x / 255) * 100;
 *    if TCNT0 < OCR0x, HIGH Output
 *    else if TCNT >= OCR0x, LOW Output 
 *    
 * Inverting :
 * Duty_cycle = (1 - (OCR0x / 255)) * 100
 *    if TCNT0 < OCR0x, LOW Output
 *    else if TCNT >= OCR0x, HIGH Output 
 *  
 *  PWM Frequency :
 *  f_PWM = F_CPU / (PRESCALER_VALUE * 510);
 *        
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
  
  TCCR0A = 0x00;          // Reset TCCR0x to 0
  TCCR0B = 0x00;          
  
  TCCR0A |= 1 << WGM00;   // Set Timer/Counter Mode to Phase Correct PWM
  TCCR0A |= 2 << COM0A0;  // Set OC0x on Compare Match (TCNT0 == OCR0x).
  TCCR0A |= 2 << COM0B0;  // clear OC0x at BOTTOM (non-inverting)
  
  TCCR0B |= DIVIDE_BY_256 << CS00;  // Set Prescaler
  TCNT0 = 0;
    
  OCR0A =  DUTY_CYCLE(DUTY_CYCLE1);
  OCR0B =  DUTY_CYCLE(DUTY_CYCLE2); 
  
  while (1)
  {
    // Do Nothing :)    
  }
}
