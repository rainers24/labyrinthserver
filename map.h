#ifndef LABYRINTH_SERVER_MAP_H
#define LABYRINTH_SERVER_MAP_H

typedef struct Spawn{
    int x;
    int y;
    int free;

    struct Spawn* next;
    struct Spawn* prev;
} Spawn;


typedef struct {
    Spawn *head;
    Spawn *tail;
    int size;
} SpawnList;

typedef struct Map{
    char **map;
    int width;
    int height;
    SpawnList *spawn_list;
} Map;

Map *create_map(int, int);

char **read_map(Map*, int, int);

int is_path(Map*, int, int);

void print_map(Map*);

Spawn *get_free_spawn_point(Map*);

#endif //LABYRINTH_SERVER_MAP_H
