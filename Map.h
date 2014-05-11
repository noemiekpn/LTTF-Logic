#ifndef MAP_H
	#define	MAP_H 

	typedef struct map Map;
	typedef struct object Object;

	typedef enum {
		MAP_TileForest, MAP_TileGrass
	} MAP_Tiles;

	typedef enum {
		MAP_ObjHole, MAP_ObjMonster, MAP_ObjFakeSword, MAP_ObjRealSword, 
		MAP_ObjWarp, MAP_ObjHeart, MAP_ObjRupee
	} MAP_Objs;


//------------------------------------------------------------
//	Reads map file and creates a map, setting its positions
//	to unvisited, with zero objects, as well as its terrains.
//
//	fileName = file location
//------------------------------------------------------------
	Map *MAP_LoadMap(char *fileName);

//------------------------------------------------------------
//	Destroys (frees) designated map.
//------------------------------------------------------------
	void MAP_DestroyMap(Map *map);

//------------------------------------------------------------
//	Reads objects file, attributing types and amounts.
//
//	fileName = file location
//------------------------------------------------------------
	void MAP_LoadObjects(Map *map, char *fileName);

//------------------------------------------------------------
//	Places objects randomly on map positions according to
//	these rules:
//	1. Objects can only be placed on grass;
//	2. One position cannot have two objects of same type;
//	3. One position cannot have combination of hole, monster,
//	warp and real Master Sword.
//------------------------------------------------------------
	void MAP_PlaceObjectsRandom(Map *map);

//------------------------------------------------------------
//	Returns width of map (number of columns).
//------------------------------------------------------------
	int MAP_GetMapWidth(Map *map);

//------------------------------------------------------------
//	Returns height of map (number of rows).
//------------------------------------------------------------
	int MAP_GetMapHeight(Map *map);

//------------------------------------------------------------
//	Returns terrain type of a map position.
//------------------------------------------------------------
	char MAP_GetPositionTerrain(Map *map, int pos);

//------------------------------------------------------------
//	Returns visit status of map position.
//------------------------------------------------------------
	bool MAP_GetPositionVisitStatus(Map *map, int pos);

//------------------------------------------------------------
//	Sets visit status of map position.
//------------------------------------------------------------
	void MAP_SetPositionVisitStatus(Map *map, int pos, bool status);

//------------------------------------------------------------
//	Returns number of objects in a position.
//------------------------------------------------------------
	int MAP_GetPositionNumObjects(Map *map, int pos);

//------------------------------------------------------------
//	Returns vector of objects in a position.
//------------------------------------------------------------
	Object *MAP_GetPositionObjects(Map *map, int pos);

//------------------------------------------------------------
//	Returns the type of a map object.
//------------------------------------------------------------
	int MAP_GetObjectType(Object obj);

#endif	/* MAP_H */

