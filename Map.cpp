#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Map.h"

typedef struct position {
	int numObjs;			// Number objects in this position
	char terrain;			// Type of terrain
	char object;			// Type of object in this position
	
	bool visited;			// If position has been visited
} Position; 

typedef struct object {
	int type;
	int amount;
} Object;

struct map {
	int width;				// Width of map
	int height;				// Height of map
	
	Position *positions;	// Table of positions 

	Object *objects;		// Vector of maps objects
	int numObjects;			// Total number of objects in this map
	int numTypes;			// Total number of types of objects in this map
};
	
//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
	void DisplayMapInfo(Map *map);

Map *MAP_LoadMap(char * fileName) {
	Map *map;
	FILE *f;
    
	int size = 0,			// Stores number of map columns
		byte = 0;			// Stores read character
	char i = 0, j = 0;		// Count columns and rows of file
		
	//------------------------------------------------------------
	//	CREATE MAP
	//------------------------------------------------------------
	map = (Map *) malloc(sizeof(Map));
	
	if(map == NULL) {
		printf("Memory access error. Failed to create map.\n");
	}
    
	//------------------------------------------------------------
	//	OBTAIN FILE # OF COLUMNS
	//------------------------------------------------------------
	f = fopen(fileName, "r");
        
	if(f == NULL){
        printf("Failed to read the file %s.\n", fileName);
    }
        
	while(byte != '\n'){
		byte = fgetc(f);
		if (byte == ' ') { }
		else{
			size++;
		}
	}

	size--;
    rewind(f);
	/*printf("SIZE = %d\n", size);*/

	// Because we will always have square maps...
	map->width = size; 
	map->height = size;

	//------------------------------------------------------------
	//	CREATE POSITION TABLE
	//------------------------------------------------------------
	map->positions = (Position*) malloc((size * size) * sizeof(Position));
        
	if(map->positions == NULL)
        printf("Error while allocating positions matrix.\n");

	//------------------------------------------------------------
	//	FILLING TABLES
	//------------------------------------------------------------
	while(byte != EOF){
		byte = fgetc(f);
		/*printf("%c", byte);*/
			
		switch(byte) {
		case 'f': // Forest
			map->positions[(i*map->width) + j].terrain = MAP_TileForest;
			map->positions[(i*map->width) + j].visited = false;
			map->positions[(i*map->width) + j].numObjs = 0;
			j++;
			break;
		case 'g': // Grass
			map->positions[(i*map->width) + j].terrain = MAP_TileGrass;
			map->positions[(i*map->width) + j].visited = false;
			map->positions[(i*map->width) + j].numObjs = 0;
			j++;
			break;
		case '\n':
			i++;
			break;
		default:
			break;
		}

		if(j == map->width){
			j = 0;
		}
    }

	/*DisplayMapInfo(map);*/
	fclose(f);
    return map;
}

void MAP_DestroyMap(Map *map) {
	free(map);
}

void MAP_LoadObjects(Map *map, char *fileName) {
	char type;				// Type of an object
	int amount;				// Amount of objects of this type

	//------------------------------------------------------------
	//	READ FILE: OBTAIN TOTAL NUMBER OF OBJECTS
	//------------------------------------------------------------
	FILE *f;
	f = fopen(fileName, "r");
        
	if(f == NULL){
        printf("ERROR: File not found.\n", fileName);
    }


	map->numObjects = 0;
	while(fscanf(f, "%c %d\n", &type, &amount) == 2) {
		/*printf("Line: %c %d\n", type, amount);*/
		map->numObjects += amount;
	}

	/*printf("There is a total of %d objects in this map.\n", map->numObjects);*/
	rewind(f);
	

	//------------------------------------------------------------
	//	CREATE VECTOR OF OBJECTS
	//------------------------------------------------------------
	map->objects = (Object*) malloc(map->numObjects * sizeof(Object));
        
	if(map->objects == NULL)
        printf("ERROR: Unable to allocate matrix.\n");
	
	int i = 0;
	while(fscanf(f, "%c %d\n", &type, &amount) == 2) {
		switch(type) {
		case 'B': // Hole
			map->objects[i].type = MAP_ObjHole;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'E': // Enemy
			map->objects[i].type = MAP_ObjMonster;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'V': // Warp
			map->objects[i].type = MAP_ObjWarp;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'F': // Fake Master Sword
			map->objects[i].type = MAP_ObjFakeSword;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'M': // Real Master Sword
			map->objects[i].type = MAP_ObjRealSword;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'C': // Heart
			map->objects[i].type = MAP_ObjHeart;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'R': // Rupee
			map->objects[i].type = MAP_ObjRupee;
			map->objects[i].amount = amount;
			i++;
			break;
		default:
			break;
		}
	}

	map->numTypes = i;
	/*printf("Total of objects loaded: %d\n", i);*/	
	
	/*for(int n = 0; n < i; n++) {
		printf("Number of ");

		switch(map->objects[n].type) {
		case MAP_Hole:
			printf("holes: ");
			break;
		case MAP_Monster:
			printf("monsters: ");
			break;
		case MAP_Warp:
			printf("warps: ");
			break;
		case MAP_FakeSword:
			printf("fake Master Swords: ");
			break;
		case MAP_RealSword:
			printf("real Master Swords: ");
			break;
		case MAP_Heart:
			printf("hearts: ");
			break;
		case MAP_Rupee:
			printf("rupees: ");
			break;
		}

		printf("%d\n", map->objects[n].amount);
	}*/
	
	fclose(f);
}

void MAP_PlaceObjectsRandom(Map *map, char *fileName) {
	FILE *objectsInfo;
	
	objectsInfo = fopen(fileName, "w");
	
	if(objectsInfo == NULL){
        printf("ERROR: Unable to create file.\n", fileName);
    }
	
	int pos;
	
	srand(time(NULL));

	for(int i = 0; i < map->numTypes; i++) {
		for(int j = 0; j < map->objects[i].amount; j++) {
			// Generate number between 0 and map size - 1;
			one: pos = rand() % (map->width * map->height - 1);

			// If this position is allowed...
			if(map->positions[pos].terrain == MAP_TileGrass) {
				
				// If position is empty, no restrictions
				if(map->positions[pos].numObjs == 0) {
					map->positions[pos].object = map->objects[i].type;
					map->positions[pos].numObjs++;
				} else {
					// Cannot have 2 objects of same type
					if(map->positions[pos].object == map->objects[i].type) {
						goto one;

					// Cannot have combination of hole, monster, warp and real Master Sword
					} else if(
				
				}
			} else {
				printf("Ops... Can't place it here!\n");
				goto one;
			}
		}
	
	}

}

int MAP_GetMapWidth(Map *map) {
	return map->width;
}

int MAP_GetMapHeight(Map *map) {
	return map->height;
}

char MAP_GetPositionTerrain(Map *map, int pos) {
	return map->positions[pos].terrain;
}

bool MAP_GetPositionVisitStatus(Map *map, int pos) {
	return map->positions[pos].visited;
}

void MAP_SetPositionVisitStatus(Map *map, int pos, bool status) {
	map->positions[pos].visited = status;
}

//------------------------------------------------------------
//	AUX FUNCTIONS
//------------------------------------------------------------
void DisplayMapInfo(Map *map) {

	int i, j = 0, 
		points = 0;

	printf("\n\nTERRAIN MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d", map->positions[(i*map->height) + j].terrain);
			points++;
		}
		printf("\n");
	}

	printf("\nVISITED MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d", map->positions[(i*map->height) + j].visited);
		}
		printf("\n");
	}

   	printf("\n\n# OF POINTS: %d\n", points);
}
