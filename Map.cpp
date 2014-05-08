#include <stdio.h>
#include <stdlib.h>

#include "Map.h"

struct map {
	int width;					// Width of map
	int height;					// Height of map
	char *terrain;				// Table of terrain types
	char *visited;				// Table of visited positions
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
			map->terrain[(i*map->width) + j] = FOREST;
			map->visited[(i*map->width) + j] = 0;
			j++;
			break;
		case 'g': // Grass
			map->terrain[(i*map->width) + j] = GRASS;
			map->visited[(i*map->width) + j] = 0;
			j++;
			break;
		case ' ':
			break;
		case '\n':
			i++;
			break;
		/*default:
			printf("Invalid character in map file: %c\n", byte);
			break;*/
		}

		if(j == map->width){
			j = 0;
		}
    }

	DisplayMapList(map);
	fclose(f);
    return map;
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

//------------------------------------------------------------
//	AUX FUNCTIONS
//------------------------------------------------------------
void DisplayMapList(Map *map) {

	int i, j = 0, 
		points = 0;

	printf("\n\nTERRAIN MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d ", map->terrain[(i*map->height) + j]);
			points++;
		}
		printf("\n");
	}

	printf("\n\VISITED MAP\n\n");
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			printf("%d ", map->visited[(i*map->height) + j]);
		}
		printf("\n");
	}

   	printf("\n\n# OF POINTS: %d\n", points);
}