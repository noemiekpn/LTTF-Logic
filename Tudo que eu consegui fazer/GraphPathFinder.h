#ifndef GRAPHPATHFINDER_H
	#define	GRAPHPATHFINDER_H    

	#include "MapPublicInfo.h"

	typedef struct point {
		int x;
		int y;
	} Point;

//------------------------------------------------------------
//	Finds shortest (least energy consuming) path from a
//	start to ending point, given a map.
//	In array, position 0 is the length of the array and
//	position 1 is the total cost of this path (moving forward).
//------------------------------------------------------------
	int *GPF_FindShortestPath(Point start, Point end, Map *map);

#endif	/* GRAPHPATHFINDER_H */