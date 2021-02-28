#pragma once
#include "debug.h"
#include <stdbool.h>
#include <stdio.h>
#include "room.h"
//definitions for rooms.c functions
void* shuffle(void* array[], size_t size);
room_t** createRooms(char* roomNames[], size_t Ncount, roomType_t roomTypes[], size_t Tcount);
bool printRooms(FILE* fp, room_t* rooms[], size_t size);
destroyRooms(room_t** rooms);