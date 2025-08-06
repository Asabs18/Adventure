#include "game.h"
#include <stdio.h>
#include <string.h>

void play_game(Room* rooms, int count) {
    Room* current = find_start_room(rooms, count);
    Room* path[128];
    int steps = 0;

    printf("Welcome to the Adventure!\n");

    while (current->type != END_ROOM) {
        printf("\nCURRENT LOCATION: %s\n", current->name);
        printf("POSSIBLE CONNECTIONS:");
        for (int i = 0; i < current->connection_count; ++i) {
            printf(" %s", current->connections[i]->name);
            if (i < current->connection_count - 1) printf(",");
        }
        printf(".\nWHERE TO? > ");

        char input[MAX_NAME_LEN];
        fgets(input, MAX_NAME_LEN, stdin);
        input[strcspn(input, "\n")] = 0;

        int found = 0;
        for (int i = 0; i < current->connection_count; ++i) {
            if (strcmp(input, current->connections[i]->name) == 0) {
                current = current->connections[i];
                path[steps++] = current;
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        }
    }

    printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
    for (int i = 0; i < steps; ++i) {
        printf("%s\n", path[i]->name);
    }
}
