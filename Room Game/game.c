#include "room.h"
#include "game.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
//#include <WinNT.h>  //----- There is an issue with this header file
#include <windows.h>

//Creates and prints the output to the user for the game
char* output(room_t* currentRoom) {
	char userInput[FILENAME_MAX];
	char* nextRoom = NULL;
	//Prints current room info
	printf("CURRENT LOCATION: %s\n", currentRoom->name);
	printf("POSSIBLE CONNECTIONS: \n");
	
	int i = 0;
	while (i < MAX_CONNECTIONS && currentRoom->connections[i] != NULL) {
		printf("%s \n", currentRoom->connections[i]);
		i++;
	}
	//prompts the user for a new location and checks if it is a valid room
	bool validConnection = false;
	while (validConnection == false) {
		printf("\nWHERE TO? > ");
		fgets(userInput, sizeof(userInput), stdin);
		userInput[strlen(userInput) - 1] = '\0';
		printf("\n");
		for (int i = 0; i < MAX_CONNECTIONS; i++) {
			if (currentRoom->connections[i] == 0) {
				printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
				validConnection = false;
				break;
			}
			if (0 == strcmp(userInput, currentRoom->connections[i])) {
				nextRoom = currentRoom->connections[i];
				validConnection = true;
				break;
			}
		}
	}
	return nextRoom;
}

//Finds the most recently created VALID room Directory
char* findRoomsDir() {
	static const char roomsTemplate[] = "sabatinia.rooms.*";
	int pid;
	int scanReturn;
	//int pid, scanReturn;

	// A directory that matches roomsTemplate
	struct _finddata_t roomsDir;
	struct _finddata_t newestDir = { 0 };
	intptr_t handle;

	//finds first file with roomsTemplate
	if ((handle = _findfirst(roomsTemplate, &roomsDir)) != -1L) {
		do {
			//Checks if it is a directory or not 
			//TODO: bitwise(Need to talk about)
			if ((roomsDir.attrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
				//checks if last part(PID) is a number and checks if its newer than the current newest functions
				scanReturn = sscanf_s(roomsDir.name, "sabatinia.rooms.%d", &pid);
				if (scanReturn != 0 && roomsDir.time_create > newestDir.time_create) {
					newestDir = roomsDir;
				}
			}
			//runs while there are still files with this template
		} while (_findnext(handle, &roomsDir) == 0);
		_findclose(handle);
	}
	return newestDir.name[0] == '\0' ? NULL : _strdup(newestDir.name);
}

//Reads data from one of the files in dirName
room_t* readFromFile(const char* fileName, room_t* room) {
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName, "r");
	if (fp == NULL) {
		perror("fopen() failed");
	}
	assert(err == 0);
	assert(fp != 0);
	//reads in the room name
	room->name = readName(fp, room);

	//reads in the rooms connections
	int i = 0;
	while ((room->connections[i] = readConnection(fp, room, i)) != NULL) {
		i++;
	}

	//reads in the rooms type
	room->roomType = readRoomType(fp, room);
	//closes the file and frees dirName
	fclose(fp);

	return room;
}

//Reads the rooms type
roomType_t readRoomType(FILE* fp, room_t* room) {
	int sscanReturn;
	char buffer[FILENAME_MAX];
	sscanReturn = fscanf_s(fp, "ROOM TYPE : %s\n", buffer, FILENAME_MAX);
	assert(sscanReturn == 1);
	char* roomTypeString = NULL;
	//Converts the char* buffer value to the room type enum value
	if (strcmp(buffer, "START_ROOM") == 0) {
		room->roomType = START_ROOM;
	}
	else if (strcmp(buffer, "MID_ROOM") == 0) {
		room->roomType = MID_ROOM;
	}
	else {
		room->roomType = END_ROOM;
	}
	return room->roomType;
}

//Reads one of a rooms connections
char* readConnection(FILE* fp, room_t* room, int connectionIndex) {
	int sscanReturn;
	char buffer[FILENAME_MAX] = { '\0' };
	//throw away var
	int connectionNumber = connectionIndex + 1;
	//read a connection and returns NULL if the case fails
	sscanReturn = fscanf_s(fp, "CONNECTION %d : %[^\n]\n", &connectionNumber, buffer, FILENAME_MAX);
	room->connections[connectionIndex] = _strdup(buffer);
	if (sscanReturn == 0) {
		room->connections[connectionIndex] = NULL;
	}
	return room->connections[connectionIndex];
}

//Reads the rooms names
char* readName(FILE* fp, room_t* room) {
	int sscanReturn;
	char buffer[FILENAME_MAX];
	//Reads and returns the room name
	sscanReturn = fscanf_s(fp, "ROOM NAME : %[^\n]\n", buffer, FILENAME_MAX);
	assert(sscanReturn == 1);
	room->name = _strdup(buffer);
	return room->name;
}

room_t* readFromFiles() {
	char buffer[FILENAME_MAX];
	room_t* currentRoom = NULL;
	static const char fileTemplate[] = "%s\\room%d.txt";
	char* dirName = findRoomsDir();
	assert(dirName != NULL);
	//Loops through each room file and reads it info
	for (int i = 0; i < NUM_FILES; i++) {
		sprintf_s(buffer, sizeof(buffer), fileTemplate, dirName, i);
		rooms[i] = *readFromFile(buffer, &rooms[i]);
		if (rooms[i].roomType == START_ROOM) {
			assert(currentRoom == NULL);
			currentRoom = &rooms[i];
		}
	}
	return currentRoom;
}