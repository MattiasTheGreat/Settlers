#ifndef FPS_COUNTER
#define FPS_COUNTER

#include <time.h>

//klassl�s och i header ;D detta hade inte labbledarna gillat. eller stackoverflow. 


int fpsCounter(clock_t &current_time, clock_t &previous_time) {
	//deklarera 
	// clock_t current_time = clock();
	// clock_t previous_time = clock();
	// i main

	// current_time beh�vs inte dock nedan. 

	current_time = clock();

	double tickdiff = current_time - previous_time;

	previous_time = clock();		//sparar till n�sta v�nda

	return (int) (CLOCKS_PER_SEC / tickdiff);
}


int tickDiff(clock_t &previous_time) {
	return (clock() - previous_time);
}


int timeDiff_ms(clock_t &previous_time) {
	
	return (int) (1000 / (CLOCKS_PER_SEC / (double) tickDiff(previous_time))); // 1000ms / fps = ms per frame , wow detta beh�ver b�ttre struktur. den �r testad dock. 
}

#endif