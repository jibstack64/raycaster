
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "config.h"
#include "map.h"

#define BACKGROUND  BLACK

int main(void) {

    // Load map (defaults to 10x10)
    Map map = map_load(MAP_BIN);

    // Initialize window
    const int   screen_length = SCREEN_LENGTH;
    const char  *screen_title = "editor";

    InitWindow(screen_length, screen_length, screen_title);
    SetTargetFPS(60);

    // Calculate cell size based on map size
    int cell_size = screen_length / map.width;

    // Tracks what cell the user has selected
    Vector2 selected = {0, 0};
    Cell paint = WALL_WHITE;

    while (!WindowShouldClose()) {

        // Get user input
        Vector2 mouse_position = GetMousePosition();
        selected = (Vector2) {
            floor(mouse_position.x / cell_size),
            floor(mouse_position.y / cell_size)
        };

        if (IsKeyPressed(KEY_ONE))
            paint = WALL_WHITE;
        if (IsKeyPressed(KEY_TWO))
            paint = WALL_BLUE;
        if (IsKeyPressed(KEY_THREE))    
            paint = WALL_ORANGE;
        if (IsKeyPressed(KEY_FOUR))
            paint = SPAWN;
        if (IsKeyPressed(KEY_FIVE))
            paint = ENEMY;
        if (IsKeyPressed(KEY_ZERO))
            paint = EMPTY;

        // Paint on click
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            map_set(map, selected.x, selected.y, paint);
        }

        BeginDrawing();

            ClearBackground(BACKGROUND);

            // Draw map
            for (int y = 0; y < map.height; y++) {
                for (int x = 0; x < map.width; x++) {
                    Cell cell = map_get(map, x, y);
                    Color colour = cell_colour(cell);

                    // Highlight if selected
                    if (x == selected.x && y == selected.y) {
                        colour = cell_colour(paint);
                    }

                    // Draw spawn, enemies and walls differently
                    if (cell == SPAWN)
                        DrawRectangle(x * cell_size + 5, y * cell_size + 5, cell_size - 10, cell_size - 10, PURPLE);
                    else if (cell == ENEMY)
                        DrawRectangle(x * cell_size + 5, y * cell_size + 5, cell_size - 10, cell_size - 10, RED);
                    else
                        DrawRectangle(x * cell_size, y * cell_size, cell_size, cell_size, colour);

                    DrawRectangleLines((x * cell_size), (y * cell_size), cell_size, cell_size, GREEN);
                }
            }

        EndDrawing();
    }

    CloseWindow();

    // Save map and close
    map_save(MAP_BIN, map);
    map_free(map);

    return 0;
}
