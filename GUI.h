#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Map.h"
#include "Player.h"

// Obs.: Tiles corresponds to Lost Woods tileset, which includes forest, grass, hole and warp (in this order)
typedef enum {
	GUI_Title, GUI_Tiles, GUI_Link, GUI_Monster, GUI_Rupee, GUI_Heart, GUI_Swords, GUI_Pendants
} GUI_Images;

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
//	Creates game window (using screenWidth and screenHeight).
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
//	Draws map on screen.
//------------------------------------------------------------
	void GUI_DrawMap(Map *map, ALLEGRO_BITMAP *image);

//------------------------------------------------------------
//	Draws player on screen.
//------------------------------------------------------------
	void GUI_DrawPlayer(Player &player);

int GUI_GetScreenHeight();
int GUI_GetScreenWidth();

ALLEGRO_BITMAP *GUI_GetImage(int index);
ALLEGRO_DISPLAY *GUI_GetDisplay();