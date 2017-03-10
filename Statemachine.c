#include "Elevmodule.h"
#include "Timer.h"
#include "Eventmanager.h"
#include "Queue.h"
#include "Statemachine.h"
#include <stdio.h>

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    elevator_init(); 
    elev_set_floor_indicator(elev_get_floor_sensor_signal()); 
    queue_init(); 
    queue_nOrders_init(); 

    state=IDLE;
    nextstate=IDLE;

    while (1) {

    //Setter etasjeindikator, forrige etasje og forrige retning.  
    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){ //Unngår å sette etasjeindikatoren når heisen ikke er i en etasje 
        elev_set_floor_indicator(floor); 
        last_floor = floor; 
        if(floor<orders[0].floor){
            last_dir=1;
        }
        if(floor>orders[0].floor){
            last_dir=-1;
        }     
    }


        switch(state){
                    case IDLE:
                        queue_update();
                        queue_add_order(); 

                       if(ev_emergency_button_pushed()==1){
                            nextstate = EMERGENCY_STOP;
                        }
                        else if(ev_order_same_floor() == 1){
                            nextstate = DOOR_OPEN;
                        }
                        else if(ev_orders_in_queue()==1){
                            nextstate = RUN;
                        }
                        else{
                            nextstate = IDLE; 
                        }
                        break;
                    case EMERGENCY_STOP:
                        if(ev_emergency_button_pushed()==0){
                            elev_set_stop_lamp(0);
                            queue_delete_all_orders();

                            int floor = elev_get_floor_sensor_signal();
                            if(floor != -1){
                                elev_set_floor_indicator(floor); //kanskje denne må inn i fsm_door_open()!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                nextstate = DOOR_OPEN;
                            }
                            else{
                                nextstate = IDLE; 
                            }
                        }
                        else{
                            nextstate = EMERGENCY_STOP;
                        }
                        break;
                    case RUN:   
                        queue_update();
                        queue_add_order();

                        if(ev_emergency_button_pushed() == 1){
                            nextstate = EMERGENCY_STOP;
                        }
                        else if(ev_order_same_floor() == 1){
                            nextstate = IDLE;
                        }
                        else{
                            nextstate = RUN;
                        }
                        break;
                    case DOOR_OPEN:
                        queue_update();
                        queue_add_order();

                        if(ev_time_door_out() == 1){
                            elev_set_door_open_lamp(0);
                            queue_delete_order(); //Sletter bestillingen her for å kunne åpne døren i IDLE 
                            nextstate = IDLE;                                        
                        }
                        else{
                            nextstate = DOOR_OPEN;
                        }
                        break;
                    default:
                        //Hvis noe er feil
                        break;
        }

          
        //Hvis stoppknappen trykkes inn vil neste tilstand alltid være EMERGENCY_STOP
        if(ev_emergency_button_pushed() == 1){
            nextstate = EMERGENCY_STOP;
        }
        
        if(state != nextstate){
            switch(nextstate){
                case IDLE:
                    fsm_IDLE();
                    break;
                case EMERGENCY_STOP:
                    fsm_EMERGENCY_STOP();
                    break;
                case RUN:   
                    fsm_RUN();
                    break;
                case DOOR_OPEN:
                    fsm_DOOR_OPEN();
                    break;
                default:
                    //Hvis noe er feil
                    break;
            }
        }
        state = nextstate;
    
    }
    return 0;
}

     
void fsm_DOOR_OPEN() {
    elev_set_door_open_lamp(1);
    timer_start(); 
}


void fsm_EMERGENCY_STOP() {
    elev_set_stop_lamp(1); 
    elev_set_motor_direction(DIRN_STOP); 

    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){ 
        elev_set_floor_indicator(floor); 
    }
}


void fsm_IDLE() {
    elev_set_motor_direction(DIRN_STOP); 

    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){ 
        elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
        //For å unngå å skru av lys i knapper som ikke eksisterer
        if(floor != N_FLOORS-1){
            elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
        }
        if(floor != 0){
            elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
        }
    }    
}


void fsm_RUN() {
    //Hånterer kjøreretning ved nødstopp mellom to etasjer
    if(elev_get_floor_sensor_signal() == -1){
        if(last_floor < orders[0].floor){
            elev_set_motor_direction(1);
        }
        else if(last_floor > orders[0].floor){
            elev_set_motor_direction(-1);
        }
        else if(last_floor==orders[0].floor){
            if(last_dir==1){
                elev_set_motor_direction(-1);
            }
            else if(last_dir==-1){
                elev_set_motor_direction(1);
            }
        }
    }
    else{
        elev_set_motor_direction(get_direction());
    }
}


void elevator_init() { 
    while(elev_get_floor_sensor_signal() == -1) {
        elev_set_motor_direction(DIRN_UP);
    }
    elev_set_motor_direction(DIRN_STOP);
}


int get_direction(){
    int floor = elev_get_floor_sensor_signal();
    if(orders[0].floor>floor){
        return 1;
    }

    if(orders[0].floor<floor){
        return -1;
    }   
    return 0;
}
