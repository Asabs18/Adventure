#include <stdio.h>
#include <process.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>
#include "room.h"
#include "game.h"

void main() {
	int steps = 0;
	char* path[260];
	char* nextRoom;
	//Get the name of the correct Directory
	char* dirName = findRoomsDir();
	assert(dirName != NULL);
	//fills the rooms array
	room_t* currentRoom = readFromFiles();
	printf("Welcome to Adventure!\n");
	//++++++++++START OF MAIN GAME LOOP++++++++++//
	do {
		//Prompts the user for a next move and checks if it is correct
		nextRoom = output(currentRoom);
		//Updates the Current Room and updates the path/steps
		for (int i = 0; i < USED_ROOMS; i++) {
			if (0 == strcmp(nextRoom, rooms[i].name)) {
				currentRoom = &rooms[i];
				break;
			}
		}
		path[steps] = currentRoom->name;
		steps++;
	} while (currentRoom->roomType != END_ROOM);
	//Prints the case for winning the game
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
	for (int i = 0; i < steps; i++) {
		printf(path[i]);
		printf("\n");
	}
	free(dirName);
}