#include  "game.h"

Game *game;

/**
 *
 * @return
 */
Game *create_game() { // the game is created here, with lobby, players, map, queue and fruits
    game = malloc(sizeof(Game));

    game->player_id_seq = 0;
    game->state = GS_LOBBY;
    game->players = create_playerlist();
    game->map = create_map(122, 31);
    game->queue = create_move_request_queue();
    game->fruits = create_fruits_list();

    return game;
}


/**
 *
 * @return
 */
PlayerList *create_playerlist() { // players are stored in a list
    PlayerList *list = malloc(sizeof(PlayerList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


/**
 *
 * @param id
 * @param name
 * @param socket
 * @return
 */
Player *create_player(int id, char *name, Socket *socket) { // for the players there will be a name which is based on ID , if it is 65 => A
    //score in the start is 1 not to die in the start
    Player *player = malloc(sizeof(Player));
    player->id = id;
    player->letter = id + 65;
    player->name = name;
    player->socket = socket;
    player->score = 1;
    player->state = PS_ALIVE;
    player->last_ping = time(NULL);

    player->next = NULL;
    player->prev = NULL;

    return player;
}

void attach_spawn_point(Game *game, Player *player) {// for each player there will be a separate spawn point, so that they do not spawn in one place
    Spawn *spawn = get_free_spawn_point(game->map);
    player->x = spawn->x;
    player->y = spawn->y;
    spawn->free = 0;
}


/**
 *
 * @param game
 * @param player
 * @return
 */
Player *add_player(Game *game, Player *player) { // player is added to the player list and after that it gets a spawn point coordinates
    PlayerList *list = game->players;

    if (list->tail != NULL) {
        list->tail->next = player;
        player->prev = list->tail;
        list->tail = player;
    } else {
        list->head = player;
        list->tail = player;
    }

    list->size++;

    attach_spawn_point(game, player);

    return player;
}


/**
 *
 * @param game
 * @param player
 */
void remove_player(Game *game, Player *player) { // player is removed
    PlayerList *list = game->players;

    if (player->prev) {
        player->prev->next = player->next;
    } else {
        list->head = player->next;
    }

    if (player->next) {
        player->next->prev = player->prev;
    } else {
        list->tail = player->prev;
    }

    list->size--;

    free(player->name);
    free(player);
};

/**
 *
 * @return
 */
MoveRequestQueue *create_move_request_queue() {
    MoveRequestQueue *queue = malloc(sizeof(MoveRequestQueue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

MoveRequest *create_move_request(enum Direction direction) {

}

void add_move_request(Game *game, Player *player, MoveRequest *request) {
    MoveRequestQueue *queue = game->queue;

}

void clear_requests(Game *game) { // clear the moverequests

}

FruitsList *create_fruits_list() {

}

Fruit* create_fruit(int x, int y) {

}

void add_fruit(Game *game, Fruit * fruit) {
    FruitsList *list = game->fruits;

}

/**
 *
 * @param socket
 * @return
 */
void *connection_handler(void *socket) {
    //Get the socket descriptor
    Socket *sock = (Socket *) socket;
    int socket_d = *sock->socket;

    ssize_t read_size;
    char *payload = malloc(200);

    Player *player = NULL;

    while ((read_size = recv(socket_d, payload, 200, 0)) > 0) {
        int packet_code = payload[0] - '0'; // converts packet code char to int (works for 0-9)
        switch (packet_code) {
            case PT_JOIN_GAME: {
                if (player) {
                    printf("cannot send join packet multiple times\n");
                    break;
                }
                player = handle_join_request(sock, player, payload);
                break;
            }
            case PT_MOVE: {
                if (player) {
                    handle_move_request(player, payload);
                }
                break;
            }


            default: {
                printf("unknown packet\n");
                break;
            }
        }
        printf("debug: %d %s\n", player ? player->id : -1, payload);

        bzero(payload, 200);
    }

    if (read_size == 0) {
        if (player) {
            remove_player(game, player);
        }
        send_loby_info(game);
        print_lobby_info(game);
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    delete_socket(sock);

    return 0;
}


/**
 *
 * @param payload
 * @return
 */
char *parse_join_packet(const char *payload) {
    char *name = malloc(16);
    bzero(name, 16);

    int i;
    for (i = 0; i < 15; i++) {
        char character = payload[i + 1];
        if (character < 32 || character > 126) {
            name[i] = 0;
            break;
        }
        name[i] = character;
    }

    return name;
}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
Player *handle_join_request(Socket *socket, Player *player, char *payload) {
    char *name = parse_join_packet(payload);
    if (name != NULL) {
        player = create_player(game->player_id_seq, name, socket);
        game->player_id_seq++;
        add_player(game, player);
    }
    send_loby_info(game);
    print_lobby_info(game);
    return player;
}


void handle_move_request(Player *player, char *payload) {

}


/**
 *
 * @param game
 */
void send_loby_info(Game *game) { // client side will receive the info about the lobby, what kind of players are in the lobby
    char *message = malloc((size_t) 2 + (game->players->size * 16));
    int offset = 0;

    memmove(message + offset, inttostr(2, 1), 1);
    offset += 1;
    memmove(message + offset, inttostr(game->players->size, 1), 1);
    offset += 1;

    Player *player = game->players->head;

    while (player) {
        memmove(message + offset, player->name, 16);
        offset += 16;
        player = player->next;
    }
    broadcast(message, offset);
    free(message);
}

/**
 *
 * @param socket
 * @param player
 * @param code
 */
void print_lobby_info(Game *game) {  // print the info about the player who has joined and his starting coordinates

    Player *player = game->players->head;
    puts("====================");
    while (player) {
        printf("ID: %d SYMBOL: %c NAME: %s READY: %d COORDS: %d;%d\n", player->id, player->letter, player->name, player->state, player->x, player->y);
        player = player->next;
    }
    puts("====================");
}

void send_game_start(Game *game){

}

void send_map_row() {

}

void send_map() {

}

void send_game_update(Game *game) {
}

void game_tick(Game *game) { // handling the requestes for the movement and control the game tick
    MoveRequest *request = game->queue->head;

    while (request) {
        Player *player = request->player;
        switch (request->direction) {
            case DIR_UP:
                if (is_path(game->map, player->x, player->y - 1)) {
                    player->y--;
                }
                break;
            case DIR_DOWN:
                if (is_path(game->map, player->x, player->y + 1)) {
                    player->y++;
                }
                break;
            case DIR_LEFT:
                if (is_path(game->map, player->x - 1, player->y)) {
                    player->x--;
                }
                break;
            case DIR_RIGHT:
                if (is_path(game->map, player->x + 1, player->y)) {
                    player->x++;
                }
                break;
        }

        request = request->next;
    }

    if (game->queue->size > 0) {
        send_game_update(game);
        clear_requests(game);
    }
}

/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_player_disconnect_packet(Socket *socket, Player *player, char *payload) {
    close(*socket->socket);
}