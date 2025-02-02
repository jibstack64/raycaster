
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "config.h"
#include "map.h"
#include "cast.h"

#include "images/background.h"
#include "images/angry.h"

int main(void) {

    // Load map (defaults to 10x10)
    Map map = map_load(MAP_BIN);

    const int   screen_length = SCREEN_LENGTH;
    const char  *screen_title = "raycaster";

    InitWindow(screen_length, screen_length, screen_title);
    SetTargetFPS(60);

    // Load images
    Image background_image = (Image) {
        .data = BACKGROUND_DATA,
        .width = BACKGROUND_WIDTH,
        .height = BACKGROUND_HEIGHT,
        .mipmaps = 1,
        .format = BACKGROUND_FORMAT
    };
    Texture background = LoadTextureFromImage(background_image);
    Image angry_image = (Image) {
        .data = ANGRY_DATA,
        .width = ANGRY_WIDTH,
        .height = ANGRY_HEIGHT,
        .mipmaps = 1,
        .format = ANGRY_FORMAT
    };
    Texture angry = LoadTextureFromImage(angry_image);

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
            player_position.x += move_speed * cos(player_looking) * frame_time;
            player_position.y += move_speed * sin(player_looking) * frame_time;
            bob_tracker += bob_amount * frame_time;
        }
        if (IsKeyDown(KEY_S)) {
            player_position.x -= move_speed * cos(player_looking) * frame_time;
            player_position.y -= move_speed * sin(player_looking) * frame_time;
            bob_tracker += bob_amount * frame_time;
        }
        if (IsKeyDown(KEY_A)) {
            player_looking -= look_speed * frame_time;
        }
        if (IsKeyDown(KEY_D)) {
            player_looking += look_speed * frame_time;
        }

        BeginDrawing();

            ClearBackground(BLACK);
            
            // WOOOH!
            float bobbing_offset = sin(bob_tracker) * 5;

            // Draw floor and ceiling
            DrawTexture(background, 0, 0, WHITE);
            DrawRectangle(0, screen_length / 2, screen_length, screen_length / 2, FLOOR_COLOUR);

            // Send casts for each column
            Vector2 last_enemy_pos = {-2, -2};
            for (int x = 0; x < screen_length; x++) {

                float angle = x * (PI / 180) * FOV / screen_length + (player_looking - PI / 6);
                Cast cast = cast_from(player_position, angle, map);
                if (cast.distance != 0 && cast.cell != EMPTY) {
                    DrawLine(x, screen_length / 2 - screen_length / cast.distance / 2 + bobbing_offset, x, screen_length / 2 + screen_length / cast.distance / 2 + bobbing_offset, cell_colour(cast.cell));
                }

                // Draw enemy (if any)
                if (cast.enemy.distance != -1 && cast.enemy.position.x != last_enemy_pos.x && cast.enemy.position.y != last_enemy_pos.y) {
                    Vector2 enemy_pos = (Vector2) {
                        x,
                        screen_length / 2 - screen_length / cast.enemy.distance / 2 + bobbing_offset
                    };
                    float enemy_scale = 1 / cast.enemy.distance * 3.5;
                    if (last_enemy_pos.x != -1) {
                        DrawTextureEx(angry, enemy_pos, 0, enemy_scale, WHITE);
                    }
                    last_enemy_pos = cast.enemy.position;
                }

            }

        EndDrawing();
    }

    // Clean up
    UnloadTexture(background);
    UnloadTexture(angry);

    CloseWindow();

    return 0;
}
