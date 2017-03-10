#include "Elevmodule.h"

int nOrders;

typedef struct order {
	int floor;
	int direction;
} order; 

//Antall bestillingsknapper  
#define orderButtons 1+3*(N_FLOORS-1)  

//Kølisten
order orders[orderButtons]; 

//Initialiserer antall bestillinger 
void queue_nOrders_init();

//Initialiserer køen hvor hver bestilling settes til verdi orderButtons+1
void queue_init();

//Sjekker om bestilt bestilling allerede ligger i køen 
int queue_order_not_same();

//Forskyver bestillingene i køen slik at alle bestillingene ligger etterhverandre uten "hull" (forekommer ved sletting)
void queue_update();

void queue_add_order();

void queue_delete_all_orders();

void queue_delete_order();

order queue_next_order();

void queue_print();








