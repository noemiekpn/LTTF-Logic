#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Player.h"

#define PLR_SIZE 20

enum {
	SOUTH, WEST, NORTH, EAST
} Directions;

void PLR_InitializePlayer(Player &player) {
	player.energy = 100;
	player.points = 0;
	
	player.alive = true;

	player.enemiesKilled = 0;
	player.heartsCollected = 0;
	player.rupeesCollected = 0;
	player.swordsCollected = 0;

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

void PLR_MoveForward(Map *map, Player &player) { 
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
		player.animationRow = EAST;
	
	// Update player's points
	player.points--;
}

void PLR_TurnRight(Player &player) {
	player.animationRow++;
	
	if(player.animationRow > EAST)
		player.animationRow = SOUTH;

	// Update player's points
	player.points--;
}

void PLR_AttackEnemy(Map *map, Player &player) {
	// Delete monster from map
	/*MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjMonster);*/

	// Update player's energy and points
	player.energy -= 10;
	player.points -= 5;
}

void PLR_CollectHeart(Map *map, Player &player) {
	// Delete heart from map
	MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjHeart);

	// Update player's energy and points
	if(player.energy + 50 < 100)
		player.energy += 50;
	else
		player.energy = 100;
	player.points -= 10;

	// Update player's heart account
	player.heartsCollected++;
}

void PLR_CollectRupee(Map *map, Player &player) {
	// Delete rupee from map
	MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjRupee);
	
	// Update player's points
	player.points += 10;

	// Update player's rupee account
	player.rupeesCollected++;
}

void PLR_CollectMasterSword(Map *map, Player &player) {
	// Delete sword from map (whichever it is...)
	MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjFakeSword);
	MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjRealSword);
	
	// Update player's points
	player.points -= 100;

	// Update player's rupee account
	player.swordsCollected++;
}

void PLR_FallIntoHole(Player &player) {
	// Update player's points
	player.points -= 10000;

	// Player dies...
	player.alive = false;
}

void PLR_FallIntoWarp(Map *map, Player &player) {
	MAP_GenerateRandomWarpDestiny(map, &player.posX, &player.posY); 
}

void PLR_BeAttackedByEnemy(Player &player) {
	// Update player's points
	player.points -= 10000;

	// Player dies...
	player.alive = false;
}
