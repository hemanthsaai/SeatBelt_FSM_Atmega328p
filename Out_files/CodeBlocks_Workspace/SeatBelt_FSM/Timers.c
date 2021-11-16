/**< Header Inclusion */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "main.h"

/** \brief
 * Function to Start the timer 1
 * \param  None
 * \param  None
 * \return None
 *
 */
void Timer1_Start(void)
{
    TCNT1  = 0x0;                                       // Reset the Timer Counter
	TIMSK1 |= (1<<OCIE1A);                              // Enable Interrupt Timer, Output Compare A (TIMER1_COMPA_vect)
    sei();
}

/** \brief
 * Function to Stop the timer 1
 * \param  None
 * \param  None
 * \return None
 *
 */
void Timer1_Stop(void)
{
    TIMSK1 &= ~(1<<OCIE1A);                             // Disable Interrupt Timer, Output Compare A (TIMER1_COMPA_vect)
    TCNT1  = 0x0;
}

/** \brief
 * Function to Configure Timer 2 for Fast PWM
 * \param  None
 * \param  None
 * \return None
 *
 */
void Timer2_PWM_setup(void)
{
  // WGM22/WGM21/WGM20 all set -> Mode 7, fast PWM
  TCCR2A = (1<<COM2B1) + (1<<WGM21) + (1<<WGM20); // Set OC2B at bottom, clear OC2B at compare match
  TCCR2B = (1<<CS22) + (1<<CS21)+ (1<<CS20) + (1<<WGM22); // prescaler = 1024;
}

/** \brief
 *  Interface to Start the PWM on PD3
 * \param  None
 * \param  None
 * \return None
 *
 */
void Timer2_Start_PWM(void)
{
    // Configure to get 50% duty cycle and set pin as output
      OCR2A = 249;
      OCR2B = 124;
      DDRD |= (1<<PD3);
}

/** \brief
 * Interface to Stop the PWM on PD3
 * \param  None
 * \param  None
 * \return None
 *
 */
void Timer2_Stop_PWM(void)
{
    //Clear the Registers and reset the pin state to stop pwm
      OCR2A = 0;
      OCR2B = 0;
      DDRD &= ~(1<<PD3);
}
