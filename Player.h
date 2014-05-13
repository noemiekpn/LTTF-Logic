#ifndef PLAYER_H
	#define	PLAYER_H 

#include "MapPublicInfo.h"
#include "GraphPathFinder.h"

typedef struct player {
	int energy;
	int points;

	bool alive;
	bool realSword;

	int enemiesKilled;
	int heartsCollected;
	int rupeesCollected;
	int swordsCollected;

	int posX,					// Column, i.e. "j"
		posY;					// Line, i.e "i"
	int speedX, speedY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth, frameHeight;
	
	int animationColumns;
	int animationDirection;
	int animationRow;			// Direction player is facing

} Player;

//------------------------------------------------------------
//	Initializes all player struct variables.
//------------------------------------------------------------
	void PLR_InitializePlayer(Player &player);

//------------------------------------------------------------
//	Increments player's position according to his speed and
//	direction, animating movement. Also decrements player's 
//	points by 1.
//	Facing north:	-y
//	Facing south:	+y
//	Facing west:	-x
//	Facing east:	+x
//
//	numSpaces = number of spaces to move
//------------------------------------------------------------
	void PLR_MoveForward(Map *map, Player &player);
	
//------------------------------------------------------------
//	Changes player direction by "rotating" 90 degrees left.
//	direction. Also decrements player's points by 1.
//------------------------------------------------------------	
	void PLR_TurnLeft(Player &player);

//------------------------------------------------------------
//	Changes player direction by "rotating" 90 degrees right.
//	direction. Also decrements player's points by 1.
//------------------------------------------------------------	
	void PLR_TurnRight(Player &player);

//------------------------------------------------------------
//	Moves player from his position to defined position using
//	A*. Cost is calculated within.
//------------------------------------------------------------	
	void PLR_MoveTo(Map *map, Player &player, int posX, int posY);

//------------------------------------------------------------
//	Decrements player's points by 5 and decrements player's
//	energy by 10. Monster is always killed. Assumes adjacent
//	monster position.
//
//	posX = Coordinate x of position to attack
//	posY = Coordinate y of position to attack
//------------------------------------------------------------	
	bool PLR_AttackEnemy(Map *map, Player &player, int posX, int posY);

//------------------------------------------------------------
//	Decrements player's points by 10 and increment's player's
//	energy by 50.
//------------------------------------------------------------	
	void PLR_CollectHeart(Map *map, Player &player);

//------------------------------------------------------------
//	Increments player's points by 10.
//------------------------------------------------------------	
	void PLR_CollectRupee(Map *map, Player &player);

//------------------------------------------------------------
//	Decrements player's points by 100.
//------------------------------------------------------------	
	void PLR_CollectMasterSword(Map *map, Player &player);

//------------------------------------------------------------
//	Decrements player's points by 10000.
//------------------------------------------------------------	
	void PLR_FallIntoHole(Player &player);

//------------------------------------------------------------
//	Teletransports player to random position in map.
//------------------------------------------------------------	
	void PLR_FallIntoWarp(Map *map, Player &player);

//------------------------------------------------------------
//	Decrements player's points by 10000.
//------------------------------------------------------------	
	void PLR_BeAttackedByEnemy(Player &player);

#endif	/* PLAYER_H */