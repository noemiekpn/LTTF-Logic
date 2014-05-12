#include "Player.h"

#define PLR_SIZE 20

enum {
	SOUTH, NORTH, EAST, WEST
} Directions;

//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
void UpdatePlayerAnimation(Player &player);

void PLR_InitializePlayer(Player &player) {
	player.energy = 100;
	player.points = 0;
	
	player.posX =  21;
	player.posY =  38;

	player.speedX = 1;
	player.speedY = 1;

	player.curFrame = 0;
	player.frameHeight = PLR_SIZE;
	player.frameWidth = PLR_SIZE;

	player.animationColumns = 7;
	player.animationDirection = 1;	// From left to right of sprite sheet
	player.animationRow = SOUTH;
}

void PLR_MoveForward(Player &player) { 
	switch(player.animationRow) {
	case SOUTH:
		player.posY += player.speedY;
		break;
	case NORTH:
		player.posY -= player.speedY;
		break;
	case EAST:
		player.posX += player.speedX;
		break;
	case WEST:
		player.posX -= player.speedX;
		break;
	}
	
	player.curFrame++;
	if(player.curFrame > player.animationColumns) {
		player.curFrame	= 0; // Reset
	}

	// Update player's points
	player.points--;
}

void PLR_TurnLeft(Player &player) {
	player.animationRow--;
	
	if(player.animationRow < SOUTH)
		player.animationRow = WEST;
	
	// Update player's points
	player.points--;
}

void PLR_TurnRight(Player &player) {
	player.animationRow++;
	
	if(player.animationRow > WEST)
		player.animationRow = SOUTH;

	// Update player's points
	player.points--;
}

void PLR_AttackEnemy(Player &player) {
	// Delete monster from map
	
	// Update player's energy and points
	player.energy -= 10;
	player.points -= 5;
}

void PLR_CollectHeart(Player &player) {
	// Delete heart from map

	// Update player's energy and points
	player.energy += 50;
	player.points -= 10;
}

void PLR_CollectRupee(Player &player) {
	// Update player's points
	player.points += 10;
}

void PLR_CollectMasterSword(Player &player) {}

void PLR_FallIntoHole(Player &player) {}

void PLR_BeAttackedByEnemy(Player &player) {}