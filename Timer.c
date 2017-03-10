#include "Timer.h"

void timer_start() {
	timer_door_opened_ms = time(NULL) * 1000;
}

int timer_time_out() {
	time_t timer_now_ms = time(NULL) * 1000;

	if(timer_now_ms < timer_door_opened_ms + 3000){
		return 0;
	}
	return 1;
}

