#pragma once 
#include "rooms.h"
#include "room.h"

bool IsGraphFull(room_t* rooms[], size_t count);
size_t mapRoomToRoomIndex(const Room* rooms[], size_t count, const room_t* room);
void AddRandomConnection(room_t* rooms[], size_t count);
bool IsRoomFull(const Room* room);

// Connects Rooms x and y together, does not check if this connection is valid 
inline void ConnectRoom(const Room* rooms[], size_t count, Room* x, Room* y) {
	x->connections[mapRoomToRoomIndex(rooms, count, y)] = true;
	x->numOfConnections++;
}

// Returns true if Rooms x and y are the same Room, false otherwise
inline bool IsSameRoom(Room* x, Room* y) {
	return x == y;
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
inline bool ConnectionAlreadyExists(const Room* rooms[], size_t count, const Room* room1, const Room* room2) {
	return room1->connections[mapRoomToRoomIndex(rooms, count, room2)];
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise 
inline bool CanAddConnectionFrom(Room* x) {
	return x->numOfConnections < MAX_ALLOWED_CONNECTIONS;
}

// Returns a random Room, does NOT validate if connection can be added 
inline Room* GetRandomRoom(Room* rooms[], size_t count) {
	return rooms[rand() % count];
}