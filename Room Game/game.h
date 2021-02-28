#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "room.h"

#include <stdint.h>

char* output(room_t* currentRoom);
char* findRoomsDir();
char* readConnection(FILE* fp, room_t* room, int connection);
char* readName(FILE* fp, room_t* room);
room_t* readFromFile(const char* fileName, room_t* room);
room_t* readFromFiles();
roomType_t readRoomType(FILE* fp, room_t* room);

