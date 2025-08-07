#pragma once
#include <assert.h>
#include <stdlib.h>
#include <time.h>
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

void generateRandomRooms(room_t rooms[], size_t numRooms) {
    srand((unsigned int)time(NULL));

    // Assign random names and types to rooms
    for (size_t i = 0; i < numRooms; i++) {
        rooms[i].name = (char*)malloc(20 * sizeof(char));
        sprintf(rooms[i].name, "Room_%zu", i);
        rooms[i].roomType = (i == 0) ? START_ROOM : (i == numRooms - 1) ? END_ROOM : MID_ROOM;

        for (size_t j = 0; j < MAX_CONNECTIONS; j++) {
            rooms[i].connections[j] = NULL;
        }
    }

    // Randomly connect rooms ensuring all are reachable
    for (size_t i = 0; i < numRooms; i++) {
        size_t connections = MIN_ALLOWED_CONNECTIONS + rand() % (MAX_ALLOWED_CONNECTIONS - MIN_ALLOWED_CONNECTIONS + 1);
        for (size_t j = 0; j < connections; j++) {
            size_t target;
            do {
                target = rand() % numRooms;
            } while (target == i || isConnected(rooms[i], rooms[target]));

            addConnection(&rooms[i], &rooms[target]);
        }
    }

    // Ensure all rooms are reachable
    ensureConnectivity(rooms, numRooms);
}

bool isConnected(const room_t room1, const room_t room2) {
    for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
        if (room1.connections[i] != NULL && strcmp(room1.connections[i], room2.name) == 0) {
            return true;
        }
    }
    return false;
}

void addConnection(room_t* room1, room_t* room2) {
    for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
        if (room1->connections[i] == NULL) {
            room1->connections[i] = room2->name;
            break;
        }
    }
    for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
        if (room2->connections[i] == NULL) {
            room2->connections[i] = room1->name;
            break;
        }
    }
}

void ensureConnectivity(room_t rooms[], size_t numRooms) {
    bool visited[MAX_ROOMS] = {false};
    dfs(rooms, 0, visited, numRooms);

    for (size_t i = 0; i < numRooms; i++) {
        if (!visited[i]) {
            // Connect unvisited room to a random visited room
            size_t target;
            do {
                target = rand() % numRooms;
            } while (!visited[target]);

            addConnection(&rooms[i], &rooms[target]);
            dfs(rooms, i, visited, numRooms);
        }
    }
}

void dfs(room_t rooms[], size_t index, bool visited[], size_t numRooms) {
    visited[index] = true;
    for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
        if (rooms[index].connections[i] != NULL) {
            for (size_t j = 0; j < numRooms; j++) {
                if (strcmp(rooms[index].connections[i], rooms[j].name) == 0 && !visited[j]) {
                    dfs(rooms, j, visited, numRooms);
                }
            }
        }
    }
}