
#ifndef CELL_H
#define CELL_H

#include <raylib.h>

#include "images.h"

typedef enum Cell {
    SPAWN,
    EMPTY,
    WALL_BRICK,
    WALL_CONCRETE,
    WALL_METAL,
    WALL_CUSTOM
} Cell;

static Texture2D g_textures[4] = { 0 };

void cell_load_textures() {
    g_textures[0] = LoadTextureFromImage((Image) {
        .data = BRICK_DATA,
        .width = BRICK_WIDTH,
        .height = BRICK_HEIGHT,
        .mipmaps = 1,
        .format = BRICK_FORMAT
    });
    g_textures[1] = LoadTextureFromImage((Image) {
        .data = CONCRETE_DATA,
        .width = CONCRETE_WIDTH,
        .height = CONCRETE_HEIGHT,
        .mipmaps = 1,
        .format = CONCRETE_FORMAT
    });
    g_textures[2] = LoadTextureFromImage((Image) {
        .data = METAL_DATA,
        .width = METAL_WIDTH,
        .height = METAL_HEIGHT,
        .mipmaps = 1,
        .format = METAL_FORMAT
    });
    // Attempt to load custom texture
    if      (FileExists("custom.png"))
        g_textures[3] = LoadTexture("custom.png");
    else if (FileExists("custom.jpg"))
        g_textures[3] = LoadTexture("custom.jpg");
    else if (FileExists("custom.bmp"))
        g_textures[3] = LoadTexture("custom.bmp");
    else
        g_textures[3] = LoadTextureFromImage((Image) {
            .data = MISSING_DATA,
            .width = MISSING_WIDTH,
            .height = MISSING_HEIGHT,
            .mipmaps = 1,
            .format = MISSING_FORMAT
        });
}

void cell_unload_textures() {
    for (int i = 0; i < 3; i++) {
        UnloadTexture(g_textures[i]);
    }
}

Texture2D *cell_texture(Cell cell) {
    switch (cell) {
        case SPAWN:
            return NULL;
        case EMPTY:
            return NULL;
        case WALL_BRICK:
            return &(g_textures[0]);
        case WALL_CONCRETE:
            return &(g_textures[1]);
        case WALL_METAL:
            return &(g_textures[2]);
        case WALL_CUSTOM:
            return &(g_textures[3]);
        default:
            return NULL;
    }
}

#endif
