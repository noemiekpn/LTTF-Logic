#include "Sensor.h"
//------------------------------------------------------------
//	GLOBAL VARIABLES
//------------------------------------------------------------

bool SNS_DetectDistortion(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjWarp)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjWarp)!=-1)
		return true;
	else
		return false;
}

bool SNS_DetectBreeze(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjHole)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjHole)!=-1)
		return true;
	else
		return false;
}

bool SNS_DetectNoise(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y+1) +x,MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*(y-1)+x,MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x+1),MAP_ObjMonster)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +(x-1),MAP_ObjMonster)!=-1)
		return true;
	else
		return false;
}

bool SNS_DetectPendantsVirtue(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjRealSword)!=-1)
		return true;
	else if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjFakeSword)!=-1)
		return true;
	else
		return false;
}

bool SNS_DetectBrightness(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y+x,MAP_ObjRupee)!=-1)
		return true;
	else
		return false;
}

bool SNS_DetectFairy(Map* map, int x, int y){
	if(MAP_SearchPositionObject(map,MAP_GetMapWidth(map)*y +x,MAP_ObjHeart)!=-1)
		return true;
	else
		return false;
}