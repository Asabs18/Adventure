#include "gui.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define MIN_WIDTH 800
#define MIN_HEIGHT 600
#define ROOM_RADIUS 40
// Enhanced dungeon color scheme
#define COLOR_BG CLITERAL(Color){ 16, 16, 24, 255 }            // Darker background for more atmosphere
#define ROOM_COLOR CLITERAL(Color){ 40, 85, 40, 255 }          // Unexplored: muted green
#define ROOM_COLOR_CURRENT CLITERAL(Color){ 180, 45, 45, 255 }  // Current: deeper red
#define ROOM_COLOR_VISITED CLITERAL(Color){ 60, 60, 120, 255 }  // Visited: deep blue
#define ROOM_COLOR_START CLITERAL(Color){ 220, 180, 60, 255 }   // Start: gold
#define ROOM_COLOR_HIGHLIGHT CLITERAL(Color){ 255, 255, 255, 40 } // Highlight overlay
#define ROOM_OUTLINE CLITERAL(Color){ 255, 255, 255, 180 }      // Softer white outline
#define ROOM_SHADOW CLITERAL(Color){ 0, 0, 0, 100 }            // Room shadow

typedef struct {
    float x, y;
} Vec2;

// Draw a room with enhanced styling
static void DrawEnhancedRoom(float x, float y, Color color, const char* name, bool isCurrent) {
    // Draw shadow
    DrawCircle(x + 2, y + 2, ROOM_RADIUS, ROOM_SHADOW);
    
    // Draw main room circle
    DrawCircle(x, y, ROOM_RADIUS, color);
    
    // Draw highlight on top part of room
    DrawCircleGradient(x, y - ROOM_RADIUS/3, ROOM_RADIUS, 
                      ROOM_COLOR_HIGHLIGHT,
                      Fade(ROOM_COLOR_HIGHLIGHT, 0.0f));
    
    // Draw outline
    DrawCircleLines(x, y, ROOM_RADIUS, ROOM_OUTLINE);
    if (isCurrent) {
        DrawCircleLines(x, y, ROOM_RADIUS + 2, Fade(ROOM_OUTLINE, 0.5f));
        DrawCircleLines(x, y, ROOM_RADIUS + 4, Fade(ROOM_OUTLINE, 0.25f));
    }
    
    // Draw room name with shadow
    int fontSize = 16;
    int tw = MeasureText(name, fontSize);
    DrawText(name, x - tw/2 + 1, y - 8 + 1, fontSize, BLACK);  // Shadow
    DrawText(name, x - tw/2, y - 8, fontSize, RAYWHITE);       // Text
}

// Scale positions to fit current screen
static void scale_positions_to_screen(Room* rooms, int count, Vec2* positions, int screenWidth, int screenHeight) {
    // Find bounds of current layout
    float min_x = rooms[0].x, max_x = rooms[0].x;
    float min_y = rooms[0].y, max_y = rooms[0].y;
    
    for (int i = 1; i < count; i++) {
        if (rooms[i].x < min_x) min_x = rooms[i].x;
        if (rooms[i].x > max_x) max_x = rooms[i].x;
        if (rooms[i].y < min_y) min_y = rooms[i].y;
        if (rooms[i].y > max_y) max_y = rooms[i].y;
    }
    
    float layout_width = max_x - min_x;
    float layout_height = max_y - min_y;
    
    // Calculate scaling to fit screen with margins
    float margin = 100.0f;
    float available_width = screenWidth - 2 * margin;
    float available_height = screenHeight - 2 * margin;
    
    float scale_x = (layout_width > 0) ? available_width / layout_width : 1.0f;
    float scale_y = (layout_height > 0) ? available_height / layout_height : 1.0f;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;
    
    // Apply scaling and centering
    for (int i = 0; i < count; i++) {
        positions[i].x = margin + (rooms[i].x - min_x) * scale + (available_width - layout_width * scale) / 2;
        positions[i].y = margin + (rooms[i].y - min_y) * scale + (available_height - layout_height * scale) / 2;
    }
}

static void reset_game(Room* rooms, int max_count, int* count, Room** current, Room** end_room, Room* path[], int* steps, int* won, int* found_star, char* event_msg) {
    // Regenerate rooms
    *count = generate_rooms(rooms, max_count);
    
    // Reset all game state
    *current = NULL;
    *end_room = NULL;
    *steps = 0;
    *won = 0;
    *found_star = 0;
    event_msg[0] = 0;
    
    // Find start room
    for (int i = 0; i < *count; i++) {
        if (rooms[i].type == START_ROOM) {
            *current = &rooms[i];
            break;
        }
    }
    
    // Find end room
    for (int i = 0; i < *count; i++) {
        if (rooms[i].type == END_ROOM) {
            *end_room = &rooms[i];
            break;
        }
    }
    
    // Mark start as visited
    if (*current) (*current)->visited = 1;
}

