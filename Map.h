#ifndef MAP_H
	#define	MAP_H 

	typedef struct map Map;

	typedef enum {
		MAP_Forest, MAP_Grass
	} MAP_Tiles;

	typedef enum {
		MAP_Hole, MAP_Monster, MAP_FakeSword, MAP_RealSword, MAP_Warp, MAP_Heart, MAP_Rupee
	} MAP_Objects;


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
//	Returns map table of terrains.
//------------------------------------------------------------
	char *MAP_GetMapTerrains(Map *map);

//------------------------------------------------------------
//	Returns map table of visited points.
//	0 - unvisited
//	1 - visited
//------------------------------------------------------------
	char *MAP_GetMapVisited(Map *map);

#endif	/* MAP_H */

