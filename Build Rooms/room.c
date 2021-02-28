#include "room.h"
#include <time.h>
#include <assert.h>
#include <string.h>
room_t* createRoom(const char* roomName, roomType_t roomType) {
//"mallocs" the size of a room
	room_t* room = malloc(sizeof(room_t));
//assigns the proper room names and types and connections
	assert(room);
	room->roomName = _strdup(roomName);
	room->roomType = roomType;
	room->numOfConnections = 0;
//sets all connections to false - for loop
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		room->connections[i] = false;
	}
	return room;
}

//prints one room
bool printRoom(FILE* fp, const room_t* room, const room_t* rooms[]) {
	int count = 0;
	fprintf(fp, "ROOM NAME : %s\n", room->roomName);
//prints connections of 1 rooms
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (room->connections[i] == true) {
			count++;
			fprintf(fp, "CONNECTION %d : %s\n", count, rooms[i]->roomName);
		}
	}
//sets to string and prints "room type"
	char* roomTypeString = NULL;
	switch (room->roomType) {
	case START_ROOM:
		roomTypeString = "START_ROOM";
		break;
	case MID_ROOM:
		roomTypeString = "MID_ROOM";
		break;
	case END_ROOM:
		roomTypeString = "END_ROOM";
		break;
	default:
		assert(false);
		break;
	}
	fprintf(fp, "ROOM TYPE : %s\n", roomTypeString);
	printf("\n");
	return true;
}
bool destroyRoom(room_t* room) {
//frees one rooms memory
	assert(room != NULL);
	free(room->roomName);
	free(room);
	return true;
}