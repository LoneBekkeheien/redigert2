#include "Elevmodule.h"
#include "Eventmanager.h"
#include "Timer.h"
#include "Queue.h"

int ev_emergency_button_pushed(){ 
    return elev_get_stop_signal();
}


int ev_time_door_out(){ 
    return timer_time_out();
}


int ev_orders_in_queue(){ 
    if (nOrders != 0){
        return 1;
    }
    return 0;
}

 
int ev_order_same_floor(){ 
    int floor = elev_get_floor_sensor_signal();
    if(orders[0].floor==floor){
        return 1;
    }
    
    for(int i=0; i<nOrders; i++){ 
        if(orders[i].floor==floor && orders[i].direction==0){ 
            return 1;
        }
    }
    
    for(int i=0; i<nOrders; i++){ 
        if(orders[i].floor==floor && orders[i].direction == get_direction()){ 
            return 1;
        }
    } 
    return 0;
}



