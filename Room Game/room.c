#pragma once
#include <assert.h>
#include "room.h"

bool printRooms(const room_t rooms[], size_t size) {
	//calls function that prints 1 room multiple times through loop
	for (size_t i = 0; i < size; i++) {
		printRoom(rooms[i], rooms);
	}
	return true;
}

bool printRoom(const room_t room, const room_t rooms[]) {
	int count = 0;
	printf("ROOM NAME : %s\n", room.name);
	//prints connections of 1 rooms
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		count++;
		printf("CONNECTION %d : %s\n", count, rooms[i].name);
	}
	//sets to string and prints "room type"
	char* roomTypeString = NULL;
	switch (room.roomType) {
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
	//returns true to show the function was successful 
	printf("ROOM TYPE : %s\n", roomTypeString);
	printf("\n");
	return true;
}