/*------------------------------------------------------------
	This module is responsible for graphic user interface
	administration as well as game logic.
------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>

#include "GUI.h"
#include "Map.h"
#include "Player.h" 

enum STATES {MAIN, SOLVE, END};
enum KEYS	{UP, DOWN, LEFT, RIGHT};

int main(void) {
	//------------------------------------------------------------
	//	GAME VARIABLES
	//------------------------------------------------------------
	bool endGame = false;
	bool render = true;
	bool dirKeys[] = {false, false, false, false, false};

	const int FPS = 60;
	int state = MAIN;

	Player link;
	Map *lostWoods = MAP_LoadMap("map.txt");

	//------------------------------------------------------------
	//	ALLEGRO VARIABLES
	//------------------------------------------------------------
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//------------------------------------------------------------
	//	INITIALIZE & TEST ALLEGRO LIBRARY
	//------------------------------------------------------------
	if(!al_init()) {
		printf("ERROR: Failed to initialize Allegro.\n");
		return -1;
	}

	//------------------------------------------------------------
	//	INITIALIZE ADD-ONS
	//------------------------------------------------------------
	GUI_InitializeAllegroAddons();
	
	//------------------------------------------------------------
	//	INITIALIZE GUI & PLAYER
	//------------------------------------------------------------
	GUI_CreateDisplay();
	GUI_LoadImages();
	GUI_LoadFonts();
	PLR_InitializePlayer(link, GUI_GetImage(LINK));
	// Create timer
	timer = al_create_timer(1.0 / FPS);
	
	// Event queue will register input events
	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(GUI_GetDisplay()));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

	//-----------------------------------------------------------
	//	GAME LOOP
	//-----------------------------------------------------------
	al_start_timer(timer);
	while(!endGame) {
		ALLEGRO_EVENT event;
		al_wait_for_event(eventQueue, &event);

		//------------------------------------------------------------ 
		//	KEYBOARD INPUT
		//------------------------------------------------------------
		if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if(event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				state = SOLVE;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				endGame = true;
			}
		} /* End of key down if */

		//------------------------------------------------------------ 
		//	CLOSE WINDOW
		//------------------------------------------------------------
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				endGame = true;
		} /* End of display close if */

		//------------------------------------------------------------ 
		//	GAME UPDATE
		//------------------------------------------------------------
		else if(event.type == ALLEGRO_EVENT_TIMER) {
			render = true;
			
		} /* End of timer if */
		
		//------------------------------------------------------------ 
		//	DRAWING TO SCREEN
		//------------------------------------------------------------
		if(render && al_is_event_queue_empty(eventQueue)) {
			render = false;
			
			if(state == MAIN) {
				GUI_DrawMainTitle();
			}

			if(state == SOLVE) {
				// Draw the map
				GUI_DrawMap(lostWoods, GUI_GetImage(TILES));

				// Draw Link
				PLR_DrawPlayer(link);

			}

			if(state == END) {
				endGame = true;
			}

			al_flip_display();	
			al_clear_to_color(al_map_rgb(0, 0, 0));
		} /* End of render event if*/
	} /* End of game while*/

	//------------------------------------------------------------
	//	MEMORY CLEANUP
	//------------------------------------------------------------
	al_destroy_timer(timer);
	al_destroy_event_queue(eventQueue);
	GUI_DestroyFonts();
	GUI_DestroyImages();
	GUI_DestroyDisplay();

	return 0;
}