void play_game_gui(Room* rooms, int initial_count) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);  // Make window resizable
    InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Adventure Game (GUI)");
    SetTargetFPS(60);
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);

    Vec2 positions[12];  // Use fixed size instead of MAX_ROOMS
    int count = initial_count;

    Room* current = NULL;
    for (int i = 0; i < count; ++i) {
        if (rooms[i].type == START_ROOM) {
            current = &rooms[i];
            break;
        }
    }
    Room* end_room = NULL;
    for (int i = 0; i < count; ++i) {
        if (rooms[i].type == END_ROOM) {
            end_room = &rooms[i];
            break;
        }
    }
    Room* path[128];
    int steps = 0;
    int won = 0;
    int found_star = 0;
    char event_msg[128] = "";

    // Mark start as visited
    if (current) current->visited = 1;

    while (!WindowShouldClose()) {
        // Update room layout if window is resized
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        scale_positions_to_screen(rooms, count, positions, screenWidth, screenHeight);

        BeginDrawing();
        ClearBackground(COLOR_BG);

        // Draw connections: only show from current to its connections, and between visited rooms
        for (int i = 0; i < count; ++i) {
            if (&rooms[i] == current || rooms[i].visited) {
                for (int j = 0; j < rooms[i].connection_count; ++j) {
                    Room* conn = rooms[i].connections[j];
                    int idx1 = &rooms[i] - rooms;
                    int idx2 = conn - rooms;
                    if ((conn == current || conn->visited) && (&rooms[i] < conn)) {
                        // Draw connection line with gradient
                        Vector2 start = {positions[idx1].x, positions[idx1].y};
                        Vector2 end = {positions[idx2].x, positions[idx2].y};
                        DrawLineEx(start, end, 2.0f, Fade(ROOM_OUTLINE, 0.3f));
                        // Draw small connecting dots
                        DrawCircle((start.x + end.x)/2, (start.y + end.y)/2, 3, Fade(ROOM_OUTLINE, 0.4f));
                    }
                }
            }
        }

        // Draw rooms: only show current, its connections, and visited
        for (int i = 0; i < count; ++i) {
            int show = 0;
            if (&rooms[i] == current || rooms[i].visited) show = 1;
            else {
                for (int j = 0; j < current->connection_count; ++j) {
                    if (current->connections[j] == &rooms[i]) {
                        show = 1;
                        break;
                    }
                }
            }
            if (show) {
                Color c = ROOM_COLOR;
                if (&rooms[i] == current) c = ROOM_COLOR_CURRENT;
                else if (rooms[i].visited) c = ROOM_COLOR_VISITED;
                else if (rooms[i].type == START_ROOM) c = ROOM_COLOR_START;
                DrawEnhancedRoom(positions[i].x, positions[i].y, c, rooms[i].name, &rooms[i] == current);
            }
        }

        // Draw color key in top right (smaller, far right, no start)
        int key_x = GetScreenWidth() - 10 - 12 - 60; // 10px from right, 12px icon, 60px text
        int key_y = 20, key_h = 12, key_w = 12, key_gap = 12, text_off = 18; // increased gap from 4 to 8
        DrawText("Legend:", key_x, key_y - 12, 10, RAYWHITE);
        
        // Current
        DrawCircle(key_x + key_w/2, key_y + key_h/2, ROOM_RADIUS/4, ROOM_COLOR_CURRENT);
        DrawCircleLines(key_x + key_w/2, key_y + key_h/2, ROOM_RADIUS/4, ROOM_OUTLINE);
        DrawText("Current", key_x + text_off, key_y, 9, RAYWHITE);
        
        // Unexplored (increased y-position multiplier)
        DrawCircle(key_x + key_w/2, key_y + (key_h + key_gap*2), ROOM_RADIUS/4, ROOM_COLOR);
        DrawCircleLines(key_x + key_w/2, key_y + (key_h + key_gap*2), ROOM_RADIUS/4, ROOM_OUTLINE);
        DrawText("Unexplored", key_x + text_off, key_y + (key_h + key_gap*2) - key_h/2, 9, RAYWHITE);
        
        // Visited (increased y-position multiplier)
        DrawCircle(key_x + key_w/2, key_y + (key_h + key_gap*4), ROOM_RADIUS/4, ROOM_COLOR_VISITED);
        DrawCircleLines(key_x + key_w/2, key_y + (key_h + key_gap*4), ROOM_RADIUS/4, ROOM_OUTLINE);
        DrawText("Explored", key_x + text_off, key_y + (key_h + key_gap*4) - key_h/2, 9, RAYWHITE);
        
        // Star (increased y-position multiplier)
        DrawPoly((Vector2){key_x + key_w/2, key_y + (key_h + key_gap*6)}, 5, 5, -PI/2, YELLOW);
        DrawText("Star", key_x + text_off, key_y + (key_h + key_gap*6) - key_h/2, 9, YELLOW);

        // Draw path/steps
        char stepstr[64];
        snprintf(stepstr, sizeof(stepstr), "Steps: %d", steps);
        // Draw steps with dark background panel
        int padding = 15;
        DrawRectangle(padding, padding, MeasureText(stepstr, 20) + 20, 30, Fade(BLACK, 0.5f));
        DrawText(stepstr, padding + 10, padding + 5, 20, RAYWHITE);

        // Show room count
        char roomstr[64];
        snprintf(roomstr, sizeof(roomstr), "Rooms: %d", count);
        DrawRectangle(padding, padding + 40, MeasureText(roomstr, 20) + 20, 30, Fade(BLACK, 0.5f));
        DrawText(roomstr, padding + 10, padding + 45, 20, RAYWHITE);

        // Draw event message
        if (event_msg[0]) {
            // Draw event message with background panel
            int msgWidth = MeasureText(event_msg, 20);
            DrawRectangle(GetScreenWidth()/2 - msgWidth/2 - 20, GetScreenHeight()-70, 
                         msgWidth + 40, 40, Fade(BLACK, 0.7f));
            DrawText(event_msg, GetScreenWidth()/2 - msgWidth/2, GetScreenHeight()-60, 20, YELLOW);
        }

        if (won) {
            // Draw semi-transparent overlay
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
            
            // Draw victory panel
            int panelWidth = 400;
            int panelHeight = 200;
            int panelX = GetScreenWidth()/2 - panelWidth/2;
            int panelY = GetScreenHeight()/2 - panelHeight/2;
            
            // Panel background
            DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(DARKGRAY, 0.9f));
            DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, GOLD);
            
            // Victory text with shadow
            DrawText("YOU WIN!", GetScreenWidth()/2-82, panelY + 30, 40, BLACK);
            DrawText("YOU WIN!", GetScreenWidth()/2-80, panelY + 28, 40, GOLD);
            
            // Star status
            if (found_star) {
                DrawText("Found the Star!", GetScreenWidth()/2-102, panelY + 90, 24, BLACK);
                DrawText("Found the Star!", GetScreenWidth()/2-100, panelY + 88, 24, YELLOW);
            } else {
                DrawText("You missed the star.", GetScreenWidth()/2-102, panelY + 90, 24, BLACK);
                DrawText("You missed the star.", GetScreenWidth()/2-100, panelY + 88, 24, GRAY);
            }
            
            // Instructions
            DrawText("Press R to restart", GetScreenWidth()/2-92, panelY + 130, 20, BLACK);
            DrawText("Press R to restart", GetScreenWidth()/2-90, panelY + 128, 20, GREEN);
            DrawText("Press ESC to exit", GetScreenWidth()/2-82, panelY + 155, 20, BLACK);
            DrawText("Press ESC to exit", GetScreenWidth()/2-80, panelY + 153, 20, RAYWHITE);
        }

        EndDrawing();

        if (won) {
            if (IsKeyPressed(KEY_R)) {
                // Reset the game
                reset_game(rooms, MAX_ROOMS, &count, &current, &end_room, path, &steps, &won, &found_star, event_msg);
            }
            continue;
        }

        // Handle mouse click to move
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < current->connection_count; ++i) {
                Room* next = current->connections[i];
                int idx = next - rooms;
                float dx = mouse.x - positions[idx].x;
                float dy = mouse.y - positions[idx].y;
                // Only allow moving to visible connected rooms
                if (dx*dx + dy*dy < ROOM_RADIUS*ROOM_RADIUS) {
                    current = next;
                    if (!current->visited) {
                        current->visited = 1;
                        if (current->has_event) {
                            snprintf(event_msg, sizeof(event_msg), "Found the Star!");
                            found_star = 1;
                        } else {
                            event_msg[0] = 0;
                        }
                    } else {
                        event_msg[0] = 0;
                    }
                    path[steps++] = current;
                    if (current == end_room) won = 1;
                    break;
                }
            }
        }
    }
    CloseWindow();
}