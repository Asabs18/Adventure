#ifndef ROOM_H
#define ROOM_H

#define MAX_CONNECTIONS 6
#define MAX_NAME_LEN 32

typedef enum {
    START_ROOM,
    MID_ROOM,
    END_ROOM
} RoomType;

typedef struct Room {
    char name[MAX_NAME_LEN];
    RoomType type;
    struct Room* connections[MAX_CONNECTIONS];
    int connection_count;
    int visited; // 0 = not visited, 1 = visited
    int has_event; // 0 = none, 1 = event present
} Room;

void generate_rooms(Room* rooms, int count);
void connect_rooms(Room* rooms, int count);
Room* find_start_room(Room* rooms, int count);

#endif
