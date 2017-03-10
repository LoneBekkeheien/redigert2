//Returnerer 1 hvis stoppknappen er trykket 
int ev_emergency_button_pushed();

//Returnerer 1 når døren har vært åpen i 3 sekunder
int ev_time_door_out();

//Returnerer 1 hvis det er bestillinger i køen
int ev_orders_in_queue();

//Returnerer 1 hvis andre bestillinger stemmer med bestillingen som utføres (retning og etasje)
int ev_order_same_floor();





