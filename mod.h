
#ifndef MOD_H
#define MOD_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include "cell.h"

// Lua <-> C data structures
typedef struct ModUpdateData {
    bool    *Running;
    float   DeltaTime,
            *PlayerX,
            *PlayerY,
            *PlayerLooking;
    int     MapWidth,
            MapHeight;
    Cell    *MapData;
} ModUpdateData;

static int lua_update_function_ref = LUA_REFNIL;

int _mod_bind_update_function(lua_State *L) {
    if (!lua_isfunction(L, 1))
        return luaL_error(L, "Expected a function.");
    luaL_unref(L, LUA_REGISTRYINDEX, lua_update_function_ref);
    lua_update_function_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return 0;
}

// Initialise lua
lua_State *mod_init(const char *script) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "BindUpdateFunction", _mod_bind_update_function);
    if (luaL_dostring(L, script) != LUA_OK) {
        printf("Error loading Lua script: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    return L;
}

// From file
lua_State *mod_init_file(const char *filename) {
    
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    
    lua_State *L = mod_init(buffer);
    free(buffer);

    return L;
}

// Runs bound func
void mod_update(lua_State *L, ModUpdateData data) {
    if (lua_update_function_ref == LUA_REFNIL) return;

    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_update_function_ref);

    lua_newtable(L);
    
    // Running
    lua_pushstring(L, "Running");
    lua_pushboolean(L, *data.Running);
    lua_settable(L, -3);

    // Delta time
    lua_pushstring(L, "DeltaTime");
    lua_pushnumber(L, data.DeltaTime);
    lua_settable(L, -3);

    // Player X
    lua_pushstring(L, "PlayerX");
    lua_pushnumber(L, *data.PlayerX);
    lua_settable(L, -3);

    // Player Y
    lua_pushstring(L, "PlayerY");
    lua_pushnumber(L, *data.PlayerY);
    lua_settable(L, -3);

    // Player looking
    lua_pushstring(L, "PlayerLooking");
    lua_pushnumber(L, *data.PlayerLooking);
    lua_settable(L, -3);

    // Map width
    lua_pushstring(L, "MapWidth");
    lua_pushinteger(L, data.MapWidth);
    lua_settable(L, -3);

    // Map height
    lua_pushstring(L, "MapHeight");
    lua_pushinteger(L, data.MapHeight);
    lua_settable(L, -3);

    // Map data
    int map_size = data.MapWidth * data.MapHeight;
    lua_pushstring(L, "MapData");
    lua_newtable(L);
    for (int i = 0; i < map_size; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushinteger(L, (int)data.MapData[i]);
        lua_settable(L, -3);
    }
    lua_settable(L, -3);

    // Call the func with the the table
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }

    // After the Lua function, get the table back from the Lua stack

    // Running
    lua_pushstring(L, "Running");
    lua_gettable(L, -2);
    if (lua_isboolean(L, -1))
        *data.Running = (bool)lua_toboolean(L, -1);
    lua_pop(L, 1);
    
    // PlayerX
    lua_pushstring(L, "PlayerX");
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1))
        *data.PlayerX = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // PlayerY
    lua_pushstring(L, "PlayerY");
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1))
        *data.PlayerY = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // PlayerLooking
    lua_pushstring(L, "PlayerLooking");
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1))
        *data.PlayerLooking = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Map data
    lua_pushstring(L, "MapData");
    lua_gettable(L, -2);
    if (lua_istable(L, -1)) {
        for (int i = 0; i < map_size; i++) {
            lua_pushinteger(L, i + 1);
            lua_gettable(L, -2);
            if (lua_isinteger(L, -1))
                data.MapData[i] = (Cell)lua_tointeger(L, -1);
            else
                printf("NOT INTeGER!\n");
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    lua_pop(L, 1); // Remove the table from the stack
}

void mod_close(lua_State *L) {
    if (L) {
        lua_close(L);
        L = NULL;
    }
}

#endif
