#ifndef LABYRINTH_SERVER_GAME_H
#define LABYRINTH_SERVER_GAME_H

#include <pthread.h>
#include "signal.h"

#include "libs.h"
#include "net.h"
#include "map.h"

enum PacketType {
    PT_JOIN_GAME = 0,
    PT_MOVE = 1,
    PT_LOBBY_INFO = 2,
    PT_GAME_IN_PROGESS = 3,
    PT_USERNAME_TAKEN = 4,
    PT_GAME_START = 5,
    PT_MAP_ROW = 6,
    PT_GAME_UPDATE = 7,
    PT_PLAYER_DEAD = 8,
    PT_GAME_END = 9,
};

enum GameState {
    GS_LOBBY = 0,
    GS_GAME = 1,
};

enum PlayerState {
    PS_DEAD = 0,
    PS_ALIVE = 1,
};

enum Direction {
    DIR_UP = 'U',
    DIR_DOWN = 'D',
    DIR_LEFT = 'L',
    DIR_RIGHT = 'R',
};

typedef struct Player {
    int id;
    int state;
    char *name;
    char letter;

    int score;
    int x;
    int y;
    struct MoveRequest *move_request;

    time_t last_ping;

    Socket *socket;

    struct Player *next;
    struct Player *prev;
} Player;

typedef struct {
    Player *head;
    Player *tail;
    int size;
} PlayerList;

typedef struct Fruit {
    int x;
    int y;

    struct Fruit *next;
    struct Fruit *prev;
} Fruit;

typedef struct {
    Fruit *head;
    Fruit *tail;
    int size;
} FruitsList;

typedef struct MoveRequest {
    Player *player;
    enum Direction direction;

    struct MoveRequest *next;
    struct MoveRequest *prev;
} MoveRequest;

typedef struct {
    MoveRequest *head;
    MoveRequest *tail;
    int size;
} MoveRequestQueue;

typedef struct {
    int player_id_seq;
    int state;
    Map* map;
    PlayerList *players;
    FruitsList *fruits;
    MoveRequestQueue *queue;
} Game;

Game *game;

PlayerList *create_playerlist();

Player *create_player(int, char *, Socket *);

Player *add_player(Game *, Player *);

void remove_player(Game *, Player *);

MoveRequestQueue *create_move_request_queue();

FruitsList *create_fruits_list();

Game *create_game();

void *connection_handler(void *);

Player *handle_join_request(Socket *socket, Player *player, char *payload);

void handle_move_request(Player *, char *);

void send_loby_info(Game *);

void print_lobby_info(Game *);

void game_tick(Game *);

void handle_player_disconnect_packet(Socket *socket, Player *player, char *payload);

#endif //LABYRINTH_SERVER_GAME_H
