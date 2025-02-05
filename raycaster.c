
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "config.h"
#include "map.h"
#include "cast.h"

#include "images/background.h"

int main(void) {

    // Load map (defaults to 10x10)
    Map map = map_load(MAP_BIN);

    const int   screen_length = SCREEN_LENGTH;
    const char  *screen_title = "raycaster";

    InitWindow(screen_length, screen_length, screen_title);
    SetTargetFPS(60);

    // Load textures
    cell_load_textures();

    // Load images
    Image background_image = (Image) {
        .data = BACKGROUND_DATA,
        .width = BACKGROUND_WIDTH,
        .height = BACKGROUND_HEIGHT,
        .mipmaps = 1,
        .format = BACKGROUND_FORMAT
    };
    Texture background = LoadTextureFromImage(background_image);

    // Track player position, looking, etc.
    Vector2 player_position = map_spawn(map);
    float   player_looking = 0,
            move_speed = 1,
            look_speed = 1.5,
            bob_tracker = 0,
            bob_amount = 10;

    while (!WindowShouldClose()) {

        // Basic movement
        float frame_time = GetFrameTime();
        if (IsKeyDown(KEY_W)) {
            float move_to_x = player_position.x + move_speed * cos(player_looking) * frame_time; 
            float move_to_y = player_position.y + move_speed * sin(player_looking) * frame_time;
            int floored_x = floor(move_to_x), floored_y = floor(move_to_y);
            if (map_get(map, floored_x, floored_y) == EMPTY || map_get(map, floored_x, floored_y) == SPAWN) {
                player_position.x = move_to_x;
                player_position.y = move_to_y;
            }
            bob_tracker += bob_amount * frame_time;
        }
        if (IsKeyDown(KEY_S)) {
            float move_to_x = player_position.x - move_speed * cos(player_looking) * frame_time;
            float move_to_y = player_position.y - move_speed * sin(player_looking) * frame_time;
            int floored_x = floor(move_to_x), floored_y = floor(move_to_y);
            if (map_get(map, floored_x, floored_y) == EMPTY || map_get(map, floored_x, floored_y) == SPAWN) {
                player_position.x = move_to_x;
                player_position.y = move_to_y;
            }
            bob_tracker += bob_amount * frame_time;
        }
        if (IsKeyDown(KEY_A)) {
            player_looking -= look_speed * frame_time;
        }
        if (IsKeyDown(KEY_D)) {
            player_looking += look_speed * frame_time;
        }
        player_looking += GetMouseDelta().x * look_speed * frame_time;

        BeginDrawing();

            ClearBackground(BLACK);
            
            // WOOOH!
            float bobbing_offset = sin(bob_tracker) * 5;

            // Draw floor and ceiling
            DrawTexture(background, 0, 0, WHITE);
            DrawRectangle(0, screen_length / 2, screen_length, screen_length / 2, FLOOR_COLOUR);

            // Send casts for each column
            for (int x = 0; x < screen_length; x++) {

                float angle = x * (PI / 180) * FOV / screen_length + (player_looking - PI / 6);
                Cast cast = cast_from(player_position, angle, map);
                
                // Get the texture, if not any, skip
                Texture2D *texture = cell_texture(cast.cell);
                if (!texture) {
                    continue;
                }
                
                int y_pos = screen_length / 2 - screen_length / cast.distance / 2 + bobbing_offset;
                int wall_height = screen_length / cast.distance;

                // Get the position in the texture to draw
                float wall_hit_pos = (cast.normal.x != 0) ? cast.final.y : cast.final.x;
                int texture_x = (int)(wall_hit_pos * texture->width) % texture->width;
                if (texture_x < 0) texture_x += texture->width;

                Rectangle source_rect = {
                    texture_x,
                    0,
                    1,
                    texture->height
                };

                Rectangle to_rect = {
                    x,
                    y_pos,
                    1,
                    wall_height
                };

                if (cast.distance != 0)
                    DrawTexturePro(*texture, source_rect, to_rect, (Vector2) {0, 0}, 0, WHITE);
            }

        EndDrawing();
    }

    // Clean up
    cell_unload_textures();
    UnloadTexture(background);

    CloseWindow();

    return 0;
}
