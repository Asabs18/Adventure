#pragma once

#include <stdbool.h>
#include <stdio.h>

#define MAX_ROOMS 10
#define USED_ROOMS 7
#define NUM_FILES 7
#define MIN_CONNECTIONS 3
#define MAX_CONNECTIONS 7
#define MIN_ALLOWED_CONNECTIONS 3
#define MAX_ALLOWED_CONNECTIONS 6

typedef enum _roomType_t {
	START_ROOM = 3,
	MID_ROOM,
	END_ROOM
}   roomType_t;

typedef struct {
	char* name;
	char* connections[MAX_CONNECTIONS];
	roomType_t roomType;
}   room_t;
room_t rooms[USED_ROOMS];

bool printRoom(const room_t room, const room_t rooms[]);
bool printRooms(const room_t rooms[], size_t size);
void generateRandomRooms(room_t rooms[], size_t numRooms);
bool isConnected(const room_t room1, const room_t room2);
void addConnection(room_t* room1, room_t* room2);
void ensureConnectivity(room_t rooms[], size_t numRooms);
void dfs(room_t rooms[], size_t index, bool visited[], size_t numRooms);