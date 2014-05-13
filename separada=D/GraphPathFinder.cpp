#include <stdio.h>
#include <stdlib.h>

#include "GraphPathFinder.h"
#include "MapPrivateInfo.h"

#define MAX 2000

typedef struct node{
	int x, y;			// Indexes in map matrix	
	int sumCost;		// Total movement cost summed until current node
	int modulus;		// Total estimated cost from current node to end 
	int function;		// Modulus + sumCost (heuristic function)
	int step;			// How many steps until current node
	struct node *previous;
 } Node;

typedef struct mark{
	bool marked;
	bool closed;
	Node *where;
} Mark;

//-----------------------------------------------------------
//	GLOBAL VARIABLE
//-----------------------------------------------------------
	Node memNode[MAX];	// Memory source

//-----------------------------------------------------------
//	AUX FUNCTIONS PROTOTYPES
//-----------------------------------------------------------
	void InitializeMarks(Mark *mark, int column);
	int ManhattanModulus(int x, int y, int z, int w);

int *GPF_FindShortestPath(Point start, Point end, Map *map){ 
	int line = MAP_GetMapHeight(map);
	
	bool arrived = false;			// Escape from main loop
	int memoryLimit = MAX;

	int smallest = 0,			// Smallest accumulated cost
		inferiorLimit = 0,
		smallestIndex = 0;
	bool reachedLimit = false;

	int x=0, y=0,					// x and y of current node		
		total;					// Total sum from current position to end
	
	int numExpansions = 1;
	int *path;
	
	Node *dueNodes[MAX];		// Array of nodes due to analysis
	Node *tempNode;
	Mark *visited;
	
	//--------------------------------------------------------------------------------
	visited = (Mark*) malloc((line * line) * sizeof(Mark));
	InitializeMarks(visited, line);
	
	memNode[0].x = start.x;
	memNode[0].y = start.y;
	memNode[0].sumCost = 0;
	memNode[0].step = 1;
	memNode[0].previous = NULL;
	memNode[0].modulus = ManhattanModulus(start.x, start.y, end.x, end.y);
	memNode[0].function = memNode[0].modulus;
	printf("\n x_%d,y_%d,z_%d,w_%d, \n",start.x, start.y, end.x, end.y);
	dueNodes[0] = &memNode[0];
	
	// Iteration to find best path to goal
	while(!arrived){
		if(!memoryLimit){
	 		printf("ERROR: Interrupted for lack of memory.\n");
	 		return NULL;
	 	}
        
		// Decides for smallest cost node
		smallest=0;
		reachedLimit=false;
		for(int i = 0; i < numExpansions; i++) {
 			// Items which have already been removed
			if(dueNodes[i] != NULL) { 
				reachedLimit = true;
				if (smallest == 0) {
					smallest = dueNodes[i]->function;
					//printf("Smallest:%d\ti: %d\n", smallest, i); 
					smallestIndex = i;
 				} else if ((dueNodes[i]->function) < smallest) {
	                smallest = dueNodes[i]->function;
	                smallestIndex = i;
					//printf("Smallest:%d\ti: %d\n", smallest, i); 
	            }
			} else {
				if(!reachedLimit) {
					inferiorLimit++;
				}
			}
        }
	//	printf("\n\n\t Madei um OI!");
		// Checks if every node has been expanded
		if(dueNodes[smallestIndex] == NULL){
			printf("\nGoal not found. Expansion index: %d\tPoint: (%d, %d)\n", smallestIndex, numExpansions, x, y);
			//exit(1);
			for(int i = 0; i < numExpansions; i++){
				if(dueNodes[i] != NULL)
					printf("Position: %d\tCost: %d\tPoint: (%d, %d)", i, dueNodes[i]->sumCost, dueNodes[i]->x, dueNodes[i]->y);
			}
			return NULL;
		}
 		
		// Expanding adjacent nodes...
		x = dueNodes[smallestIndex]->x + 1;	// Starts with left adjacent node
		y = dueNodes[smallestIndex]->y;

		int numAdjacents = 4;

		while(numAdjacents--) {
			if(arrived)
				break;
			
			// Changes conditions for each adjacency
 			if(numAdjacents == 2) {
 				x = x - 2;
			} else if(numAdjacents == 3) {
				// Do nothing...
			} else if(numAdjacents == 1) {
 				x = x + 1;
 				y = y + 1;
 			} else if(numAdjacents == 0) {
 				y = y - 2;
 			} else {
 				printf("ERROR: Unexpected error in number of adjacent nodes.\n");
 				return NULL;
 			}
		//	printf("\n xxxx_%d, yyyy_%d\n",x,y);
			// Within map boundaries...
			if((x >= 0) && (x < line) && (y < line) && (y >= 0)) {
			//	printf("\n PASSEI!\n");
				// If node has not been analysed and map position is safe
				if(!visited[x * line + y].closed && MAP_GetPositionSafetyStatus(map, y* line + x)) { 
					//printf("\n UUHLLL EH SAFE!\n");
					
					// If node has not been visited...
					if(!visited[x * line + y].marked){ 
						// Make expansion!
						numExpansions++;
						memoryLimit--;
						
						// Put node in line of expansion and analysis
						dueNodes[numExpansions - 1] = &memNode[numExpansions - 1];
						dueNodes[numExpansions - 1]->x = x;
						dueNodes[numExpansions - 1]->y = y;
						dueNodes[numExpansions - 1]->sumCost = dueNodes[smallestIndex]->sumCost + 1;
						dueNodes[numExpansions - 1]->modulus = ManhattanModulus(x, y, end.x, end.y);
						dueNodes[numExpansions - 1]->function = dueNodes[numExpansions - 1]->sumCost + dueNodes[numExpansions - 1]->modulus;
						dueNodes[numExpansions - 1]->previous = dueNodes[smallestIndex];
						dueNodes[numExpansions - 1]->step = dueNodes[smallestIndex]->step + 1;

						// Index and control visited nodes
						visited[x * line + y].marked = true;
						visited[x * line + y].where = dueNodes[numExpansions - 1];
						
						// Check if x and y are end point coordinates
						if(end.x == x && end.y == y){
							arrived = true;
							//break;
						}
					} /* End of not visited if */
					// If node has been visited...
					else { 
						total = dueNodes[smallestIndex]->sumCost + 1 + ManhattanModulus(x, y, end.x, end.y);

						// Checks if new path is better than previous
						if(visited[x * line + y].where->function > total) {
							visited[x * line + y].where->modulus = ManhattanModulus(x, y, end.x, end.y);
							visited[x * line + y].where->sumCost = total - ManhattanModulus(x, y, end.x, end.y);
							visited[x * line + y].where->function = total;
							visited[x * line + y].where->step = dueNodes[smallestIndex]->step + 1;
							visited[x * line + y].where->previous = dueNodes[smallestIndex];
						}
					} 
				} /* End of is walkable node if */

				/*If is not close_visited, not walkable, maybe is destiny*/
				else if ((end.x==x && end.y==y )){
					printf("\nFor the HONOR!");
						// Make expansion!
						numExpansions++;
						memoryLimit--;
						
						// Put node in line of expansion and analysis
						dueNodes[numExpansions - 1] = &memNode[numExpansions - 1];
						dueNodes[numExpansions - 1]->x = x;
						dueNodes[numExpansions - 1]->y = y;
						dueNodes[numExpansions - 1]->sumCost = dueNodes[smallestIndex]->sumCost + 1;
						dueNodes[numExpansions - 1]->modulus = ManhattanModulus(x, y, end.x, end.y);
						dueNodes[numExpansions - 1]->function = dueNodes[numExpansions - 1]->sumCost + dueNodes[numExpansions - 1]->modulus;
						dueNodes[numExpansions - 1]->previous = dueNodes[smallestIndex];
						dueNodes[numExpansions - 1]->step = dueNodes[smallestIndex]->step + 1;

						// Index and control visited nodes
						visited[x * line + y].marked = true;
						visited[x * line + y].where = dueNodes[numExpansions - 1];
				}/*End of if is not close_visited, not walkable, maybe is destiny*/

			} /* End of boundaries if */
 		} /* End of numAdjacents while */
 		
		// Eliminates analysed node
		visited[dueNodes[smallestIndex]->x * line + dueNodes[smallestIndex]->y].closed = true;
		dueNodes[smallestIndex] = NULL;
 	} /* End of arrived while */
	
	tempNode = visited[x * line + y].where;
	tempNode->step--;
 	total = (tempNode->step * 2) + 4;	// Times 2 for coordinates x and y
 	smallest = tempNode->sumCost;
 	
	// Create path
	path = (int*) malloc(total * sizeof(int) + 10);	// Plus 10 is safety neura...
	if(path == NULL){
		printf("ERROR: Not enough memory to allocate path.\n");
		return NULL;
	}

	// First two indexes of path array keep array lenght and (smallest) cost of moving forward
	path[0] = total;
	path[1] = smallest;
	 	
	// From second to final index of path array are the coordinates.
	// x will always be in an even position and y in an odd one.
	for(int i = 2; i < total; i += 2){
		if(tempNode == NULL) {
			printf("ERROR: Null pointer to node.\n");
			break;
		}

	 	path[i] = tempNode->x;
	 	path[i + 1] = tempNode->y;
	 	tempNode = tempNode->previous;
	}

	/*printf("Total: %d\tSmallest: %d\n", path[0], path[1]);*/
	/*printf("End: (%d, %d)\tStart: (%d, %d)\n", path[2], path[3], path[total - 2], path[total - 1]);*/
	 	
	if (tempNode == NULL){
	 	return path;
	} else {	
	 	printf("Error: Unexpected erro with node pointer.\n");
	 	return NULL;
	}

	free(visited);
 }


//-----------------------------------------------------------
//	AUX FUNCTIONS
//-----------------------------------------------------------
int ManhattanModulus(int x, int y, int z, int w){
	int modA, modB;
	modA = x - z;
	modB = y - w;
	if (modA < 0)
		modA *= -1;
	if (modB < 0)
		modB *= -1;
	return (modA + modB);
}

void InitializeMarks(Mark *mark, int column){
	for(int i = 0; i < column; i++){
		for(int j = 0; j < column; j++){
			mark[i * column + j].closed = false;
			mark[i * column + j].marked = false;
			mark[i * column + j].where = NULL;
		}
	}
}