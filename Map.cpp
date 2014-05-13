#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MapPublicInfo.h"
#include "MapPrivateInfo.h"
#include "MapSettings.h"

#define MAX_OBJS 4

typedef struct object {
	int type;
	int amount;
} Object;

typedef struct position {
	int numObjs;			// Number objects in this position
	char terrain;			// Type of terrain
	int objs[MAX_OBJS];		// Array of object types in this position
	bool visited;			// If position has been visited
	bool safe;				// If position is safe
} Position; 

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
	bool AllowObjectsCombination(int object1, int object2);
	int ConvertType(char type);
	int SearchMapObject(Map *map, int object);

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
			map->positions[(i*map->width) + j].safe = false;
			map->positions[(i*map->width) + j].numObjs = 0;
			j++;
			break;
		case 'g': // Grass
			map->positions[(i*map->width) + j].terrain = MAP_TileGrass;
			map->positions[(i*map->width) + j].visited = false;
			map->positions[(i*map->width) + j].safe = false;
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
        printf("ERROR: File not found.\n");
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

void MAP_PlaceObjects(Map *map, char *fileName) {
	int x, y;		// Object's coordinates from file 
	char type;		// Object's type from tile
	int total = 0;	// Total of objects placed

	FILE *f;
	f = fopen(fileName, "r");
        
	if(f == NULL){
        printf("ERROR: File not found.\n");
    }

	int mapColumns = map->width;

	while(fscanf(f, "%d %d %c\n", &y, &x, &type) == 3) {
		/*printf("Place object type %c on (%d, %d)\n", type, x, y);*/
		
		// If this position is grass...
			if(map->positions[y * mapColumns + x].terrain == MAP_TileGrass) {	
			// If position is empty, no restrictions
			if(map->positions[y * mapColumns + x].numObjs == 0) {
				map->positions[y * mapColumns + x].objs[0] = ConvertType(type);
				map->positions[y * mapColumns + x].numObjs++;
				total++;
			}

			// If position is occupied...
			else {
				// ... check if the combination of objects is possible 
				bool allow = true;
				for(int k = 0; k < map->positions[y * mapColumns + x].numObjs; k++) {
					if(!AllowObjectsCombination(map->positions[y * mapColumns + x].objs[k], ConvertType(type))) {
						allow = false;
					}
				}
				if(allow) {
					map->positions[y * mapColumns + x].objs[map->positions[y * mapColumns + x].numObjs + 1] = ConvertType(type);
					map->positions[y * mapColumns + x].numObjs++;
					total++;
				}
			}
		}
	}

	printf("Total of objects placed: %d\n", total);

	fclose(f);
}

void MAP_PlaceObjectsRandom(Map *map) {
	int pos;
	int total = 0;
	
	srand(time(NULL));

	for(int i = 0; i < map->numTypes; i++) {
		for(int j = 0; j < map->objects[i].amount;) {
			/*printf("Placing %d/%d objects of type %d...\n", j + 1, map->objects[i].amount, map->objects[i].type);*/

			// Generate number between 0 and map size - 1;
			pos = rand() % (map->width * map->height - 1);

			// If this position is grass...
			if(map->positions[pos].terrain == MAP_TileGrass) {
				
				// If position is empty, no restrictions
				if(map->positions[pos].numObjs == 0) {
					map->positions[pos].objs[0] = map->objects[i].type;
					map->positions[pos].numObjs++;
					/*printf("Position %d now has %d objects of type %d.\n", pos, map->positions[pos].numObjs, map->positions[pos].objs[0]);*/
					j++;
					total++;

				}
				
				// If position is occupied...
				else if(map->positions[pos].numObjs > 0 && map->positions[pos].numObjs < 4) {
					// ... check if the combination of objects is possible 
					bool allow = true;
					for(int k = 0; k < map->positions[pos].numObjs; k++) {
						if(!AllowObjectsCombination(map->positions[pos].objs[k], map->objects[i].type)) {
							allow = false;
						}
					}
					if(allow) {
						map->positions[pos].objs[map->positions[pos].numObjs + 1] = map->objects[i].type;
						map->positions[pos].numObjs++;
						/*printf("Position %d now has %d objects: \n", pos, map->positions[pos].numObjs);
						for(int k = 0; k < map->positions[pos].numObjs; k++) {
							printf("Object %d: %d\n", k, map->positions[pos].objs[k]);
						}*/	
						j++;
						total++;
					}
				}					
				// If position is full...
				else {
					/*printf("Ops... These objects can't be together!\n");*/
				}
			} /* End of grass if */
			
			// If position is forest...
			else {
				/*printf("Ops... Can't place it here!\n");*/
			}
		} /* End of second for */
	} /* End of first for */

	printf("Total of objects placed: %d\n", total);
} /* End of function */

