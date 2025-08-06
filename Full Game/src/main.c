#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"
#include "game.h"
#include "gui.h"

#define ROOM_COUNT 7

int main() {
    Room rooms[ROOM_COUNT];
    
    generate_rooms(rooms, ROOM_COUNT);
    printf("Rooms generated!\n");

    play_game_gui(rooms, ROOM_COUNT);

    return 0;
}
