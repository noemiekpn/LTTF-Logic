#ifndef MAPPUBLIC_H
	#define	MAPPUBLIC_H

	typedef struct map Map;

	typedef enum {
		MAP_TileForest, MAP_TileGrass
	} MAP_Tiles;

	typedef enum {
		MAP_ObjHole, MAP_ObjMonster, MAP_ObjFakeSword, MAP_ObjRealSword, 
		MAP_ObjWarp, MAP_ObjHeart, MAP_ObjRupee
	} MAP_Objs;

//------------------------------------------------------------
//	Returns width of map (number of columns).
//------------------------------------------------------------
	int MAP_GetMapWidth(Map *map);

//------------------------------------------------------------
//	Returns height of map (number of rows).
//------------------------------------------------------------
	int MAP_GetMapHeight(Map *map);

//------------------------------------------------------------
//	Deletes an object (from MAP_Objs) from position array.
//	If object is not found, no deletion is made.
//------------------------------------------------------------
	void MAP_DeletePositionObject(Map *map, int pos, int object);

//------------------------------------------------------------
//	Generates a random new position the warp object will send
//	to. New position can be in any grass terrain of map.
//------------------------------------------------------------
	void MAP_GenerateRandomWarpDestiny(Map *map, int *posX, int *posY); 

#endif	/* MAPPUBLIC_H */