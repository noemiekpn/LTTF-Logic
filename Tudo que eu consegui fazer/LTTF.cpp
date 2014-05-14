/*------------------------------------------------------------
	This module is responsible for graphic user interface
	administration as well as game logic.
------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include "Sensor.h"
#include "GUI.h"
#include "MapPublicInfo.h"
#include "MapSettings.h"
#include "Player.h" 
#include <iostream>
#include <exception>
using namespace std; 

enum STATES {MAIN, SOLVE, END};
enum KEYS	{UP, DOWN, LEFT, RIGHT};

void setSafeAround(Map *map, int x, int y){
	char temp[25];

	sprintf(temp,"safe(point(%d,%d))",x,y);
	if(PlCall(temp)){
		MAP_SetPositionSafetyStatus(map, MAP_GetMapWidth(map)*y+x, true);
	}

	sprintf(temp,"safe(point(%d,%d))",x+1,y);
	if(PlCall(temp)){
		MAP_SetPositionSafetyStatus(map, MAP_GetMapWidth(map)*y+(x+1), true);
	}

	sprintf(temp,"safe(point(%d,%d))",x-1,y);
	if (PlCall(temp)){
		MAP_SetPositionSafetyStatus(map, MAP_GetMapWidth(map)*y+(x-1), true);
	}

	sprintf(temp,"safe(point(%d,%d))",x,y+1);
	if (PlCall(temp)){
		MAP_SetPositionSafetyStatus(map, MAP_GetMapWidth(map)*(y+1)+x, true);
	}
	sprintf(temp,"safe(point(%d,%d))",x,y-1);
	if (PlCall(temp)){
		MAP_SetPositionSafetyStatus(map, MAP_GetMapWidth(map)*(y-1)+x, true);
	}
}

int main(void) {
	//------------------------------------------------------------
	//	GAME VARIABLES
	//------------------------------------------------------------
	bool endGame = false;
	bool render = true;
	bool firstRender = false;
	const int FPS = 5;
	int state = MAIN;

	FILE *gameLog = fopen("log.txt", "w");
	if(gameLog == NULL) {
		printf("ERROR: Failed to create log file\n");
		return -1;
	}

	//------------------------------------------------------------
	//	GAME OBJECTS
	//------------------------------------------------------------
	Player link;
	Map *lostWoods = MAP_LoadMap("map.txt");

	PLR_InitializePlayer(link);	
	MAP_LoadObjects(lostWoods, "objects.txt");
	/*MAP_PlaceObjectsRandom(lostWoods);*/
	MAP_PlaceObjects(lostWoods, "items.txt");

	//------------------------------------------------------------
	//	ALLEGRO VARIABLES
	//------------------------------------------------------------
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//------------------------------------------------------------
	//	PROLOG VARIABLES
	//------------------------------------------------------------
	char* argv[] = {"libswipl.dll", "-s", "linkMind.pl","-q", NULL}; /* -q takes out the Prolog intro */
	_putenv("SWI_HOME_DIR=C:\\Program Files (x86)\\swipl");
	int counter=0;
	char temp[30];
	int link_x,link_y;

	//------------------------------------------------------------
	//	INITIALIZE ALLEGRO
	//------------------------------------------------------------
	if(!al_init()) {
		printf("ERROR: Failed to initialize Allegro.\n");
		return -1;
	}

	GUI_InitializeAllegroAddons();
	
	//------------------------------------------------------------
	//	INITIALIZE PROLOG
	//------------------------------------------------------------
	 
	PlEngine e(4, argv ); /* Turn on Prolog "engine" */

	// Link's start position 30,29
	PlCall("attlink(point(21,38)).");
	fprintf(gameLog, "# Assert Link's start point:\tattlink(point(21,38)).\n");
	
	// Assert forest points in Prolog
	for(int i = 0; i < MAP_GetMapWidth(lostWoods); i++) {
		for(int j = 0; j < MAP_GetMapHeight(lostWoods); j++) {
			if(MAP_TileForest == MAP_GetPositionTerrain(lostWoods, MAP_GetMapWidth(lostWoods)* i + j)){
				sprintf(temp,"setWood(point(%d,%d)).", j, i);
				//fprintf(gameLog, "# Assert a forest point:\tsetWood(point(%d,%d)).\n", j, i);
				if(!PlCall(temp)){
					printf("Could not finish setting wood points");
				}

			}
		}
	}

	//int abcd;
	printf("\n Sai do initialize PROLOG");
	//scanf("%d",&abcd);
	//------------------------------------------------------------
	//	INITIALIZE GUI
	//------------------------------------------------------------
	GUI_CreateDisplay();
	GUI_LoadImages();
	GUI_LoadFonts();
	
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
			} else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				if(state == END)
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
		else if(event.type == ALLEGRO_EVENT_TIMER ) {
			bool static detected=false;
			if (detected){
				int abcd;
				//scanf("%d",&abcd);
				detected=false;
			}
			if(state==MAIN){
				//do nothing
			}
			else if(!link.alive || link.realSword) {
				state = END;
			} else if (firstRender) {
				firstRender=false; counter++;

				//------------------------------------------------------------
				// STEP 1: Alert all there is in current position
				//------------------------------------------------------------
				for(int i=0; i<10000;i++)
					fprintf(gameLog, ".");
				fprintf(gameLog, "\n");
				PlTerm av;
				av = PlCompound("X");
				fprintf(gameLog, "# Query Link's point:\tlink(X).\n");
				if(!PlCall("link", av)) {
					printf("Error while trying to get Link's position.\n");
				}
				fprintf(gameLog, "# Answer Link's point:\t%s.\n",(char*)av);
				//printf("# Answer Link's point:\t%s.\n",(char*)av);
				link_x= (int)av[1];
				link_y= (int)av[2];

				/*Updating C database of Prolog's safe points*/
				setSafeAround(lostWoods, link_x, link_y);

				if(SNS_DetectDistortion(lostWoods, link_x, link_y)) {
					/*Att if it was setted true*/
					sprintf(temp,"setDistortion(point(%d,%d)).", link_x, link_y);
					fprintf(gameLog, "\t# Assert distortion point:\tsetDistortion(point(%d,%d)).\n", link_x, link_y);
					try{
						PlCall(temp);

					}catch(PlException ex){
						printf("SetingDs");
						cerr << (char *) ex << endl;
						exit(1);
					}
				}
				if(SNS_DetectNoise(lostWoods, link_x, link_y)) {
					sprintf(temp,"setNoise(point(%d,%d)).",link_x,link_y);
					fprintf(gameLog, "\t# Assert noise point:\tsetNoise(point(%d,%d)).\n", link_x, link_y);
					PlCall(temp);
				}else{
					/*Att if it was setted true_Monster Killed*/
					sprintf(temp,"hasnoise(point(%d,%d)).", link_x, link_y);
					if(PlCall(temp)){
						sprintf(temp,"retract(hasnoise(point(%d,%d))).", link_x, link_y);
						try{
							PlCall(temp);
						}catch(PlException ex){
							printf("Setting noise");
							cerr << (char *) ex << endl;
							exit(1);
					}
					}
				}
				if(SNS_DetectBreeze(lostWoods, link_x, link_y)) {
					sprintf(temp,"setBreeze(point(%d,%d)).",link_x,link_y);
					fprintf(gameLog, "\t# Assert breeze point:\tsetBreeze(point(%d,%d)).\n", link_x, link_y);
					PlCall(temp);
				}
				if(SNS_DetectPendantsVirtue(lostWoods, link_x, link_y)) {
					sprintf(temp,"setBrightness(point(%d,%d)).",link_x,link_y);
					fprintf(gameLog, "\t# Assert pendants brightess point:\tsetBrightness(point(%d,%d)).\n", link_x, link_y);
					PlCall(temp);
				}
				if(SNS_DetectBrightness(lostWoods, link_x, link_y)) {
					sprintf(temp,"setRbrightness(point(%d,%d)).",link_x,link_y);
					fprintf(gameLog, "\t# Assert rupee brightess point:\tsetRBrightness(point(%d,%d)).\n", link_x, link_y);
					PlCall(temp);
				}
				if(SNS_DetectFairy(lostWoods, link_x, link_y)) {
					sprintf(temp,"setFairy(point(%d,%d)).", link_x, link_y);
					fprintf(gameLog, "\t# Assert fairy point:\tsetFairy(point(%d,%d)).\n", link_x, link_y);
					PlCall(temp);
				} /* End of step 1 if */
				fprintf(gameLog, "\n"); 
				//------------------------------------------------------------
				//	STEP 2: Query for best action
				//------------------------------------------------------------
				PlTerm *gv= new PlTerm;
				//gv = PlCompound("X");
				fprintf(gameLog, "# Query best action:\t best_action(%s).\n",(char*)*gv);
				printf("\nBaTuDUm\n");
				//if(!PlCall("best_action", gv)) {
				//	printf("Error while trying to get best action.\n");
				//	fprintf(gameLog, "\t# Best action was:\t%s\n\n", (char*)gv);
				//	exit(1);
				//}
				try{
					//printf("\n >>>counter__%d_",counter);
					if(!PlCall("best_action", *gv)){
						printf("Error while trying to get best action.\n");
					}
				}
				catch(PlException ex){
					printf("\n counter__%d__%s",counter,(char*)*gv);
					cerr << (char *) ex << endl;
					int abcd;
					scanf("%d",&abcd);
					exit(1);
				}
				printf("\nBaTuDUm\n");
				fprintf(gameLog, "\t# Best action was:\t%s\n\n", (char*)*gv);
				printf("\t# Best action was:\t%s\n\n", (char*)*gv);


				if(!strcmp((char*)gv->name(), "game_over_lost")) {
					link.alive = false;
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);

				} else if(!strcmp((char*)gv->name(), "game_over_won")) {
					link.realSword = true;
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);

				} else if(!strcmp((char*)gv->name(), "game_over_dilema")) {
					state = END;
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);
				} else if(!strcmp((char*)gv->name(), "takeRupee")) {
					PLR_CollectRupee(lostWoods,link);
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);

					if(!PlCall("call", *gv)){
						printf("No such call :%s\n",(char*)*gv);
						exit(1);
					}
				} else if(!strcmp((char*)gv->name(), "takeHeart")) {
					PLR_CollectHeart(lostWoods,link);
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);

					if(!PlCall("call", *gv)){
						printf("No such call:%s\n",(char*)*gv);
						exit(1);
					}
				} else if(!strcmp((char*)gv->name(),"takeSword")) {
					PLR_CollectMasterSword(lostWoods,link);	
					fprintf(gameLog,"Exec'n best_action: %d",(char*)*gv);

					if(!PlCall("call", *gv)){
						printf("No such call:%s\n",(char*)*gv);
						exit(1);
					}
				
					// Verifying the sword...
					if(MAP_SearchPositionObject(lostWoods, MAP_GetMapWidth(lostWoods) * link_y + link_x, MAP_ObjRealSword) != -1) {
						sprintf(temp, "realSword(point(%d,%d)).", link_x, link_y);
						fprintf(gameLog, "\t# Assert real sword point:\t %s.",temp);
					
						*gv = PlCompound(temp);
					
						if(!PlCall("realSword", *gv)){
							printf("No such call.\n");
							exit(1);
						}
					}
				
					PLR_CollectMasterSword(lostWoods,link);
				} else if(!strcmp((char*)gv->name(), "walk")) {
					int x = (int)(*gv)[1][1];
					int y = (int)(*gv)[1][2];
					//printf("\ntentando mover o link para %d %d",x,y);

					PLR_MoveTo(lostWoods, link, x, y);
					detected=true;
					fprintf(gameLog, "\t\t#Exec'n best_action:\t%s\n\n", (char*)*gv);
					printf( "\t\t#Exec'n best_action:\t%s\n\n", (char*)*gv);
					//if(!PlCall("call", gv)){
					//	printf("No such call.\n");
					//	exit(1);
					//}
					
					try{
						printf("\n counter%d\tonde:%s",counter,(char*)*gv );
						PlCall("call",*gv);
						printf("\n oie_%s",(char*)*gv);
					}
					catch(PlException ex){
						printf("\n counter__%d_",counter);
						cerr << (char *) ex << endl;
						exit(1);
					}

				} else if(!strcmp((char*)gv->name(), "attackMonster")) { /*tem que implementar a retirada das detecções adjacentes ao (SE) falecido*/
					printf("\n ATACA OOO \n");
					int x = (int)gv[1][1];
					int y = (int)gv[1][2];
				
					if(!PlCall("call",*gv)){
						printf("No such call.\n");
						exit(1);
					}

					// If monster was really killed (if Link hit the right position)...
					if(PLR_AttackEnemy(lostWoods, link, x, y)) {
						if(!PlCall("killedMonster", gv[1])) {
							printf("No such call.\n");
							exit(1);
						} else {
							fprintf(gameLog, "\t# Assert killed monster point:\tkilledMonster(point(%d,%d)).", x, y);
						}
					} else {
						if(!PlCall("notKilledMonster",gv[1])){
							printf("No such call.\n");
							exit(1);
						} else {
							fprintf(gameLog, "\t# Assert not killed monster point:\tnotKilledMonster(point(%d,%d)).", x, y);
						}
					}
				} /* End of step 2 if */

				delete gv;
				//------------------------------------------------------------
				// STEP 3: Alert reactions to Link's current position
				//------------------------------------------------------------
				PlTerm *cv= new PlTerm;
				*cv = PlCompound("X");
				fprintf(gameLog, "# Query Link's point:\tlink(%s).\n",(char*)*cv);

				if(!PlCall("link", *cv)) {
					printf("Error while trying to get Link's position.\n");
				}
						
				link_x= (int)(*cv)[1];
				link_y= (int)(*cv)[2];

				// Is there monster here?
				if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y + link_x, MAP_ObjMonster)!= -1){
					PLR_BeAttackedByEnemy(link);

					if(!PlCall("withMonster", *cv)){
						printf("No such call.\n");
						exit(1);
					} else {
						fprintf(gameLog, "\t# Assert monster point:\twithMonster(point(%d,%d)).", link_x, link_y);
					}
				}
			
				// Is there warp here?
				else if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y + link_x, MAP_ObjWarp) != -1){
					/*Graphics resolution first*/
					PLR_FallIntoWarp(lostWoods,link);

					/*Prolog Knowledge changes*/
					PlTermv bv(2);
					bv[0] = *cv;
					delete cv;
					sprintf(temp,"point(%d,%d)", link.posX, link.posY); //New Position
				
					bv[1]=PlCompound(temp);

					if(!PlCall("inVortex", bv)){
						printf("No such call.\n");
						exit(1);
					} else {
						fprintf(gameLog, "\t# Assert vortex point:\tinVortex(point(%d,%d)).", link_x, link_y);
					}
				}
				// Is there hole here? 
				else if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y + link_x, MAP_ObjHole) != -1){
					PLR_FallIntoHole(link);
				
					if(!PlCall("inHole", *cv)){
						printf("No such call.\n");
						exit(1);
					} else {
						fprintf(gameLog, "\t# Assert hole point:\tinHole(point(%d,%d)).", link_x, link_y);
					}
				}
			} /* End of step 3 if */

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
				firstRender=true;
				// Draw the background
				GUI_DrawMainBackground(lostWoods, link);
			}

			if(state == END) {
				GUI_DrawEndResults(link);
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
	MAP_DestroyMap(lostWoods);
	GUI_DestroyFonts();
	GUI_DestroyImages();
	GUI_DestroyDisplay();

	fclose(gameLog);

	return 0;
}




