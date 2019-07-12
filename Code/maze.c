#include "maze.h"

// 2D array to store data for each cell
// Important!!! JUST access data via the DEFINEd commands
//
// Bits:
//
// 1-11: 	DISTANCE to start
// 12:		Visited flag
// 13:		(1) wall in the north (0) no wall in the north
// 14:		(1) wall in the east (0) no wall in the east
// 15:		(1) wall in the south (0) no wall in the south
// 16:		(1) wall in the west (0) no wall in the west


int node[6][6] = {
			{0,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255}
			};
			
// 2D array to store the X and Y index for bevorhand visited cell for each cell
// Important!!! JUST access data via the DEFINEd commands
//
// Bits:
//
// 1-8: 	Y
// 9-16:	X

int last[6][6] = {
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1}
			};

//TODO find and follow the shortest path!!!!!!!!!
			
// explores the maze starting at (0,0) 
// after exploration the mouse is at (0,0) again
// storing distance from start to cell for each cell 
// distances can be accessed by DEFINEd command
// 	
//	as example:
//
//	int i, j;	
//	for(i = 0 ; i < 6 ; i++){
//		for(j = 0 ; j < 6 ; j++){
//			printf("%2d ", DISTANCE(node[i][j]));
//		}
//		printf("\n");
//	} 

void explore(){
	
	//initial setup
	int current_X = 0;
	int current_Y = 0;
	char direction = 'N';
	SET_VISITED(node[current_X][current_Y]);
	
	int run = 1;
	while(run){
		
		//update Walls from sensor data
		setWalls(current_X, current_Y, direction, getRight(), getLeft(), getFront());
		
		//check wether there are unvisited neighbors or not 
		//either go to unvisited cell or go back to last visited one
		
		if(neighborNorthUnvisited(current_X,current_Y)){
			int temp = current_Y;
			current_Y++;
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X][temp])+1));
			SET_LAST_X(last[current_X][current_Y], current_X);
			SET_LAST_Y(last[current_X][current_Y], temp);
			SET_VISITED(node[current_X][current_Y]);
			direction = 'N';
			
			//TODO drive North
			
		} else if(neighborSouthUnvisited(current_X,current_Y)){
			int temp = current_Y;
			current_Y--;
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X][temp])+1));
			SET_LAST_X(last[current_X][current_Y], current_X);
			SET_LAST_Y(last[current_X][current_Y], temp);
			SET_VISITED(node[current_X][current_Y]);
			direction = 'S';
			
			//TODO drive South
			
		} else if(neighborEastUnvisited(current_X,current_Y)){
			int temp = current_X;
			current_X++;
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[temp][current_Y])+1));
			SET_LAST_X(last[current_X][current_Y], temp);
			SET_LAST_Y(last[current_X][current_Y], current_Y);
			SET_VISITED(node[current_X][current_Y]);
			direction = 'E';
			
			//TODO drive East
			
		} else if(neighborWestUnvisited(current_X,current_Y)){
			int temp = current_X;
			current_X--;
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[temp][current_Y])+1));
			SET_LAST_X(last[current_X][current_Y], temp);
			SET_LAST_Y(last[current_X][current_Y], current_Y);
			SET_VISITED(node[current_X][current_Y]);
			direction = 'W';
			
			//TODO drive West
			
		} else if(current_X == 0 && current_Y == 0)
			run = 0;
		else {
			int new_X = LAST_X(last[current_X][current_Y]);
			int new_Y = LAST_Y(last[current_X][current_Y]);
			if (new_X < 6 && new_Y < 6){
				current_X = new_X;
				current_Y = new_Y;
				
				//TODO drive back to last
				
			} else
				run = 0;
		}
	}	
}

//checks if there is an unvisited neighbor in the north
//else updates distance

int neighborNorthUnvisited(int X, int Y){
	int check = 0;
	if(!NORTH_Wall(node[X][Y])){
		if(!VISITED(node[X][Y+1]))
			check = 1;
		else
			SET_DISTANCE(node[X][Y], min(DISTANCE(node[X][Y]), DISTANCE(node[X][Y+1])+1));
    }
	return check;
}

