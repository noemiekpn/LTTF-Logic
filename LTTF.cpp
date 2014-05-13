/*------------------------------------------------------------
	This module is responsible for graphic user interface
	administration as well as game logic.
------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include <SWI-cpp.h>
#include "GUI.h"
#include <string.h>
#include "MapSettings.h"
#include "MapPublicInfo.h"
#include "MapPrivateInfo.h"
#include "Player.h"
#include "stdlib.h"
#include <cstdio>

enum STATES {MAIN, SOLVE, END};
enum KEYS	{UP, DOWN, LEFT, RIGHT};


bool detectDistortion(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjWarp)!=-1)
		return true;
	else
		return false;
}
bool detectBreeze(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjHole)!=-1)
		return true;
	else
		return false;
}
bool detectNoise(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjMonster)!=-1)
		return true;
	else
		return false;
}
bool detectSBrightness(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjRealSword)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjFakeSword)!=-1)
		return true;
	else
		return false;
}
bool detectRBrightness(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y+x,MAP_ObjRupee)!=-1)
		return true;
	else
		return false;
}
bool detectFairy(Map* map,int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjHeart)!=-1)
		return true;
	else
		return false;
}

int main(void) {
	//------------------------------------------------------------
	//	GAME VARIABLES
	//------------------------------------------------------------
	bool endGame = false;
	bool render = true;

	const int FPS = 60;
	int state = END;

	//------------------------------------------------------------
	//	GAME OBJECTS
	//------------------------------------------------------------
	Player link;
	Map *lostWoods = MAP_LoadMap("map.txt");

	PLR_InitializePlayer(link);	
	MAP_LoadObjects(lostWoods, "objects.txt");
	MAP_PlaceObjectsRandom(lostWoods);
	/*MAP_PlaceObjects(lostWoods, "items.txt");*/
	//------------------------------------------------------------
	//	PROLOG VARIABLES
	//------------------------------------------------------------

	char* argv[] = {"libswipl.dll", "-s", "D:\\teste.pl","-q", NULL}; /* -q take out the prolog intro */
	_putenv("SWI_HOME_DIR=C:\\Program Files (x86)\\swipl");
	char temp[30];
	int link_x,link_y;
	//------------------------------------------------------------
	//	ALLEGRO VARIABLES
	//------------------------------------------------------------
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//------------------------------------------------------------
	//	INITIALIZE PROLOG
	//------------------------------------------------------------
	 
	PlEngine e( 4 , argv ); /*Turn on */

	/*Link position*/
	PlCall("attlink(point(38,21)).");
	for(int i; i<42;i++)
		for(int j; j<42;j++)
			if(MAP_TileForest == MAP_GetPositionTerrain(lostWoods, MAP_GetMapWidth(lostWoods)*i +j) ){
				sprintf(temp,"setWood(point(%d,%d))",i,j);
				PlCall(temp);
			}

	//------------------------------------------------------------
	//	INITIALIZE ALLEGRO
	//------------------------------------------------------------
	if(!al_init()) {
		printf("ERROR: Failed to initialize Allegro.\n");
		return -1;
	}

	GUI_InitializeAllegroAddons();
	
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
			} else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				PLR_TurnLeft(link);
			} else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				PLR_TurnRight(link);
			} else if(event.keyboard.keycode == ALLEGRO_KEY_UP) {
				PLR_MoveForward(lostWoods, link);
			} else if(event.keyboard.keycode == ALLEGRO_KEY_C) {
				PLR_CollectHeart(lostWoods, link);
			} else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
				PLR_FallIntoWarp(lostWoods, link);
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
			if(!link.alive || link.realSword) {
				state = END;
			}
		//	1) Passo: avisar tudo que há aqui
			PlTerm av;
			av= PlCompound("X");
			if(!PlCall("link",av))
				printf("Erro while trying to get link position");
			 link_x= (int)av[1];
			 link_y= (int)av[2];

			 if(detectDistortion(lostWoods,link_x,link_y)){
				sprintf(temp,"setDistortion(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }
			 if(detectNoise(lostWoods,link_x,link_y)){
				sprintf(temp,"setNoise(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }
			 if(detectBreeze(lostWoods,link_x,link_y)){
				sprintf(temp,"setBreeze(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }
			 if(detectSBrightness(lostWoods,link_x,link_y)){
				sprintf(temp,"setBrightness(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }
			 if(detectRBrightness(lostWoods,link_x,link_y)){
				sprintf(temp,"setRbrightness(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }
			 if(detectFairy(lostWoods,link_x,link_y)){
				sprintf(temp,"setFairy(point(%d,%d))",link_x,link_y);
				PlCall(temp);
			 }


		//	 2) Passo: pergunto mellhor ação
			 av= PlCompound("X");
			 if(!PlCall("best_action",av))
				printf("Erro while trying to get best_actio");

			 if(!strcmp((char*)av.name(),"game_over_lost")){
				 link.alive=false;
			 }
			 else if(!strcmp((char*)av.name(),"game_over_won")){
				  link.realSword=true;
			 }
			 else if(!strcmp((char*)av.name(),"game_over_dilema")){
				 //what to do?
			 }
			 else if(!strcmp((char*)av.name(),"takeRupee")){
				 PLR_CollectRupee(lostWoods,link);
				 //av= PlCompound("X");
				 if(!PlCall("call",av)){
					 printf("\n erro");
					 exit(1);
				 }
			 }
			 else if(!strcmp((char*)av.name(),"takeHeart")){
				 PLR_CollectHeart(lostWoods,link);
				 if(!PlCall("call",av)){
					 printf("\n erro");
					 exit(1);
				 }
			 }
			 else if(!strcmp((char*)av.name(),"takeSword")){
				 PLR_CollectMasterSword(lostWoods,link);	
				  if(!PlCall("call",av)){
					 printf("\n erro");
					 exit(1);
				 }
				/*Verifying the sword*/
				  if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y +link_x,MAP_ObjRealSword)!=-1){
					  sprintf(temp,"point(%d,%d)",link_x,link_y);
					  av=PlCompound(temp);
					  if(!PlCall("realSword",av)){
						printf("\n erro");
						exit(1);
					  }
				  }
				  PLR_CollectMasterSword(lostWoods,link);
			 }
			 else if(!strcmp((char*)av.name(),"walk")){ /*só fiz a parte do prolog*/
				 int x = (int)av[1][1];
				 int y = (int)av[1][2];
				 PLR_MoveTo(lostWoods,link, x, y);
				 if(!PlCall("call",av)){
					 printf("\n erro");
					 exit(1);
				 }

			 }
			 else if(!strcmp((char*)av.name(),"attackMonster")){ /*tem que implementar a retirada das detecções adjacentes ao (SE) falecido*/
				 int x = (int)av[1][1];
				 int y = (int)av[1][2];
				 if(!PlCall("call",av)){
					 printf("\n erro");
					 exit(1);
				 }
				 /*if realy killed*/
				 if (PLR_AttackEnemy(lostWoods,link, x, y)){
					 if(!PlCall("killedMonster",av[1])){
						printf("\n erro");
						exit(1);
					 }
				 }
				 else{ /*if not*/
					 if(!PlCall("notKilledMonster",av[1])){
						printf("\n erro");
						exit(1);
					 }
				 }

			 }

		//3) Avisar reações em relação a posição atual do link
			av= PlCompound("X");
			if(!PlCall("link",av))
				printf("Erro while trying to get link position");
			 link_x= (int)av[1];
			 link_y= (int)av[2];
			/*Is there monster here?*/
			if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y +link_x,MAP_ObjMonster)!=-1){

				PLR_BeAttackedByEnemy(link);

				if(!PlCall("withMonster",av)){
					 printf("\n erro");
					 exit(1);
				 }
			}
			/*Is there warp here?*/
			else if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y +link_x,MAP_ObjWarp)!=-1){
				/*Graphics resolution first*/
				PLR_FallIntoWarp(lostWoods,link);

				/*Prolog Knowledge changes*/
				PlTermv bv(2);
				bv[0]=av;
				sprintf(temp,"point(%d,%d)",link.posX,link.posY); //New Position
				bv[1]=PlCompound(temp);

				if(!PlCall("inVortex",bv)){
					 printf("\n erro");
					 exit(1);
				 }
			}
			/*Is there hole here?*/
			else if(MAP_SearchPositionObject(lostWoods,MAP_GetMapWidth(lostWoods)*link_y +link_x,MAP_ObjHole)!=-1){
				PLR_FallIntoHole(link);
				if(!PlCall("inHole",av)){
					 printf("\n erro");
					 exit(1);
				 }
			}
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

	return 0;
}




