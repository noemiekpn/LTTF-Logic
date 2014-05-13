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
	player.realSword = false;

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

void PLR_MoveTo(Map *map, Player &player, int posX, int posY) {
	Point pi, pf;
	pi.x = player.posX;
	pi.y = player.posY;
	pf.x = posX;
	pf.y = posY;

	int *path = GPF_FindShortestPath(pf, pi, map);
	int pathLength = path[0];

	for(int pos = 2; pos < pathLength ; pos += 2) {
		int dirX = player.posX - path[pos];
		int dirY = player.posY - path[pos + 1];

		if(dirX > 0) {
			// Player is moving west
			switch(player.animationRow) {
			case SOUTH:
				PLR_TurnRight(player);
				break;
			case NORTH:
				PLR_TurnLeft(player);
				break;
			case EAST:
				PLR_TurnRight(player);
				PLR_TurnRight(player);
				break;
			case WEST:
				break;
			}
		} else if (dirX < 0) {
			// Player is moving east
			switch(player.animationRow) {
			case SOUTH:
				PLR_TurnLeft(player);
				break;
			case NORTH:
				PLR_TurnRight(player);
				break;
			case EAST:
				break;
			case WEST:
				PLR_TurnLeft(player);
				PLR_TurnLeft(player);
				break;
			}
		} else {
			// Player is not moving horizontally...
			if(dirY > 0) {
				// Player is moving north
				switch(player.animationRow) {
				case SOUTH:
					PLR_TurnRight(player);
					PLR_TurnRight(player);
					break;
				case NORTH:
					break;
				case EAST:
					PLR_TurnLeft(player);
					break;
				case WEST:
					PLR_TurnRight(player);
					break;
				}
			} else if (dirY < 0) {
				// Player is moving south
				switch(player.animationRow) {
				case SOUTH:
					break;
				case NORTH:
					PLR_TurnRight(player);
					PLR_TurnRight(player);
					break;
				case EAST:
					PLR_TurnRight(player);
					break;
				case WEST:
					PLR_TurnLeft(player);
					break;
				}
			} else {
				// Player is not moving vertically...
			}
		} /* End of direction if-else */

		PLR_MoveForward(map, player);
	} /* End of for */
}

bool PLR_AttackEnemy(Map *map, Player &player, int posX, int posY) {
	bool killed;
	
	int dirX = player.posX - posX;
	int dirY = player.posY - posY;
		
	if(dirX > 0) {
		// Monster is at left of player
		switch(player.animationRow) {
		case SOUTH:
			PLR_TurnRight(player);
			break;
		case NORTH:
			PLR_TurnLeft(player);
			break;
		case EAST:
			PLR_TurnRight(player);
			PLR_TurnRight(player);
			break;
		case WEST:
			break;
		}

	} else if (dirX < 0) {
		// Monster is at right of player
		switch(player.animationRow) {
		case SOUTH:
			PLR_TurnLeft(player);
			break;
		case NORTH:
			PLR_TurnRight(player);
			break;
		case EAST:
			break;
		case WEST:
			PLR_TurnLeft(player);
			PLR_TurnLeft(player);
			break;
		}
	} else {
		// Monster is at same x of player (above or below...)
		if(dirY > 0) {
			// Monster is above player
			switch(player.animationRow) {
			case SOUTH:
				PLR_TurnRight(player);
				PLR_TurnRight(player);
				break;
			case NORTH:
				break;
			case EAST:
				PLR_TurnLeft(player);
				break;
			case WEST:
				PLR_TurnRight(player);
				break;
			}
		} else if (dirY < 0) {
			// Monster is below player
			switch(player.animationRow) {
			case SOUTH:
				break;
			case NORTH:
				PLR_TurnRight(player);
				PLR_TurnRight(player);
				break;
			case EAST:
				PLR_TurnRight(player);
				break;
			case WEST:
				PLR_TurnLeft(player);
				break;
			}
		} else {
			// Monster is at same position of player
			PLR_BeAttackedByEnemy(player);
		}
	}

	killed = MAP_DeletePositionObject(map, posY * MAP_GetMapWidth(map) + posX, MAP_ObjMonster);
	
	// Update player's energy and points
	player.energy -= 10;
	player.points -= 5;

	return killed;
}

void PLR_CollectHeart(Map *map, Player &player) {
	// Delete heart from map
	if(MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjHeart)) {
		// Update player's energy and points
		if(player.energy + 50 < 100)
			player.energy += 50;
		else
			player.energy = 100;
		player.points -= 10;

		// Update player's heart account
		player.heartsCollected++;
	}
}

void PLR_CollectRupee(Map *map, Player &player) {
	// Delete rupee from map
	if(MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjRupee)) {
		// Update player's points
		player.points += 10;

		// Update player's rupee account
		player.rupeesCollected++;
	}
}

void PLR_CollectMasterSword(Map *map, Player &player) {
	bool realSword = MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjRealSword);
	bool fakeSword = MAP_DeletePositionObject(map, player.posY * MAP_GetMapWidth(map) + player.posX, MAP_ObjFakeSword);
	
	// Delete sword from map (whichever it is...)
	if(fakeSword || realSword) {
		// Update player's points
		player.points -= 100;

		// Update player's rupee account
		player.swordsCollected++;

		if(realSword)
			player.realSword = true;
	}
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
