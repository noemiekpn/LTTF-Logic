#ifndef MAPSET_H
	#define	MAPSET_H 

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
//	Reads objects coordinates file and places these objects
//	in respective positions on map. Assumes correct file.
//
//	fileName = file location
//------------------------------------------------------------
	void MAP_PlaceObjects(Map *map, char *fileName);

//------------------------------------------------------------
//	Must be preceeded by LoadObjects function. 	
//	Places objects randomly on map positions according to
//	these rules:
//	1. Objects can only be placed on grass;
//	2. One position cannot have two objects of same type;
//	3. One position cannot have combination of hole, monster,
//	warp and real Master Sword.
//------------------------------------------------------------
	void MAP_PlaceObjectsRandom(Map *map);

#endif	/* MAPSET_H */

