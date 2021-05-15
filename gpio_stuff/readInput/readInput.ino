/*
 * A Simple Demonstration of ATTINY85 GPIO as Input 
 * Continually polls the Input Pin for a trigger/level change
 * Connections: 
 * PB3 : LED and Resistor to GND
 * PB4 : Switch to Input Pin connected to GND
 * Optional : Put a Debouncing Circuit/Code for clean logic transition
 */
#include <avr/io.h> 


unsigned char buttonState = 0;
unsigned char previousState = 0;

int main()  
{
  DDRB |= 1 << DDB3;    // Set PB3 as Output 
  DDRB &= ~(1 << DDB4); // Set PB4 as Input 
  
  PORTB |= 1 << PORTB3; // Initially Set the LED Pin to HIGH
  PORTB |= 1 << PORTB4; // Activate Internal Pull-up Resistor

  while (1)
  {
    while (previousState == (buttonState = (PINB >> PINB4 & 1))); // Check if the button is pressed
    previousState = buttonState;

    if (buttonState)
		  PORTB ^= 1 << PORTB3; 			        // If the button is pressed, toggle the logic state
  }

}
