#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Player.h"
#include "GUI.h"

#define PLR_SIZE 20

//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
void UpdatePlayerAnimation(Player &player);

void PLR_InitializePlayer(Player &player, ALLEGRO_BITMAP *image) {
	player.energy = 100;
	player.points = 0;
	
	player.posX =  21;
	player.posY =  38;

	player.speedX = 0;
	player.speedY = 0;

	player.maxFrame = 9;
	player.curFrame = 0;
	player.frameCount = 0;
	player.frameDelay = 10;
	player.frameHeight = PLR_SIZE;
	player.frameWidth = PLR_SIZE;

	player.animationColumns = 7;
	player.animationDirection = 7;
	player.animationRow = 0;

	player.image = image;
}

void PLR_DrawPlayer(Player &player) {
	int frameX = (player.curFrame % player.animationColumns) * player.frameWidth;
	int frameY = player.animationRow * player.frameHeight;

	al_draw_bitmap_region(player.image, frameX, frameY, player.frameWidth, 
		player.frameHeight, player.posX * GUI_GetTileSize(), player.posY * GUI_GetTileSize(), 0);
}

void PLR_MoveForward(Player &player) { 
	player.animationRow = 3;
	player.posY -= player.speedY;

	UpdatePlayerAnimation(player);
}

void PLR_TurnLeft(Player &player) {
	player.animationRow = 0;
	player.posY += player.speedY;
	UpdatePlayerAnimation(player);
}

void PLR_TurnRight(Player &player) {
	player.animationRow = 1;
	player.posX -= player.speedX;
	UpdatePlayerAnimation(player);
}

//------------------------------------------------------------
//	AUX FUNCTIONS
//------------------------------------------------------------
void UpdatePlayerAnimation(Player &player) {
	if(++player.frameCount >= player.frameDelay - 1) {
		player.curFrame += player.animationDirection;
				
		if(player.curFrame >= player.maxFrame) {
			player.curFrame = 0; // Reset to start	
		}
		else if(player.curFrame <= 0) {
			player.curFrame = player.maxFrame - 1;
		}

		player.frameCount = 0;
	}
}