#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>

#ifndef PACKAGE
#  define PACKAGE "Maze"
#endif

#define CHECK_ALLOCATE(var)\
  if (var == NULL) {\
    fprintf(stderr, PACKAGE ": cannot allocate variable `" #var "'\n");\
    exit(EXIT_FAILURE);\
  }

#define FREE_ALLOCATED_SPACE(var)\
  if (var != NULL) {\
    free(var);\
  }

void carve(char** maze, int mazeRows, int mazeCols, int carveRow, int carveCol);
void randomDirections(int list[4][2]);
int findSeed(char** maze, int mazeRows, int mazeCols, int* seedRowPtr, int* seedColPtr);
void Generate_Objectives(char** maze,int w,int h,int stage);

char** Generate_Maze(int w,int h){ 
	char** maze;
	maze = (char**) malloc(w*sizeof(char*));
	for(int i = 0; i < w; i++){
    	maze[i] = (char*) malloc((h)*sizeof(char));
  	}
  	CHECK_ALLOCATE(maze);
    for(int r = 0; r < w; r++) {
		for(int c = 0; c < h; c++) {
			if(r < w && c < h)
				maze[r][c] = '#';
			else
				maze[r][c] = '\0';
		}
	}
    for(int r = 0; r < w; r++)
		maze[r][h] = '\0';

  	maze[1][1] = 'O';
  	int seedR, seedC;

	while(findSeed(maze, w, h, &seedR, &seedC))
		carve(maze, w, h, seedR, seedC);

	maze[w - 1][h - 2] = 'G';
	maze[w - 2][h - 2] = 'B';
	
  	for(int i = 0; i < w; i++){
    	for(int j = 0; j < h; j++){
      		if( maze[i][j] == 'O' ){
        		maze[i][j] = ' ';
      		}
    	}    
  	}
	
  	return maze;
}

int findSeed(char** maze, int mazeRows, int mazeCols, int* seedRowPtr, int* seedColPtr) {
	int relAdjs[4][2];
	randomDirections(relAdjs);
	
	for(int r = 1; r < mazeRows; r += 2) {
		for(int c = 1; c < mazeCols; c += 2) {
			if(maze[r][c] == '#') {
				for(int i = 0; i < 4; i++) {
					int tempR = r + relAdjs[i][0] * 2;
					int tempC = c + relAdjs[i][1] * 2;
					if(tempR >= 0 && tempR < mazeRows && tempC >= 0 && tempC < mazeCols && maze[tempR][tempC] == 'O') {
						maze[r][c] = 'O';
						maze[r + relAdjs[i][0]][c + relAdjs[i][1]] = 'O';
						*seedRowPtr = r;
						*seedColPtr = c;
						return 1;
					}
				}
			}
		}
	}
	
	return 0;
}

void carve(char** maze, int mazeRows, int mazeCols, int carveRow, int carveCol) {
	int movePossible = 1;
	int moves[4][2];
	while(movePossible) {
		movePossible = 0;
		randomDirections(moves);
		for(int i = 0; i < 4; i++) {
			int tempR = carveRow + moves[i][0] * 2;
			int tempC = carveCol + moves[i][1] * 2;
			if(tempR >= 0 && tempR < mazeRows && tempC >= 0 && tempC < mazeCols && maze[tempR][tempC] == '#') {
				maze[tempR][tempC] = 'O';
				maze[carveRow + moves[i][0]][carveCol + moves[i][1]] = 'O';
				carveRow = tempR;
				carveCol = tempC;
				movePossible = 1;
				break;
			}
		}
	}
}

void randomDirections(int list[4][2]) {
	int remain = 4;
	int possibleDirections[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	while(remain > 0) {
		int choose = rand() % remain;
		list[4 - remain][0] = possibleDirections[choose][0];
		list[4 - remain][1] = possibleDirections[choose][1];
		remain--;
		for(int i = choose; i < 4; i++) {
			possibleDirections[i][0] = possibleDirections[i + 1][0];
			possibleDirections[i][1] = possibleDirections[i + 1][1];
		}
	}
}

void Generate_Objectives(char** maze,int w,int h,int stage){
	int x = w-2;
	int y = h-2;
	int i,j;

	time_t t;
	srand((unsigned) time(&t));

	for(int k = 0; k < stage*5+2; k++){
		do{
			i = rand()%x + 1;
			j = rand()%y + 1;
		}while(maze[i][j] != ' ');

		maze[i][j] = 'T';
	}

	for(int k = 0; k < stage*5+2; k++){
		do{
			i = rand()%x + 1;
			j = rand()%y + 1;
		}while(maze[i][j] != ' ');

		maze[i][j] = 'M';
	}
}