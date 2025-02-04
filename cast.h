
#ifndef CAST_H
#define CAST_H

#include <raylib.h>
#include <math.h>
#include "config.h"
#include "map.h"

typedef struct Cast {
    float distance;
    Vector2 normal;
    Vector2 final;
    Cell cell;
} Cast;

Cast cast_from(Vector2 position, float angle, Map map) {
    Vector2 direction = (Vector2) {
        cos(angle),
        sin(angle)
    };

    Vector2 delta_dist = (Vector2) {
        fabs(1 / direction.x),
        fabs(1 / direction.y)
    };

    Vector2 step = (Vector2) {
        direction.x < 0 ? -1 : 1,
        direction.y < 0 ? -1 : 1
    };

    Vector2 map_pos = (Vector2) {
        floor(position.x),
        floor(position.y)
    };

    Vector2 side_dist = (Vector2) {
        direction.x < 0 ? (position.x - map_pos.x) * delta_dist.x : (map_pos.x + 1 - position.x) * delta_dist.x,
        direction.y < 0 ? (position.y - map_pos.y) * delta_dist.y : (map_pos.y + 1 - position.y) * delta_dist.y
    };

    int hit = 0;
    int side = 0;

    while (hit == 0 && (map_pos.x >= 0 && map_pos.x < map.width) && (map_pos.y >= 0 && map_pos.y < map.height)) {
        if (side_dist.x < side_dist.y) {
            side_dist.x += delta_dist.x;
            map_pos.x += step.x;
            side = 0;
        } else {
            side_dist.y += delta_dist.y;
            map_pos.y += step.y;
            side = 1;
        }
        if (map_get(map, map_pos.x, map_pos.y) != EMPTY && map_get(map, map_pos.x, map_pos.y) != SPAWN) {
            hit = 1;
        }
    }

    float perp_dist;
    if (side == 0) {
        perp_dist = (map_pos.x - position.x + (1 - step.x) / 2) / direction.x;
    } else {
        perp_dist = (map_pos.y - position.y + (1 - step.y) / 2) / direction.y;
    }

    return (Cast) {
        .distance = perp_dist,
        .normal = side == 0 ? (Vector2) { step.x, 0 } : (Vector2) { 0, step.y },
        .final = (Vector2) {
            position.x + perp_dist * direction.x,
            position.y + perp_dist * direction.y
        },
        .cell = map_get(map, map_pos.x, map_pos.y)
    };
}

#endif
