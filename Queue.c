#include "Elevmodule.h"
#include "Queue.h"
#include <stdio.h> 

void queue_nOrders_init(){
	nOrders=0;
}


void queue_init(){
	for(int i=0; i<orderButtons; i++){
        orders[i].floor=(orderButtons+1);
        orders[i].direction=(orderButtons+1); 
    }    
}


void queue_print(){
	printf("-------------------------------------\n");
	for(int i=0; i<orderButtons; i++)
     {
        printf("Bestilling: etasje %d", orders[i].floor);
        printf(", retning %d \n", orders[i].direction);
     }
}


order queue_next_order(){
	return orders[0];	
}


void queue_update(){ 
	for(int i = 0; i < orderButtons; i++){
		if(orders[i].floor == orderButtons + 1){ 
			for(int j = i+1; j < orderButtons; j++){
				if(orders[j].floor != orderButtons+1){
					orders[i].floor = orders[j].floor;
					orders[i].direction = orders[j].direction;
					orders[j].floor = orderButtons + 1;
					orders[j].direction = orderButtons + 1;
					break;
				}
			}
		}
	}
}


int queue_order_not_same(int direction, int Floor){
	for(int i=0; i<nOrders; i++){
		if(orders[i].direction==direction && orders[i].floor==Floor){
			return 0;
		}
	}
	return 1;
}


void queue_add_order(){ 
	//Deler opp for å unngå å sette knapper som ikke eksisterer
	for(int i=0; i< N_FLOORS; i++) {
		if(elev_get_button_signal(BUTTON_COMMAND,i)==1){
			if(queue_order_not_same(0,i)==1){
				orders[nOrders].floor=i;
				orders[nOrders].direction=0;
				elev_set_button_lamp(BUTTON_COMMAND, i, 1);
				nOrders+=1;
			}
		}
	}
	
	for(int i = 0; i < N_FLOORS-1; i++){
		if(elev_get_button_signal(BUTTON_CALL_UP,i)==1){
			if(queue_order_not_same(1,i)==1){
				orders[nOrders].floor=i;
				orders[nOrders].direction=1;
				elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
				nOrders+=1;
			}
		}
		if(elev_get_button_signal(BUTTON_CALL_DOWN,i+1)==1){	
			if(queue_order_not_same(-1,i+1) == 1){
					orders[nOrders].floor=i+1;
					orders[nOrders].direction=-1;
					elev_set_button_lamp(BUTTON_CALL_DOWN, i+1, 1);
					nOrders+=1;
			}
		}	
	}
}


void queue_delete_order(){ 
	for(int i=0; i<orderButtons; i++){
		if(elev_get_floor_sensor_signal() == orders[i].floor){
			orders[i].floor = orderButtons + 1;
			orders[i].direction = orderButtons + 1;
			nOrders -= 1; 
		}
	}
}


void queue_delete_all_orders(){
	for(int i=0; i<orderButtons; i++){
		orders[i].floor = orderButtons + 1;
		orders[i].direction = orderButtons + 1;
	}
	nOrders=0;	
	for(int i = 0; i < N_FLOORS; i++){
        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
	}
	for(int i = 0; i < N_FLOORS-1; i++){
		elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
		elev_set_button_lamp(BUTTON_CALL_DOWN, i+1, 0);
	}
}


