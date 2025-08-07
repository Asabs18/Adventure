#include "room.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static const char* ROOM_NAMES[] = {
    "Dungeon", "Library", "Hall", "Garden", "Tower", "Cellar", "Chamber", "Vault", 
    "Atrium", "Gallery", "Crypt", "Study", "Kitchen", "Armory", "Chapel", "Throne",
    "Forge", "Observatory", "Sanctum", "Archive"
};
#define ROOM_NAME_COUNT 20

// BFS to find shortest distance between two rooms
int bfs_distance(Room* start, Room* end, Room* rooms, int count) {
    if (start == end) return 0;
    
    // Reset visited flags for BFS
    for (int i = 0; i < count; i++) {
        rooms[i].visited = 0;
    }
    
    static Room* queue[12];  // Use static arrays with fixed size
    static int distances[12];
    int front = 0, rear = 0;
    
    queue[rear] = start;
    distances[rear] = 0;
    start->visited = 1;
    rear++;
    
    while (front < rear) {
        Room* current = queue[front];
        int current_dist = distances[front];
        front++;
        
        for (int i = 0; i < current->connection_count; i++) {
            Room* neighbor = current->connections[i];
            if (!neighbor->visited) {
                neighbor->visited = 1;
                queue[rear] = neighbor;
                distances[rear] = current_dist + 1;
                rear++;
                
                if (neighbor == end) {
                    return current_dist + 1;
                }
            }
        }
    }
    
    return -1; // Not reachable
}

// Check if all rooms are connected
int is_graph_connected(Room* rooms, int count) {
    if (count <= 1) return 1;
    
    // Reset visited flags
    for (int i = 0; i < count; i++) {
        rooms[i].visited = 0;
    }
    
    static Room* queue[12];  // Use static array with fixed size
    int front = 0, rear = 0;
    
    queue[rear++] = &rooms[0];
    rooms[0].visited = 1;
    int visited_count = 1;
    
    while (front < rear) {
        Room* current = queue[front++];
        
        for (int i = 0; i < current->connection_count; i++) {
            Room* neighbor = current->connections[i];
            if (!neighbor->visited) {
                neighbor->visited = 1;
                queue[rear++] = neighbor;
                visited_count++;
            }
        }
    }
    
    return visited_count == count;
}

// Generate random layout positions
static void generate_layout(Room* rooms, int count) {
    // Use a simple random scatter approach with some spacing
    const float MIN_DISTANCE = 100.0f;
    const float MAP_WIDTH = 800.0f;
    const float MAP_HEIGHT = 600.0f;
    const float MARGIN = 80.0f;
    
    for (int i = 0; i < count; i++) {
        int attempts = 0;
        do {
            rooms[i].x = MARGIN + (float)rand() / RAND_MAX * (MAP_WIDTH - 2*MARGIN);
            rooms[i].y = MARGIN + (float)rand() / RAND_MAX * (MAP_HEIGHT - 2*MARGIN);
            
            // Check distance from other rooms
            int too_close = 0;
            for (int j = 0; j < i; j++) {
                float dx = rooms[i].x - rooms[j].x;
                float dy = rooms[i].y - rooms[j].y;
                float dist = sqrtf(dx*dx + dy*dy);
                if (dist < MIN_DISTANCE) {
                    too_close = 1;
                    break;
                }
            }
            
            if (!too_close) break;
            attempts++;
        } while (attempts < 100);
    }
}

