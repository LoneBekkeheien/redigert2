enum state{
    IDLE,
    RUN,
    EMERGENCY_STOP,
    DOOR_OPEN,
} state, nextstate;

//Initialiserer med tall som ikke brukes til noe 
int last_floor = N_FLOORS + 1;
int last_dir = N_FLOORS + 1;

//Heisen initialiseres ved å kjøre OPP til nærmeste etasje, hvis ikke allerede i en etasje 
void elevator_init();

//Returnerer retningen, -1 for ned, 1 for opp og 0 for å bli stående
int get_direction();

//Funksjonene gjør endringer når heisen bytter state
void fsm_EMERGENCY_STOP();
void fsm_IDLE();
void fsm_RUN();
void fsm_DOOR_OPEN();