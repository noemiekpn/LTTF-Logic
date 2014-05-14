#ifndef SENSOR_H
	#define	SENSOR_H 

#include <SWI-cpp.h>

#include "MapPublicInfo.h"
#include "MapPrivateInfo.h"

//------------------------------------------------------------
//	Detects distortion in current position. Returns true if
//  there is a warp in adjacent position.
//------------------------------------------------------------
	bool SNS_DetectDistortion(Map* map,int x, int y);

//------------------------------------------------------------
//	Detects breeze in current position. Returns true if
//  there is a hole in adjacent position.
//------------------------------------------------------------
	bool SNS_DetectBreeze(Map* map,int x, int y);

//------------------------------------------------------------
//	Detects noise in current position. Returns true if
//  there is a monster in adjacent position.
//------------------------------------------------------------
	bool SNS_DetectNoise(Map* map,int x, int y);

//------------------------------------------------------------
//	Detects if Pendants of Virtue are shining on current
//	position. Returns true if there is a sword on current 
//	position.
//------------------------------------------------------------
	bool SNS_DetectPendantsVirtue(Map* map,int x, int y);

//------------------------------------------------------------
//	Detects brightness in current position. Returns true if
//  there is a rupee on current position.
//------------------------------------------------------------
	bool SNS_DetectBrightness(Map* map,int x, int y);

//------------------------------------------------------------
//	Detects fairies in current position. Returns true if
//  there is a heart on current position.
//------------------------------------------------------------
	bool SNS_DetectFairy(Map* map,int x, int y);

#endif	/* SENSOR_H */