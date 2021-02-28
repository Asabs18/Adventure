#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <conio.h> 
#include <process.h>
#include <direct.h>
#include "debug.h"
#include "room.h"
#include "rooms.h"
#include "looks.h"
#include "main.h"
static_assert(USED_ROOMS == MAX_CONNECTIONS, "USED_ROOMS not equal to MAX_CONNECTIONS");
static_assert(USED_ROOMS == NUM_FILES, "USED_ROOMS and NUM_FILES need to be the same number.");
void writeToFiles(room_t* rooms[]) {
	int pId = _getpid();
	char NameBuffer[_MAX_FNAME];
	sprintf_s(NameBuffer, sizeof(NameBuffer), dirNameTemplate, pId);
	int dir = _mkdir(NameBuffer);
	for (size_t i = 0; i < USED_ROOMS; i++) {
		//creates file name
		sprintf_s(NameBuffer, sizeof(NameBuffer), fileNameTemplate, pId, i);
		//opens file
		FILE* file;
		errno_t err = fopen_s(&file, NameBuffer, "w");
		assert(err == 0);
		assert(file != NULL);
		//writes one room to the file
		printRoom(file, rooms[i], rooms);
		//closes file
		fclose(file);
	}
}
void main() {
	initialize_debugging();
	room_t** rooms = createRooms(roomNames, USED_ROOMS, roomTypes, USED_ROOMS);
	//printRooms(stdout, rooms, USED_ROOMS); used to print files but I don't need this call for the real game so I commented it out.
	writeToFiles(rooms);
	destroyRooms(rooms);
	terminate_debugging();
}


//#include <stdio.h>
//
//static const char* file =
//"ROOM NAME : Pennsylvania\n"
//"CONNECTION 1 : New York\n"
//"CONNECTION 2 : California\n"
//"CONNECTION 3 : Nevada\n"
//"ROOM TYPE : START_ROOM\n";
//
//int main() {
//	int a;
//	char buffer[1024] = { 0 };
//	a = sscanf(file, "ROOM NAME : %s\n", buffer);
//	int connectionIndex;
//	for (int i = 0; i < 7; i++) {
//		a = sscanf(file, "CONNECTION %d : %s\n", &connectionIndex, buffer);
//		if (a == 0) {
//			break;
//		}
//	}
//	a = sscanf(file, "ROOM TYPE : %s\n", buffer);
//	return 0;
//}