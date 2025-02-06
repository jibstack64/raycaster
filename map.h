
#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include "cell.h"

typedef struct Map {
    int width, height;
    Cell *content;
} Map;

Cell map_get(Map map, int x, int y) {
    if (x < 0 || x >= map.width || y < 0 || y >= map.height) {
        return EMPTY;
    }
    return map.content[y * map.width + x];
}

void map_set(Map map, int x, int y, Cell cell) {
    if (x < 0 || x >= map.width || y < 0 || y >= map.height) {
        return;
    }
    map.content[y * map.width + x] = cell;
}

Vector2 map_spawn(Map map) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            if (map_get(map, x, y) == SPAWN) {
                return (Vector2){x, y};
            }
        }
    }
    return (Vector2){0, 0};
}

Map map_new(int width, int height) {
    Map map = { 0 };

    // This is so `map_free` works
    char *buffer = (char *)malloc(sizeof(int) * 2 + width * height * sizeof(Cell));
    ((int *)buffer)[0] = width;
    ((int *)buffer)[1] = height;

    map.width = width;
    map.height = height;
    map.content = (Cell *)(buffer + (sizeof(int) * 2));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map_set(map, x, y, EMPTY);
        }
    }

    return map;
}

void map_free(Map map) {
    free((int *)(map.content) - 2);
}

Map map_load(const char *filename) {
    Map map = { 0 };

    // Get `filename`'s contents
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return map_new(16, 16);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Read contents
    char *buffer = (char *)malloc(size);
    fread(buffer, sizeof(char), size, file);

    map.width = *(int *)(buffer);
    map.height = *(int *)(buffer + sizeof(int));
    map.content = (Cell *)(buffer + (sizeof(int) * 2));

    // Close file
    fclose(file);

    return map;
}

Map map_resize(Map map, int amount) {
    int new_width = map.width + amount;
    int new_height = map.height + amount;

    if (new_width <= 0 || new_height <= 0) {
        return map;
    }

    Map new_map = map_new(new_width, new_height);

    for (int y = 0; y < (map.height < new_height ? map.height : new_height); y++) {
        for (int x = 0; x < (map.width < new_width ? map.width : new_width); x++) {
            map_set(new_map, x, y, map_get(map, x, y));
        }
    }

    map_free(map);

    return new_map;
}

bool map_save(const char *filename, Map map) {

    // Open and save
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return false;
    }

    fwrite(&map.width, sizeof(int), 1, file);
    fwrite(&map.height, sizeof(int), 1, file);
    fwrite(map.content, sizeof(Cell), map.width * map.height, file);

    fclose(file);

    return true;
}

#endif
