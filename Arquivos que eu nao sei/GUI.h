#include <allegro5\allegro.h>

#include "MapPublicInfo.h"
#include "MapPrivateInfo.h"
#include "Player.h"

const int tileSize = 20;	// 20 x 20 pixel tiles
const int screenWidth = 1200;
const int screenHeight = 840;

//------------------------------------------------------------
//	Initializes all required allegro add-ons for game.
//	This function must be called before using any GUI
//	functions.
//------------------------------------------------------------
	void GUI_InitializeAllegroAddons();

//------------------------------------------------------------
//	Creates game window.
//------------------------------------------------------------
	void GUI_CreateDisplay();

//------------------------------------------------------------
//	Destroys game window.
//------------------------------------------------------------
	void GUI_DestroyDisplay();

//------------------------------------------------------------
//	Loads all game images.
//------------------------------------------------------------
	void GUI_LoadImages();

//------------------------------------------------------------
//	Destroys all game images.
//------------------------------------------------------------
	void GUI_DestroyImages();

//------------------------------------------------------------
//	Loads all game fonts.
//------------------------------------------------------------
	void GUI_LoadFonts();
	
//------------------------------------------------------------
//	Destroys all game fonts.
//------------------------------------------------------------	
	void GUI_DestroyFonts();

//------------------------------------------------------------
//	Draws main tile on screen.
//------------------------------------------------------------
	void GUI_DrawMainTitle();

//------------------------------------------------------------
//	Draws map and player on screen.
//------------------------------------------------------------
	void GUI_DrawMainBackground(Map *map, Player &player);

//------------------------------------------------------------
//	Draws final results, whether Link died or found the
//	real Master Sword. Always shows final cost.
//------------------------------------------------------------
	void GUI_DrawEndResults(Player &player);

//------------------------------------------------------------
//	Returns game display.
//------------------------------------------------------------
	ALLEGRO_DISPLAY *GUI_GetDisplay();