#ifndef MAP_H
	#define	MAP_H 

	typedef struct map Map;

	enum TILES {FOREST, GRASS};

//------------------------------------------------------------
//	Reads map file, identifies important points and sets 
//	terrain weights
//
//	fileName = file location
//------------------------------------------------------------
	Map *MAP_LoadMap(char *fileName);

//------------------------------------------------------------
//	Returns width of map (number of columns)
//------------------------------------------------------------
	int MAP_GetMapWidth(Map *map);

//------------------------------------------------------------
//	Returns height of map (number of rows)
//------------------------------------------------------------
	int MAP_GetMapHeight(Map *map);

//------------------------------------------------------------
//	Returns map table of terrains
//------------------------------------------------------------
	char *MAP_GetMapTerrains(Map *map);

//------------------------------------------------------------
//	Returns map table of visited points
//	0 - unvisited
//	1 - visited
//------------------------------------------------------------
	char *MAP_GetMapVisited(Map *map);

#endif	/* MAP_H */

