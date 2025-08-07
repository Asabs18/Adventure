#ifndef ROOM_H
#define ROOM_H

#define MAX_CONNECTIONS 6
#define MAX_NAME_LEN 32
#define MIN_ROOMS 5
#define MAX_ROOMS 12

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
    float x, y; // Position for GUI layout
} Room;

int generate_rooms(Room* rooms, int max_count);
void connect_rooms(Room* rooms, int count);
Room* find_start_room(Room* rooms, int count);
int bfs_distance(Room* start, Room* end, Room* rooms, int count);
int is_graph_connected(Room* rooms, int count);

#endif