#include <stdio.h>
#include <math.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "GUI.h"

#define NUM_IMGS 9
#define MAP_SIZE 42

//------------------------------------------------------------
//	GLOBAL VARIABLES
//------------------------------------------------------------
const int tileSize = 20;	// 20 x 20 pixel tiles
const int screenWidth = 1200;
const int screenHeight = 840;

bool initAddons = false;

ALLEGRO_BITMAP *images[NUM_IMGS];
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT* retganon;

void GUI_InitializeAllegroAddons() {
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();

	initAddons = true;
}

void GUI_CreateDisplay() {
	display = al_create_display(screenWidth, screenHeight);
	
	if(!display) {
		printf("Failed to initialize display.\n");
		return;
	}
}

void GUI_DestroyDisplay() {
	al_destroy_display(display);
}

void GUI_LoadImages() {
	if(initAddons) {
		images[TITLE] = al_load_bitmap("main_title.png");
		images[TILES] = al_load_bitmap("lost_woods_tiles.png");
		images[LINK] = al_load_bitmap("link_DURL.png");
		images[MONSTER] = al_load_bitmap("hoarder.png");
		images[RUPEE] = al_load_bitmap("rupee.png");
		images[HEART]= al_load_bitmap("heart.png");
		images[SWORDS]= al_load_bitmap("swords.png");
		images[VORTEX] = al_load_bitmap("vortex.png");
		images[PENDANTS] = al_load_bitmap("pendants.png");

		// Mask out background
		al_convert_mask_to_alpha(images[LINK], al_map_rgb(0, 128, 128));			// Cyan
		al_convert_mask_to_alpha(images[MONSTER], al_map_rgb(153, 153, 102));		// Beige
		al_convert_mask_to_alpha(images[PENDANTS], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[RUPEE], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[SWORDS], al_map_rgb(255, 0, 255));			// Magenta
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_DestroyImages() {
	for(int i = 0; i < NUM_IMGS; i++) {
		al_destroy_bitmap(images[i]);
	}
}

void GUI_LoadFonts(){
	if(initAddons) {
		retganon = al_load_font("retganon.ttf", 48, 0);
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_DestroyFonts() {
	al_destroy_font(retganon);
}

void GUI_DrawMainTitle() {
	if(initAddons) {
		// Main title logo
		al_draw_bitmap(images[TITLE], 228, 214, 0);
		al_draw_text(retganon, al_map_rgb(255, 255, 255), 430, 750, 0,"PRESS ENTER TO BEGIN");	

		// Background rectangle
		al_draw_filled_rectangle(0, 120, screenWidth, screenHeight - 140, al_map_rgb(128, 144, 216));

		int height = 168;
		int posX = 592, posY = 214;

		// Triangle 1
		al_draw_filled_triangle(posX, posY, posX - height / sqrt(3.0), posY + height, posX + height / sqrt(3.0), posY + height, al_map_rgb(247, 206, 10));
	
		// Triangle 2
		posY = posY + height;
		posX = posX - height / sqrt(3.0);
		al_draw_filled_triangle(posX, posY, posX - height / sqrt(3.0), posY + height, posX + height / sqrt(3.0), posY + height, al_map_rgb(247, 206, 10));

		// Triangle 3
		posX = posX + 2 * height / sqrt(3.0);
		al_draw_filled_triangle(posX, posY, posX - height / sqrt(3.0), posY + height, posX + height / sqrt(3.0), posY + height, al_map_rgb(247, 206, 10));
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_DrawMap(Map *map, ALLEGRO_BITMAP *image) {
	int size = MAP_GetMapWidth(map) * MAP_GetMapHeight(map);
	char *terrains = MAP_GetMapTerrains(map);
	
	int mapColumns = MAP_GetMapWidth(map);
	
	if(initAddons) {
		for(int column = 0; column < size; column++) {
			al_draw_bitmap_region(image, terrains[column] * tileSize, 0, tileSize, tileSize, 
				tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
		}
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_HideUnknownBackground(Map *map) {
	int size = MAP_GetMapWidth(map) * MAP_GetMapHeight(map);
	
	if(initAddons) {
		for(int line = 0; line < size; line++) {
			for(int column = 0; column < size; column++) {
				al_draw_filled_rectangle(line * GUI_GetTileSize(), column *GUI_GetTileSize(), 
					line * GUI_GetTileSize() + GUI_GetTileSize(), column *GUI_GetTileSize() + GUI_GetTileSize(),
					al_map_rgba(255, 255, 255, 50));
			}
		}
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

int GUI_GetScreenHeight() {
	return screenHeight;
}

int GUI_GetScreenWidth() { 
	return screenWidth;
}

int GUI_GetTileSize() {
	return tileSize;
}

ALLEGRO_BITMAP *GUI_GetImage(int index) {
	return images[index];
}

ALLEGRO_DISPLAY *GUI_GetDisplay() {
	return display;
}