#include <assert.h>
#include "connections.h"
// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull(room_t* rooms[], size_t count) {
//int fullRooms = 0;
	assert(rooms != NULL);
	bool graphFull = true;
	for (size_t i = 0; i < count; i++) {
		if (IsRoomFull(rooms[i]) == false) {
			graphFull = false;
			break;
		}
		/*	else {
				fullRooms++;
			}*/
	}
	return graphFull;
}

// Returns true if a single room has 3 to 6 outbound connections, false otherwise
bool IsRoomFull(const Room* room) {
	size_t count = 0;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (room->connections[i]) {
			count++;
		}
	}
	assert(count <= 6);
	return MIN_ALLOWED_CONNECTIONS < count && count <= MAX_ALLOWED_CONNECTIONS;
}

size_t mapRoomToRoomIndex(const Room* rooms[], size_t count, const room_t* room) {
	for (size_t i = 0; i < count; i++) {
		if (room == rooms[i]) {
			return i;
		}
	}
	assert(false);
	return -1;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(room_t* rooms[], size_t count) {
	Room* A;
	Room* B;

	while (true) {
		A = GetRandomRoom(rooms, count);

		if (CanAddConnectionFrom(A) == true) {
			break;
		}
	}

	do {
		B = GetRandomRoom(rooms, count);
	} while (CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(rooms, count, A, B) == true);

	ConnectRoom(rooms, count, A, B);
	ConnectRoom(rooms, count, B, A);
}