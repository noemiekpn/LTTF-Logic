#include <stdio.h>
#include <math.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

#include "GUI.h"

#define NUM_IMGS 8
#define MAP_SIZE 42

//------------------------------------------------------------
//	GLOBAL VARIABLES
//------------------------------------------------------------
	bool initAddons = false;

	ALLEGRO_BITMAP *images[NUM_IMGS];
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_FONT* retganon;

//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
	void DrawMap(Map *map, ALLEGRO_BITMAP *image);
	void DrawPlayer(Player &player, ALLEGRO_BITMAP *image);

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
		images[GUI_ImgTitle] = al_load_bitmap("main_title.png");
		images[GUI_ImgTiles] = al_load_bitmap("lost_woods_tiles.png");
		images[GUI_ImgLink] = al_load_bitmap("link_DURL.png");
		images[GUI_ImgMonster] = al_load_bitmap("hoarder.png");
		images[GUI_ImgRupee] = al_load_bitmap("rupee.png");
		images[GUI_ImgHeart]= al_load_bitmap("heart.png");
		images[GUI_ImgSwords]= al_load_bitmap("swords.png");
		images[GUI_ImgPendants] = al_load_bitmap("pendants.png");

		// Mask out background
		al_convert_mask_to_alpha(images[GUI_ImgLink], al_map_rgb(0, 128, 128));			// Cyan
		al_convert_mask_to_alpha(images[GUI_ImgMonster], al_map_rgb(153, 153, 102));		// Beige
		al_convert_mask_to_alpha(images[GUI_ImgPendants], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[GUI_ImgRupee], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[GUI_ImgHeart], al_map_rgb(255, 0, 255));			// Magenta
		al_convert_mask_to_alpha(images[GUI_ImgSwords], al_map_rgb(255, 0, 255));			
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

		// Main title logo
		al_draw_bitmap(images[GUI_ImgTitle], 228, 214, 0);
		al_draw_text(retganon, al_map_rgb(255, 255, 255), 430, 750, 0,"PRESS ENTER TO BEGIN");	
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_DrawMainBackground(Map *map, Player &player) {
	int size = MAP_GetMapWidth(map) * MAP_GetMapHeight(map);
	int mapColumns = MAP_GetMapWidth(map);

	DrawMap(map, images[GUI_ImgTiles]);
	DrawPlayer(player, images[GUI_ImgLink]);

	// Set player's initial position to a visited position
	MAP_SetPositionVisitStatus(map, player.posY * mapColumns + player.posX, true);

	// If position is unvisited, a "fog" will cover
	if(initAddons) {
		for(int column = 0; column < size; column++) {
			if(!MAP_GetPositionVisitStatus(map, column)) {
				al_draw_filled_rectangle(tileSize * (column % mapColumns), tileSize * (column / mapColumns),
					tileSize * (column % mapColumns) + tileSize, tileSize * (column / mapColumns) + tileSize,
					al_map_rgba(0, 0, 0, 150));
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

ALLEGRO_DISPLAY *GUI_GetDisplay() {
	return display;
}

//------------------------------------------------------------
//	AUX FUNCTIONS
//------------------------------------------------------------
void DrawMap(Map *map, ALLEGRO_BITMAP *image) {
	int size = MAP_GetMapWidth(map) * MAP_GetMapHeight(map);
	int mapColumns = MAP_GetMapWidth(map);
	
	if(initAddons) {
		for(int column = 0; column < size; column++) {
			char terrain = MAP_GetPositionTerrain(map, column);

			al_draw_bitmap_region(image, terrain * tileSize, 0, tileSize, tileSize, 
				tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
		}
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void DrawPlayer(Player &player, ALLEGRO_BITMAP *image) {
	int frameX = (player.curFrame % player.animationColumns) * player.frameWidth;
	int frameY = player.animationRow * player.frameHeight;

	al_draw_bitmap_region(image, frameX, frameY, player.frameWidth, 
		player.frameHeight, player.posX * tileSize, player.posY * tileSize, 0);
}