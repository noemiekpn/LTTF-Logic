#ifndef MAP_H
	#define	MAP_H 

	typedef struct map Map;

	typedef enum {
		MAP_TileForest, MAP_TileGrass
	} MAP_Tiles;

	typedef enum {
		MAP_ObjHole, MAP_ObjMonster, MAP_ObjFakeSword, MAP_ObjRealSword, 
		MAP_ObjWarp, MAP_ObjHeart, MAP_ObjRupee
	} MAP_Objs;


//------------------------------------------------------------
//	Reads map file, identifies important points and sets 
//	terrain weights.
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

#endif	/* MAP_H */

