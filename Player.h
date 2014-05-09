#ifndef PLAYER_H
	#define	PLAYER_H 

typedef struct player {
	int energy;
	int points;

	int posX,				// Column, i.e. "j"
		posY;				// Line, i.e "i"
	
	int direction;			// Direction player is facing
	int speedX, speedY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth, frameHeight;
	
	int animationColumns;
	int animationDirection;
	int animationRow;

} Player;

//------------------------------------------------------------
//	Initializes all player struct variables.
//------------------------------------------------------------
	void PLR_InitializePlayer(Player &player);

//------------------------------------------------------------
//	Increments player's position numSpaces according to his
//	direction, animating movement. Also decrements player's 
//	points by 1.
//	Facing north:	-y
//	Facing south:	+y
//	Facing west:	-x
//	Facing east:	+x
//
//	numSpaces = number of spaces to move
//------------------------------------------------------------
	void PLR_MoveForward(Player &player, int numSpaces);
	
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
//	Animates player sword attack. Also decrements player's 
//	points by 5.
//------------------------------------------------------------	
	void PLR_AttackEnemy(Player &player);

//------------------------------------------------------------
//	Decrements player's points by 10 and increment's player's
//	energy by 50.
//------------------------------------------------------------	
	void PLR_CollectHeart(Player &player);

//------------------------------------------------------------
//	Increments player's points by 10 and increment's player's
//	energy by 50.
//------------------------------------------------------------	
	void PLR_CollectRupee(Player &player);

//------------------------------------------------------------
//	Decrements player's points by 100.
//------------------------------------------------------------	
	void PLR_CollectMasterSword(Player &player);

//------------------------------------------------------------
//	Decrements player's points by 10000.
//------------------------------------------------------------	
	void PLR_FallIntoHole(Player &player);

//------------------------------------------------------------
//	Decrements player's points by 10000.
//------------------------------------------------------------	
	void PLR_BeAttackedByEnemy(Player &player);
#endif	/* PLAYER_H */