int MAP_GetMapWidth(Map *map) {
	return map->width;
}

int MAP_GetMapHeight(Map *map) {
	return map->height;
}

int MAP_GetMapObjectAmount(Map *map, int object) {
	int objIndex = SearchMapObject(map, object);

	if(objIndex < 0) {
		printf("Object not found.\n");
		return -1;
	} else {
		return map->objects[objIndex].amount;
	}
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

bool MAP_GetPositionSafetyStatus(Map *map, int pos) {
	return map->positions[pos].safe;
}

int MAP_GetPositionNumObjects(Map *map, int pos) {
	return map->positions[pos].numObjs;
}

int *MAP_GetPositionObjects(Map *map, int pos) {
	return map->positions[pos].objs;
}

int MAP_SearchPositionObject(Map *map, int pos, int object) {
	int index = -1;
	
	for(int i = 0; i < map->positions[pos].numObjs; i++) {
		if(map->positions[pos].objs[i] == object)
			index = i;
	}

	return index;
}

bool MAP_DeletePositionObject(Map *map, int pos, int object) {
	int objIndex = MAP_SearchPositionObject(map, pos, object);
	
	if(objIndex < 0) {
		printf("Object not found. Deletion not accomplished.\n");
		return false;
	} else {
		map->positions[pos].objs[objIndex] = -1;
		return true;
	}
}

void MAP_GenerateRandomWarpDestiny(Map *map, int *posX, int *posY) {
	int mapLines = MAP_GetMapWidth(map),
		mapColumns = MAP_GetMapHeight(map);

	srand(time(NULL));
	
	// Generate random position within map
	*posX = rand() % (mapLines - 1);
	*posY = rand() % (mapColumns - 1);

	// While position is not grass, keep trying
	while(MAP_GetPositionTerrain(map, (*posY) * mapLines + (*posX)) != MAP_TileGrass) {
		*posX = rand() % (mapLines - 1);
		*posY = rand() % (mapColumns - 1);
	}
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

/* Checks if two objects can be in the same position */
bool AllowObjectsCombination(int object1, int object2) {
	switch(object1) {
		case MAP_ObjHole:
		case MAP_ObjMonster:
		case MAP_ObjWarp:
		case MAP_ObjRealSword:
			switch(object2) {
				case MAP_ObjHole:
				case MAP_ObjMonster:
				case MAP_ObjWarp:
				case MAP_ObjRealSword:
					return false;
				case MAP_ObjFakeSword:
				case MAP_ObjHeart:
				case MAP_ObjRupee:
					return true;
				default:
					printf("Could not recognize type of object.\n");
					return false;
			}
		case MAP_ObjFakeSword:
			switch(object2) {
				case MAP_ObjHole:
				case MAP_ObjMonster:
				case MAP_ObjWarp:
				case MAP_ObjRealSword:
				case MAP_ObjHeart:
				case MAP_ObjRupee:
					return true;
				case MAP_ObjFakeSword:
					return false;
				default:
					printf("Could not recognize type of object.\n");
					return false;
			}
		case MAP_ObjHeart:
			switch(object2) {
				case MAP_ObjHole:
				case MAP_ObjMonster:
				case MAP_ObjWarp:
				case MAP_ObjRealSword:
				case MAP_ObjFakeSword:
				case MAP_ObjRupee:
					return true;
				case MAP_ObjHeart:
					return false;
				default:
					printf("Could not recognize type of object.\n");
					return false;
			}
		case MAP_ObjRupee:
			switch(object2) {
				case MAP_ObjHole:
				case MAP_ObjMonster:
				case MAP_ObjWarp:
				case MAP_ObjRealSword:
				case MAP_ObjFakeSword:
				case MAP_ObjHeart:
					return true;
				case MAP_ObjRupee:
					return false;
				default:
					printf("Could not recognize type of object.\n");
					return false;
			}				
		default:
			printf("Could not recognize type of object.\n");
			return false;
	}
}

/* Converts types from letters to enum types */
int ConvertType(char type) {
	switch(type) {
	case 'B':
		return MAP_ObjHole;
	case 'E':
		return MAP_ObjMonster;
	case 'F':
		return MAP_ObjFakeSword;
	case 'M':
		return MAP_ObjRealSword;
	case 'V':
		return MAP_ObjWarp;
	case 'C':
		return MAP_ObjHeart;
	case 'R':
		return MAP_ObjRupee;
	}
};

/* Returns the index of the object in the map objects array */
int SearchMapObject(Map *map, int object) {
	int index = -1;
	
	for(int i = 0; i < map->numTypes; i++) {
		if(map->objects[i].type == object)
			index = i;
	}

	return index;
}