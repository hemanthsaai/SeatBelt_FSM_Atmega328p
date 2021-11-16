
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "main.h"


static uint8_t StateMachine_Var = SM_IDLE_STATE;
static uint8_t Led_Status = FALSE;
static uint8_t Buzzer_Status = FALSE;
static uint8_t err_cnt;

/** \brief
 *  Function to Run the state machine. Switches between 4 states according to
 *  the respective switch inputs.
 * \param  None
 * \param  None
 * \return None
 *
 */
void Run_StateMachine(void)
{
    switch (StateMachine_Var)
    {
        case SM_IDLE_STATE:
            //Wait for seat belt and seat Occupancy
            if (CurrentState.Seat == SEAT_OCCUPIED)
            {
                StateMachine_Var = SM_SEAT_OCCUPIED;
            }
            else if (CurrentState.Belt == SEAT_BELT_ON)
            {
                StateMachine_Var = SM_SEAT_BELT_ON;
            }
            else
            {
                // Wait in SM_IDLE_STATE
            }
            break;
        case SM_SEAT_OCCUPIED:
            // enters here when seat occupied flag is set.
            if (CurrentState.Seat == SEAT_OCCUPIED && CurrentState.Belt == SEAT_BELT_OFF && Timer_Start == FALSE)
            {
                Timer_Start = TRUE;
                Timer1_Start();
            }
            else if (CurrentState.Seat == SEAT_OCCUPIED && CurrentState.Belt == SEAT_BELT_OFF && Timer_end == TRUE)
            {
                StateMachine_Var = SM_BUZZER;
                Timer_Start = FALSE;
                Timer_end = FALSE;
            }
            else if (CurrentState.Seat == SEAT_OCCUPIED && CurrentState.Belt == SEAT_BELT_ON)
            {
                if(Timer_Start == TRUE)
                {
                    Timer1_Stop();
                    Timer_Start = FALSE;
                    Timer_end = FALSE;
                }
                StateMachine_Var = SM_SEAT_BELT_ON;
                // Everything's Great. Lets Drive
            }
            else
            {
                // Waiting for Timer to End.
            }
            break;
        case SM_SEAT_BELT_ON:
            // enters here when seat belt flag is set.
            if (CurrentState.Belt == SEAT_BELT_ON)
            {
                if(Led_Status == FALSE)
                {
                    PORTB |= _BV(PINB1);
                    Led_Status = TRUE;
                }
            }
            else
            {
                StateMachine_Var = SM_IDLE_STATE;
                if (Led_Status == TRUE)
                {
                    //TURN OFF LED
                    PORTB &= ~_BV(PINB1);
                    Led_Status = FALSE;
                }
                // Wait in SM_SEAT_BELT_ON
            }
            break;
        case SM_BUZZER:
                // Wait for Seat Belt ON
                if (CurrentState.Belt == SEAT_BELT_ON)
                {
                    // Buzzer OFF
                    if(Buzzer_Status == TRUE)
                    {
                        Timer2_Stop_PWM();
                        Buzzer_Status = FALSE;
                    }
                    StateMachine_Var = SM_SEAT_BELT_ON;
                }
                else
                {   // Turn On Buzzer and Check if seat is not occupied
                    if(Buzzer_Status == FALSE)
                    {
                        Timer2_Start_PWM();
                        Buzzer_Status = TRUE;
                    }
                    if(CurrentState.Seat == SEAT_NOT_OCCUPIED)
                    {
                        StateMachine_Var = SM_IDLE_STATE;
                        if(Buzzer_Status == TRUE)
                        {
                            Timer2_Stop_PWM();
                            Buzzer_Status = FALSE;
                        }
                    }
                }
            break;
        default:
            err_cnt++;
    }
}
