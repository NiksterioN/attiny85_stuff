/*
 * ATTINY 10-Bit Free Running Single-Ended Input ADC
 * Lights up the LED when the ADC reads half the Voltage Reference (1.28V)
 * Connections :
 * Connect a Potentiometer to PB3
 * Connect an LED to PB4
 * 
 * ADC Pins :
 *  ADC0 - PB5
 *  ADC1 - PB2
 *  ADC2 - PB4
 *  ADC3 - PB3
 *  
 *  ADC Conversion :
 *  ADC = (Vin * 2^n) / V_ref  
 *  
 *  ADC Sampling Frequency should be > 50 Hz and < 200 kHz
 *  
 *  ADC Resolution:
 *  Delta_V = V_ref / 2^n, where n is n-bit ADC (19.53 mV for 8-bit, 4.88 mV for 10-bit)
 */

#include <avr/io.h>

#define F_CPU 8E6
#define VOLTAGE_REFERENCE 2.56

#define ADC_VALUE(Vin) (Vin * 1024 / VOLTAGE_REFERENCE)
#define roundup(n) (n < 0 ? (n - 0.5) : (n + 0.5))

enum ADC_SINGLE_ENDED_CONFIG
{
  ADC0, // PB5
  ADC1, // PB2
  ADC2, // PB4
  ADC3  // PB3 
};

enum ADC_DIVISION_FACTOR   // ADC Sampling Frequency
{
  ADC_DIVIDE_BY_2 = 1, 
  ADC_DIVIDE_BY_4,
  ADC_DIVIDE_BY_8,
  ADC_DIVIDE_BY_16,
  ADC_DIVIDE_BY_32,
  ADC_DIVIDE_BY_64,
  ADC_DIVIDE_BY_128
};

void initADC(ADC_SINGLE_ENDED_CONFIG ADCx)
{
  ADMUX = 0;                              // Reset ADMUX Register

  //ADMUX = 1 << ADLAR;                   // ADC Left Shift Result, use for 8-bit ADC
  ADMUX |= (1 << REFS2) | (1 << REFS1);   // Set Vref to Internal 2.56 Voltage Reference
  ADMUX |= ADCx << MUX0;                  // Enable ADC3 on PB3
                                          // ADC stored in ADCL[0:9]
                                                                           
  ADCSRA |= ADC_DIVIDE_BY_64 << ADPS0;    // ADC Sampling Frequency = 62.5 kHz
  ADCSRA |= 1 << ADEN;                    // Enable ADC  

  unsigned char DIDR_PIN;                 // Disable the Digital Input Buffer for power saving :)
  switch(ADCx)
  {
    case ADC0:
      DIDR_PIN = 5;
      break;
    case ADC1:
      DIDR_PIN = 2;
      break;
    case ADC2:
      DIDR_PIN = 4;
      break;
    case ADC3:
      DIDR_PIN = 3;
      break;
  }
  DIDR0 |= 1 << DIDR_PIN;
}
int main(void)
{
  initADC(ADC3);
  DDRB |= 1 << PB3;   // LED Pin

  while (1)
  {
    ADCSRA |= 1 << ADSC;           // Start the ADC
    while (ADCSRA & (1 << ADSC));  // Wait ADC Conversion completion    

    unsigned int adc_low = ADCL;                  // Since 8-Bit uC, 10-Bit ADC is split in two: ADCL and ADCH
    unsigned int raw_adc = ADCH <<8 | adc_low;    // if ADLAR = 0, ADCL[0:7] | ADCH[0:1] 
                                                  // if ADLAR = 1, ADCL[6:7] | ADCH[0:7]                                                     
    if (raw_adc > ADC_VALUE(VOLTAGE_REFERENCE / 2))
      PORTB |= 1 << PB3;
    else
      PORTB &= ~(1 << PB3);
  }
}
