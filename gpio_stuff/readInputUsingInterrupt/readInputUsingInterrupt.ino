/*
 * A Simple Demonstration of ATTINY85 toggling an LED using INT0 Interrupt
 * Connections: 
 * PB3 : LED and Resistor to GND
 * PB4 : Switch to Input Pin connected to GND
 * Optional : Put a Debouncing Circuit/Code for clean logic transition
 * Might be quite glitchy without debouncing circuit
 */
#include <avr/io.h> 
#include <avr/interrupt.h>

// Interrupt Handler invoked when INT_PIN logic state changes
// Defined in the <avr/interrupt.h>
ISR(INT0_vect)
{
  PORTB ^= 1 << PORTB3;
}

int main()  
{
 
  DDRB |= 1 << DDB3;    // Set PB3 as Output 
//  DDRB &= ~(1 << DDB4); // Set PB4 as Input 
  
  PORTB |= 1 << PORTB3; // Initially Set the LED Pin to HIGH
  PORTB |= 1 << PORTB4; // Activate Internal Pull-up Resistor

  // Interrupt Setup
  cli();                        // Disable Interrupt   
  GIMSK |= 1 << INT0;           // Assert External Interrupt Flag 0 (PB2)                           
  MCUCR |= 1 << ISC01;          // Falling Edge-Triggered Interrupt (""""...)
  sei();                        // Enable the Global Interrupt

  while (1)
  {
    // Do nothing while waiting for interrupt trigger
  }

}
