#include <time.h>

//Lagrer tidspunktet når døren åpnes 
time_t timer_door_opened_ms;

//Leser av tidspunktet og gjør det om til millisekunder 
void timer_start();

//Sjekker om det har gått tre sekunder siden døren ble åpnet 
int timer_time_out();