#ifndef MAPPRIVATE_H
	#define	MAPPRIVATE_H

//------------------------------------------------------------
//	Returns amount of a certain object (from MAP_Objs) in map.
//	If object is not found, returns -1.
//------------------------------------------------------------
	int MAP_GetMapObjectAmount(Map *map, int object);

//------------------------------------------------------------
//	Returns terrain type of a map position from MAP_Tiles.
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
//	Returns array of objects in a position, from MAP_Objs.
//------------------------------------------------------------
	int *MAP_GetPositionObjects(Map *map, int pos);

#endif	/* MAPPRIVATE_H */