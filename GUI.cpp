#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

#include "GUI.h"

#define NUM_IMGS 8
#define MAP_SIZE 42

/**
  * Image notes:
  *	1. Tiles image corresponds to Lost Woods tileset, which includes forest, grass, hole and warp, in this order
  * 2. Swords image corresponds to real Master Sword and fake Master Sword, in this order
  * 3. Pendants image corresponds to Pendant of Courage (green), Pendant of Power (blue) and Pendant of Wisdom (red), in this order
  */
typedef enum {
	GUI_ImgTitle, GUI_ImgTiles, GUI_ImgLink, GUI_ImgMonster, 
	GUI_ImgRupee, GUI_ImgHeart, GUI_ImgSwords, GUI_ImgPendants
} GUI_Img;

typedef struct dynamicImage {
	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth, frameHeight;

	ALLEGRO_BITMAP *image;
} DynamicImage;

//------------------------------------------------------------
//	GLOBAL VARIABLES
//------------------------------------------------------------
	bool initAddons = false;

	ALLEGRO_BITMAP *images[NUM_IMGS];
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_FONT* retganon;

	DynamicImage *dynHoarder = (DynamicImage*) malloc(sizeof(DynamicImage));
	DynamicImage *dynRupee = (DynamicImage*) malloc(sizeof(DynamicImage));
//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
	void SetDynamicImage(DynamicImage *img, int maxFrame, int curFrame, int frameCount, int frameDelay, 
		int frameWidth, int frameHeight, ALLEGRO_BITMAP *image);
	void DrawMap(Map *map, ALLEGRO_BITMAP *image);
	void DrawPlayer(Player &player, ALLEGRO_BITMAP *image);
	void DrawObjects(Map *map);
	void DrawHoarder(int posX, int posY);
	void DrawRupee(int posX, int posY);
	void UpdateAnimation(DynamicImage *img);

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
		al_convert_mask_to_alpha(images[GUI_ImgMonster], al_map_rgb(153, 153, 102));	// Beige
		al_convert_mask_to_alpha(images[GUI_ImgPendants], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[GUI_ImgRupee], al_map_rgb(153, 153, 102));
		al_convert_mask_to_alpha(images[GUI_ImgHeart], al_map_rgb(255, 0, 255));		// Magenta
		al_convert_mask_to_alpha(images[GUI_ImgSwords], al_map_rgb(255, 0, 255));

		// Set dynamic images
		SetDynamicImage(dynHoarder, 3, 0, 0, 40, tileSize, tileSize, images[GUI_ImgMonster]);
		SetDynamicImage(dynRupee, 3, 0, 0, 60, tileSize, tileSize, images[GUI_ImgRupee]);

	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void GUI_DestroyImages() {
	for(int i = 0; i < NUM_IMGS; i++) {
		al_destroy_bitmap(images[i]);
	}

	free(dynHoarder);
	free(dynRupee);
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
	DrawObjects(map);

	// Set player's position to a visited position
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
void SetDynamicImage(DynamicImage *img, int maxFrame, int curFrame, int frameCount, int frameDelay, 
	int frameWidth, int frameHeight, ALLEGRO_BITMAP *image) {
	img->maxFrame = maxFrame;
	img->curFrame = curFrame;
	img->frameCount = frameCount;
	img->frameDelay = frameDelay;
	img->frameWidth = frameWidth;
	img->frameHeight = frameHeight;
	img->image = image;
}

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

	if(initAddons) {
		al_draw_bitmap_region(image, frameX, frameY, player.frameWidth, 
			player.frameHeight, player.posX * tileSize, player.posY * tileSize, 0);
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void DrawObjects(Map *map) {
	int size = MAP_GetMapWidth(map) * MAP_GetMapHeight(map);
	int mapColumns = MAP_GetMapWidth(map);



	if(initAddons) {
		for(int column = 0; column < size; column++) {
			int numObjs = MAP_GetPositionNumObjects(map, column);
			int *objs = MAP_GetPositionObjects(map, column);

			for(int n = 0; n < numObjs; n++) {
				switch(objs[n]){
				// Static images
				case MAP_ObjRealSword:
					al_draw_bitmap_region(images[GUI_ImgSwords], 0 * tileSize, 0, tileSize, tileSize, 
						tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
					break;
				case MAP_ObjFakeSword:
					al_draw_bitmap_region(images[GUI_ImgSwords], 1 * tileSize, 0, tileSize, tileSize, 
						tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
					break;
				case MAP_ObjHeart:
					al_draw_bitmap(images[GUI_ImgHeart], tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
					break;
				case MAP_ObjHole:
					al_draw_bitmap_region(images[GUI_ImgTiles], 2 * tileSize, 0, tileSize, tileSize, 
						tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
					break;
				case MAP_ObjWarp:
					al_draw_bitmap_region(images[GUI_ImgTiles], 3 * tileSize, 0, tileSize, tileSize, 
						tileSize * (column % mapColumns), tileSize * (column / mapColumns), 0);
					break;
				// Dynamic images
				case MAP_ObjMonster:
					DrawHoarder(tileSize * (column % mapColumns), tileSize * (column / mapColumns));
					break;
				case MAP_ObjRupee:
					DrawRupee(tileSize * (column % mapColumns), tileSize * (column / mapColumns));
					break;
				}
			}		
		} 
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}
	 
void DrawHoarder(int posX, int posY) {
	if(initAddons) {
		UpdateAnimation(dynHoarder);
		
		al_draw_bitmap_region(dynHoarder->image, dynHoarder->curFrame * dynHoarder->frameWidth, 0, 
			dynHoarder->frameWidth, dynHoarder->frameHeight, posX, posY, 0);
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void DrawRupee(int posX, int posY) {
	if(initAddons) {
		UpdateAnimation(dynRupee);
		
		al_draw_bitmap_region(dynRupee->image, dynRupee->curFrame * dynRupee->frameWidth, 0,
			dynRupee->frameWidth, dynRupee->frameHeight, posX, posY, 0);
	} else {
		printf("ERROR: Allegro add-ons have not been initialized.\n");
		return;
	}
}

void UpdateAnimation(DynamicImage *img) {
	if(++(img->frameCount) >= img->frameDelay) {
		if(++(img->curFrame) >= img->maxFrame) {
			img->curFrame = 0; // Reset to start	
		}

		img->frameCount = 0;
	}
}