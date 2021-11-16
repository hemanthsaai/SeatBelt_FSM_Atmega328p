/**< MAIN.h header */

/**< Macro Definition */
#define SEAT_OCCUPANCY_PIN PIND0
#define SEATBELT_PIN PIND1
#define TRUE 1
#define FALSE 0
// Value in Seconds
#define TIME_TO_BUZZ 15

/**< Extern variable decleration */
extern CurrentStatetype CurrentState;
extern uint8_t Timer_Start;
extern uint8_t Timer_end;

/**< Interfaces Decleration */
extern void Timer1_Start(void);
extern void Timer1_Stop(void);
extern void Timer2_PWM_setup(void);
extern void Timer2_Start_PWM(void);
extern void Timer2_Stop_PWM(void);
void comp_init(void);
void Read_CurrentStatus(void);
void Run_StateMachine(void);
