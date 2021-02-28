#include "rooms.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "connections.h"
void* shuffle(void* array[], size_t size) {
	assert(array != NULL);
	//loops through any array
	void* Hold;
	for (size_t i = 0; i < size; i++) {
		//creates a random number from 1 - the size of the array
		size_t r = rand() % size;
		assert(r >= 0);
		assert(r <= size);
		//creates a hold var and swaps a random point in the arrays
		Hold = array[r];
		array[r] = array[i];
		array[i] = Hold;
	}
	//returns the array
	return array;
}

room_t** createRooms(char* roomNames[], size_t Ncount, roomType_t roomTypes[], size_t Tcount) {
	//srand((unsigned int)time(NULL));
	srand(0);
	//creates an array of rooms
	room_t** rooms = malloc(sizeof(room_t*) * USED_ROOMS);
	assert(rooms);
	//shuffles the room names and types
	shuffle(roomNames, Ncount);
	//fills the array with correctly made rooms
	assert(Tcount == Ncount);
	for (size_t i = 0; i < Ncount; i++) {
		rooms[i] = createRoom(roomNames[i], roomTypes[i]);
	}
	// Create all connections in graph 
	while (IsGraphFull(rooms, USED_ROOMS) == false) {
		AddRandomConnection(rooms, USED_ROOMS);
	}
	return rooms;
}

bool printRooms(FILE* fp, room_t* rooms[], size_t size) {
	//calls function that prints 1 room multiple times through loop
	for (size_t i = 0; i < size; i++) {
		printRoom(fp, rooms[i], rooms);
	}
	return true;
}
destroyRooms(room_t** rooms) {
	//frees multiple rooms memory
	assert(rooms != NULL);
	for (int i = 0; i < USED_ROOMS; i++) {
		destroyRoom(rooms[i]);
	}
	free(rooms);
}