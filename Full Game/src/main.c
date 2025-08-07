#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"
#include "game.h"
#include "gui.h"

int main() {
    Room rooms[MAX_ROOMS];
    
    int room_count = generate_rooms(rooms, MAX_ROOMS);
    printf("Generated %d rooms!\n", room_count);

    play_game_gui(rooms, room_count);

    return 0;
}