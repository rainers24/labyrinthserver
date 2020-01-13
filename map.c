#include "map.h"
#include "libs.h"

SpawnList *create_spawn_list() {
    SpawnList *list = malloc(sizeof(SpawnList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

Spawn* create_spawn(int x, int y) {
    Spawn* spawn = malloc(sizeof(Spawn));
    spawn->x = x;
    spawn->y = y;
    spawn->free = 1;
    spawn->next = NULL;
    spawn->prev = NULL;

    return spawn;
}

void add_spawn(Map *map, Spawn * spawn) {
    SpawnList *list = map->spawn_list;

    if (list->tail != NULL) {
        list->tail->next = spawn;
        spawn->prev = list->tail;
        list->tail = spawn;
    } else {
        list->head = spawn;
        list->tail = spawn;
    }

    list->size++;
}

char **read_map(Map* map, int width, int height) {
    char **field = (char **) calloc(width, sizeof(char *));

    int k;
    for (k = 0; k < width; k++) {
        field[k] = (char *) calloc(height, sizeof(char));
        bzero(field[k], 31);
    }

    FILE *f = fopen("map.txt", "r");

    int jj, ii;
    for (jj = 0; jj < height; jj++) {
        for (ii = 0; ii < width; ii++) {
            char c = fgetc(f);
            if (c == 'X') {
                add_spawn(map, create_spawn(ii, jj));
                field[ii][jj] = ' ';
                continue;
            }
            field[ii][jj] = c;
        }
    }

    fclose(f);


//    Spawn *sp = map->spawn_list->head; to see coordinates of the spawn points and if they match with the X char from the map
//    while (sp) {
//        printf("%d;%d\n", sp->x, sp->y);
//        sp = sp->next;
//    }

    return field;
}

Map *create_map(int width, int height) {
    Map *map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;
    map->spawn_list = create_spawn_list();
    map->map = read_map(map, width, height);

    return map;
}

int is_path(Map *map, int x, int y) {
    return map->map[x][y] == ' ';
}

void print_map(Map *map) {
    int jj, ii;
    for (jj = 0; jj < map->height; jj++) {
        for (ii = 0; ii < map->width; ii++) {
            printf("%c", map->map[ii][jj]);
        }
    }
}

Spawn *get_free_spawn_point(Map* map) {
    Spawn *spawn = map->spawn_list->head;
    while (spawn != NULL) {
        if (spawn->free) {
            return spawn;
        }
        spawn = spawn->next;
    }
}