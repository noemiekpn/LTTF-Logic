#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Map.h"

typedef struct object {
	int type;
	int amount;
	int position;
} Object;

struct map {
	int width;				// Width of map
	int height;				// Height of map
	char *terrain;			// Table of terrain types
	char *visited;			// Table of visited positions
	Object *objects;		// Table of map objects
};

//------------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//------------------------------------------------------------
void DisplayMapList(Map *map);

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
	//	CREATE TERRAIN TABLE
	//------------------------------------------------------------
	map->terrain = (char*) malloc((unsigned int)(size * size));
        
	if(map->terrain == NULL)
        printf("Error while allocating matrix.\n");

	//------------------------------------------------------------
	//	CREATE VISITED TABLE
	//------------------------------------------------------------
	map->visited = (char*) malloc((unsigned int)(size * size));
        
	if(map->visited == NULL)
        printf("Error while allocating matrix.\n");
	
	//------------------------------------------------------------
	//	FILLING TABLES
	//------------------------------------------------------------
	while(byte != EOF){
		byte = fgetc(f);
		/*printf("%c", byte);*/
			
		switch(byte) {
		case 'f': // Forest
			map->terrain[(i*map->width) + j] = MAP_Forest;
			map->visited[(i*map->width) + j] = 0;
			j++;
			break;
		case 'g': // Grass
			map->terrain[(i*map->width) + j] = MAP_Grass;
			map->visited[(i*map->width) + j] = 0;
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

	/*DisplayMapList(map);*/
	fclose(f);
    return map;
}

void MAP_DestroyMap(Map *map) {
	free(map);
}

void MAP_LoadObjects(Map *map, char *fileName) {
	int numObjects = 0;		// Total number of objects to load 
		
	char type;				// Type of an object
	int amount;				// Amount of objects of this type

	//------------------------------------------------------------
	//	READ FILE: OBTAIN TOTAL NUMBER OF OBJECTS
	//------------------------------------------------------------
	FILE *f;
	f = fopen(fileName, "r");
        
	if(f == NULL){
        printf("File not found.\n", fileName);
    }

	while(fscanf(f, "%c %d\n", &type, &amount) == 2) {
		/*printf("Line: %c %d\n", type, amount);*/
		numObjects += amount;
	}

	/*printf("There is a total of %d objects in this map.\n", numObjects);*/
	rewind(f);
	

	//------------------------------------------------------------
	//	CREATE TABLE OF OBJECTS
	//------------------------------------------------------------
	map->objects = (Object*) malloc(numObjects * sizeof(Object));
        
	if(map->objects == NULL)
        printf("Error while allocating matrix.\n");
	
	int i = 0;
	while(fscanf(f, "%c %d\n", &type, &amount) == 2) {
		switch(type) {
		case 'B': // Hole
			map->objects[i].type = MAP_Hole;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'E': // Enemy
			map->objects[i].type = MAP_Monster;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'V': // Warp
			map->objects[i].type = MAP_Warp;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'F': // Fake Master Sword
			map->objects[i].type = MAP_FakeSword;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'M': // Real Master Sword
			map->objects[i].type = MAP_RealSword;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'C': // Heart
			map->objects[i].type = MAP_Heart;
			map->objects[i].amount = amount;
			i++;
			break;
		case 'R': // Rupee
			map->objects[i].type = MAP_Rupee;
			map->objects[i].amount = amount;
			i++;
			break;
		default:
			break;
		}
	}

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

void MAP_PlaceObjectsRandom(Map *map) {
	int position;
	
	srand(time(NULL));

	// Generate number between 0 and map size - 1;
	one: position = rand() % (map->width * map->height - 1);

	if(map->terrain[position] == MAP_Forest) {
		printf("Ops... Can't place it here!\n");
		goto one;
	} else {
		
	}

}

int MAP_GetMapWidth(Map *map) {
	return map->width;
}

int MAP_GetMapHeight(Map *map) {
	return map->height;
}

char *MAP_GetMapTerrains(Map *map) {
	return map->terrain;
}

char *MAP_GetMapVisited(Map *map) {
	return map->visited;
}

//------------------------------------------------------------
//	AUX FUNCTIONS
//------------------------------------------------------------
void DisplayMapList(Map *map) {

	int i, j = 0, 
		points = 0;

	printf("\n\nTERRAIN MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d", map->terrain[(i*map->height) + j]);
			points++;
		}
		printf("\n");
	}

	printf("\nVISITED MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d", map->visited[(i*map->height) + j]);
		}
		printf("\n");
	}

   	printf("\n\n# OF POINTS: %d\n", points);
}