//checks if there is an unvisited neighbor in the east
//else updates distance

int neighborEastUnvisited(int X, int Y){
	int check = 0;
	if(!EAST_Wall(node[X][Y])){
		if(!VISITED(node[X+1][Y]))
			check = 1;
		else
			SET_DISTANCE(node[X][Y], min(DISTANCE(node[X][Y]), DISTANCE(node[X+1][Y])+1));
    }
	return check;
}

//checks if there is an unvisited neighbor in the south
//else updates distances

int neighborSouthUnvisited(int X, int Y){
	int check = 0;
	if(!SOUTH_Wall(node[X][Y])){
		if(!VISITED(node[X][Y-1]))
			check = 1;
		else
			SET_DISTANCE(node[X][Y], min(DISTANCE(node[X][Y]), DISTANCE(node[X][Y-1])+1));
    }
	return check;
}

//checks if there is an unvisited neighbor in the west
//else updates distance

int neighborWestUnvisited(int X, int Y){
	int check = 0;
	if(!WEST_Wall(node[X][Y])){
		if(!VISITED(node[X-1][Y]))
			check = 1;
		else
			SET_DISTANCE(node[X][Y], min(DISTANCE(node[X][Y]), DISTANCE(node[X-1][Y])+1));
    }
	return check;
}

//sets wall at positon X, Y regarding the current direction we're looking at
//right, left, front have to be pulled from sensors.c
//all walls except for borders have to be set for both adjacent cells

void setWalls(int X, int Y, char dir, int right, int left, int front){
	if(dir == 'N'){
		if(right){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
		if(left){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
		if(front){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
	} else if(dir == 'E'){
		if(right){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
		if(left){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
		if(front){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
	} else if(dir == 'S'){
		if(right){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
		if(left){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
		if(front){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
	} else if(dir == 'W'){
		if(right){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
		if(left){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
		if(front){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
	}  
}

int getRight(){
	//TODO get sensor.c input
	return 1;
}

int getLeft(){
	//TODO get sensor.c input
	return 1;
}

int getFront(){
	//TODO get sensor.c input
	return 1;
}

//JUST for test purpose!!!!!!!!!

void setBorder(){
	int i;
	for(i = 0 ; i < 6 ; i++)
		setWalls(i, 5, 'N', 0, 0, 1);	
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(i, 0, 'S', 0, 0, 1);
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(5, i, 'E', 0, 0, 1);
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(0, i, 'W', 0, 0, 1);
	
	for(i = 0 ; i < 4 ; i++)
		setWalls(i, 4, 'N', 0, 0, 1);
	
	for(i = 3 ; i < 5 ; i++)
		setWalls(i, 2, 'N', 0, 0, 1);
	
	for(i = 1 ; i < 5 ; i++)
		setWalls(i, 0, 'N', 0, 0, 1);
	
	for(i = 1 ; i < 2 ; i++)
		setWalls(4, i, 'E', 0, 0, 1);
	
	for(i = 1 ; i < 2 ; i++)
		setWalls(3, i, 'E', 0, 0, 1);
	
	for(i = 2 ; i < 4 ; i++)
		setWalls(1, i, 'E', 0, 0, 1);
	
	for(i = 1 ; i < 5 ; i++)
		setWalls(0, i, 'E', 0, 0, 1);
	
	printmaze();
}

//tries to print the maze
//ugly yet

void printmaze(){
	int i, j;
	for(i = 0 ; i < 6 ; i++){
		for(j = 0 ; j < 6 ; j++){
			if(SOUTH_Wall(node[i][j]))
				printf("[  ");
			else
				printf("   ");
			if(WEST_Wall(node[i][j]))
				printf(" - ");
			else
				printf("   ");
			if(EAST_Wall(node[i][j]))
				printf("_  ");
			else
				printf("   ");
			if(NORTH_Wall(node[i][j]))
				printf("]  ");
			else
				printf("   ");
		}
		printf("\n");
		printf("\n");
	}
}

//just an easy minimum function

int min(int a, int b){
	return a < b ? a : b;
}