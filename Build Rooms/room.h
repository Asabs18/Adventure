#pragma once
#define MAX_ROOMS 10
#define USED_ROOMS 7
#define NUM_FILES 7
#define MIN_CONNECTIONS 3
#define MAX_CONNECTIONS 7
#define MIN_ALLOWED_CONNECTIONS 3
#define MAX_ALLOWED_CONNECTIONS 6
#include "debug.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//room value types
typedef enum _roomType_t {
	START_ROOM,
	MID_ROOM,
	END_ROOM
}   roomType_t;

//representation of a room
typedef struct _room_t {
	char* roomName;
	roomType_t roomType;
	bool connections[MAX_CONNECTIONS];
	size_t numOfConnections;
}   room_t, Room;

//names for room builder
static const char* fileNameTemplate = "sabatinia.rooms.%d\\room%d.txt";
static const char* dirNameTemplate = "sabatinia.rooms.%d";
static char* roomNames[] = { "New York", "California", "Colorado", "Arizona", "Florida", "Nevada", "Pennsylvania", "Vermont", "Virgina", "Alaska" };
static roomType_t roomTypes[] = { START_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, END_ROOM };
const room_t* rooms[USED_ROOMS];
static_assert(sizeof(void*) == sizeof(roomType_t), "Sizes need to be equal in order to use shuffle for both");

room_t* createRoom(const char* roomName, roomType_t roomType);
bool printRoom(FILE* fp, const room_t* room, const room_t* rooms[]);
bool destroyRoom(room_t* room);