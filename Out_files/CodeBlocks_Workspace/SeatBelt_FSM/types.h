/**< Types Defines and Structs - used throughout the prj */

typedef enum StateMachine_
{
    SM_IDLE_STATE         = 0,
    SM_SEAT_OCCUPIED,
    SM_SEAT_BELT_ON,
    SM_BUZZER
}StateMachine;

typedef enum Seat_Occupancy_Status_
{
    SEAT_NOT_OCCUPIED = 0,
    SEAT_OCCUPIED
}Seat_Occupancy;

typedef enum Belt_Status_
{
    SEAT_BELT_OFF = 0,
    SEAT_BELT_ON
}Belt_Status;

typedef struct CurrentState_
{
    uint8_t Seat;
    uint8_t Belt;
}CurrentStatetype;