int generate_rooms(Room* rooms, int max_count) {
    srand((unsigned int)time(NULL));
    
    // Generate random number of rooms between MIN_ROOMS and min(MAX_ROOMS, max_count)
    int count = MIN_ROOMS + rand() % (((max_count < MAX_ROOMS) ? max_count : MAX_ROOMS) - MIN_ROOMS + 1);
    
    // Select random room names
    static int used[20];  // Use literal instead of ROOM_NAME_COUNT
    memset(used, 0, sizeof(used));  // Clear the array
    for (int i = 0; i < count; i++) {
        int idx;
        do {
            idx = rand() % ROOM_NAME_COUNT;
        } while (used[idx]);
        used[idx] = 1;
        
        strncpy(rooms[i].name, ROOM_NAMES[idx], MAX_NAME_LEN);
        rooms[i].name[MAX_NAME_LEN-1] = '\0';
        rooms[i].connection_count = 0;
        rooms[i].type = MID_ROOM;
        rooms[i].visited = 0;
        rooms[i].has_event = 0;
    }
    
    // Generate layout positions
    generate_layout(rooms, count);
    
    // Try multiple times to generate a valid graph
    int max_attempts = 50;
    for (int attempt = 0; attempt < max_attempts; attempt++) {
        // Reset connections
        for (int i = 0; i < count; i++) {
            rooms[i].connection_count = 0;
            rooms[i].type = MID_ROOM;
        }
        
        // Connect rooms
        connect_rooms(rooms, count);
        
        // Check if graph is connected
        if (!is_graph_connected(rooms, count)) {
            continue; // Try again
        }
        
        // Assign start and end rooms and check distance
        int valid_assignment = 0;
        int assignment_attempts = 20;
        
        for (int assign_attempt = 0; assign_attempt < assignment_attempts; assign_attempt++) {
            // Reset room types
            for (int i = 0; i < count; i++) {
                rooms[i].type = MID_ROOM;
            }
            
            int start_idx = rand() % count;
            int end_idx;
            do {
                end_idx = rand() % count;
            } while (end_idx == start_idx);
            
            rooms[start_idx].type = START_ROOM;
            rooms[end_idx].type = END_ROOM;
            
            // Check if distance is at least 2
            int distance = bfs_distance(&rooms[start_idx], &rooms[end_idx], rooms, count);
            if (distance >= 2) {
                valid_assignment = 1;
                break;
            }
        }
        
        if (valid_assignment) {
            // Place a star in a random room (not start or end)
            int star_attempts = 10;
            for (int star_attempt = 0; star_attempt < star_attempts; star_attempt++) {
                int star_idx = rand() % count;
                if (rooms[star_idx].type == MID_ROOM) {
                    rooms[star_idx].has_event = 1;
                    break;
                }
            }
            
            // Reset visited flags
            for (int i = 0; i < count; i++) {
                rooms[i].visited = 0;
            }
            
            return count;
        }
    }
    
    // Fallback: create a simple valid configuration
    count = MIN_ROOMS;
    for (int i = 0; i < count; i++) {
        rooms[i].connection_count = 0;
        rooms[i].type = MID_ROOM;
        rooms[i].visited = 0;
    }
    
    // Create a simple connected graph (star topology)
    for (int i = 1; i < count; i++) {
        rooms[0].connections[rooms[0].connection_count++] = &rooms[i];
        rooms[i].connections[rooms[i].connection_count++] = &rooms[0];
    }
    
    rooms[0].type = START_ROOM;
    rooms[count-1].type = END_ROOM;
    if (count > 2) rooms[1].has_event = 1;
    
    return count;
}

void connect_rooms(Room* rooms, int count) {
    // Each room gets 2-4 connections
    for (int i = 0; i < count; i++) {
        int target_connections = 2 + rand() % 3; // 2-4 connections
        
        while (rooms[i].connection_count < target_connections) {
            int j = rand() % count;
            if (j != i && rooms[j].connection_count < MAX_CONNECTIONS) {
                // Check if already connected
                int already_connected = 0;
                for (int k = 0; k < rooms[i].connection_count; k++) {
                    if (rooms[i].connections[k] == &rooms[j]) {
                        already_connected = 1;
                        break;
                    }
                }
                
                if (!already_connected) {
                    rooms[i].connections[rooms[i].connection_count++] = &rooms[j];
                    rooms[j].connections[rooms[j].connection_count++] = &rooms[i];
                }
            }
        }
    }
}

Room* find_start_room(Room* rooms, int count) {
    for (int i = 0; i < count; i++) {
        if (rooms[i].type == START_ROOM) return &rooms[i];
    }
    return NULL;
}