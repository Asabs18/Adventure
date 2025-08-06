#include "room.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char* ROOM_NAMES[] = {
    "Dungeon", "Library", "Hall", "Garden", "Tower", "Cellar", "Chamber", "Vault", "Atrium", "Gallery"
};

void generate_rooms(Room* rooms, int count) {
    srand((unsigned int)time(NULL));
    int used[10] = {0};
    for (int i = 0; i < count; ++i) {
        int idx;
        do {
            idx = rand() % 10;
        } while (used[idx]);
        used[idx] = 1;
        strncpy(rooms[i].name, ROOM_NAMES[idx], MAX_NAME_LEN);
        rooms[i].connection_count = 0;
        rooms[i].type = MID_ROOM;
        rooms[i].visited = 0;
        rooms[i].has_event = 0;
    }
    // Randomize start and end room
    int start_idx = rand() % count;
    int end_idx;
    do {
        end_idx = rand() % count;
    } while (end_idx == start_idx);
    rooms[start_idx].type = START_ROOM;
    rooms[end_idx].type = END_ROOM;
    connect_rooms(rooms, count);

    // Place a single star (item) in a random room (not start or end)
    int star_idx;
    do {
        star_idx = rand() % count;
    } while (star_idx == start_idx || star_idx == end_idx);
    rooms[star_idx].has_event = 1;
}

void connect_rooms(Room* rooms, int count) {
    for (int i = 0; i < count; ++i) {
        while (rooms[i].connection_count < 3) {
            int j = rand() % count;
            if (j != i && rooms[i].connection_count < MAX_CONNECTIONS && rooms[j].connection_count < MAX_CONNECTIONS) {
                // Check if already connected
                int already = 0;
                for (int k = 0; k < rooms[i].connection_count; ++k) {
                    if (rooms[i].connections[k] == &rooms[j]) {
                        already = 1;
                        break;
                    }
                }
                if (!already) {
                    rooms[i].connections[rooms[i].connection_count++] = &rooms[j];
                    rooms[j].connections[rooms[j].connection_count++] = &rooms[i];
                }
            }
        }
    }
}

Room* find_start_room(Room* rooms, int count) {
    for (int i = 0; i < count; ++i) {
        if (rooms[i].type == START_ROOM) return &rooms[i];
    }
    return NULL;
}
