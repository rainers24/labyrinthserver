#include <pthread.h>
#include <string.h>
#include <stdio.h>


#include "net.h"
#include "game.h"


void* input_reader(void* arg) {
    char input[64], cmd[20], args[44];

    while (gets(input)) {
        printf("Server input: %s \n", input);
        sscanf(input, "%s %s", cmd, args);
        if (strcmp(cmd, "stop") == 0) {
            stop_server();
        }

        if (strcmp(cmd, "chat") == 0) {
            broadcast(args, 44);
        }
        if (strcmp(cmd, "map") == 0) {
            print_map(game->map);
        }
        if (strcmp(cmd, "lobby") == 0) {
            print_lobby_info(game);
        }
        if (strcmp(cmd, "strtoint") == 0) {
            printf("%d", strtoint(args));
        }
        bzero(input, 64);
        bzero(cmd, 20);
        bzero(args, 44);
    }
    return NULL;
}


pthread_t create_input_reader_thread() {
    pthread_t input_reader_thread;
    pthread_create(&input_reader_thread, NULL, input_reader, NULL);
    return input_reader_thread;
}


