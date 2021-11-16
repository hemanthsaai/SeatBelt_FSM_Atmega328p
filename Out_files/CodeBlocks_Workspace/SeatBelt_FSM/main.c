/*
 */
 /**< Macro Definition */
#define F_CPU 16000000UL

/**< Header Inclusion */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "main.h"

/**< Variable Decleration */
uint8_t timer1_count = 0;
uint16_t timer2_count = 0;
uint8_t Timer_Start = FALSE;
uint8_t Timer_end = FALSE;
uint8_t State_Change = TRUE;
CurrentStatetype CurrentState = {SEAT_NOT_OCCUPIED, SEAT_BELT_OFF};

/**< Main Starts here */
int main(void)
{
    comp_init();
    Read_CurrentStatus();

    // Start of StateMachine
    while (1)
    {
        if(State_Change == TRUE)
        {
            Run_StateMachine();
        }
        else
        {
            // Wait for an interrupt. Do Nothing
        }
    }

    return 0;
}

/** \brief
 * Function Initializes the Controller peripherals
 * \param  None
 * \param  None
 * \return NOne
 *
 */
void comp_init(void)
{
    // Configure PD2 as an input using the Data Direction Register D (DDRD)
    DDRD &= ~_BV(DDD0);
    DDRD &= ~_BV(DDD1);

    // Enable the pull-up resistor on PD2 using the Port D Data Register (PORTD)
    PORTD |= _BV(PORTD0);
    PORTD |= _BV(PORTD1);

    // Enable pin change interrupt on the PCINT16 and PCINT17 pin using Pin Change Mask Register 2 (PCMSK2)
    PCMSK2 |= _BV(PCINT16);
    PCMSK2 |= _BV(PCINT17);

    // Enable pin change interrupt 2 using the Pin Change Interrupt Control Register (PCICR)
    PCICR |= _BV(PCIE2);

    // Configure PB1 as an output using the Port B Data Direction Register (DDRB)
    DDRB |= _BV(DDB1);

    //Configure Timer for Periodic Interrupt
    TCCR1B |= ((1<<CS12) | (1<<CS10) | (1<<WGM12));     // Clock/1024, 0.001024 seconds per tick, Mode=( CTC Clear timer on Compare Match)
    OCR1A  = 15625-1;                                   // 15625 means 1 sec

    Timer2_PWM_setup();
    // Enable interrupts
    sei();


    // Timer0   Timer1   Timer2
    //   8bit    16bit     8bit

}


/** \brief
 * Function reads the Pin Status to Working variable
 * \param  None
 * \param  None
 * \return None
 *
 */
void Read_CurrentStatus(void)
{
    // Read PD0 using the Port D Pin Input Register (PIND)
    if (PIND & _BV(SEAT_OCCUPANCY_PIN))
    {   // PD0 is High, so button is released/PulledUp
        // Set the CurrentState of seat to Not Occupied
        CurrentState.Seat = SEAT_NOT_OCCUPIED;
    }
    else
    {   // PD0 is Low, so button is pressed/Ground
        // Set the CurrentState of seat to Not Occupied
        CurrentState.Seat = SEAT_OCCUPIED;
    }

    if (PIND & _BV(SEATBELT_PIN))
    {   // PD0 is High, so button is released/PulledUp
        // Set the CurrentState of seat to Not Occupied
        CurrentState.Belt = SEAT_BELT_OFF;
    }
    else
    {   // PD0 is Low, so button is pressed/Ground
        // Set the CurrentState of seat to Not Occupied
        CurrentState.Belt = SEAT_BELT_ON;
    }
}

/** \brief
 *  Interrupt Service Routine for Timer 1
 * \param Interrupt Vector
 * \param None
 * \return None
 *
 */
ISR(TIMER1_COMPA_vect)
{
    //Increment the timer count.
	timer1_count++;
	if(timer1_count == TIME_TO_BUZZ)
    {
        Timer1_Stop();
        Timer_end   = TRUE;
        timer1_count = 0;
    }
}

/** \brief
 * Interrupt Service Routine for Pin State Change
 * \param Interrupt Vector
 * \param None
 * \return None
 *
 */
ISR(PCINT2_vect)
{
    State_Change = TRUE;
    Read_CurrentStatus();
}
