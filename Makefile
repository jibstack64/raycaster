CC = gcc
CFLAGS = -O2 -Wall -std=c99
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_EDITOR = editor.c
SRC_RAYCASTER = raycaster.c

OBJ_EDITOR = $(SRC_EDITOR:.c=.o)
OBJ_RAYCASTER = $(SRC_RAYCASTER:.c=.o)

EXEC_EDITOR = editor
EXEC_RAYCASTER = raycaster

all: $(EXEC_EDITOR) $(EXEC_RAYCASTER)

$(EXEC_EDITOR): $(OBJ_EDITOR)
	$(CC) $(OBJ_EDITOR) -o $@ $(LDFLAGS)

$(EXEC_RAYCASTER): $(OBJ_RAYCASTER)
	$(CC) $(OBJ_RAYCASTER) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_EDITOR) $(OBJ_RAYCASTER) $(EXEC_EDITOR) $(EXEC_RAYCASTER)
