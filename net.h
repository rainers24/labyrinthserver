#ifndef LABYRINTH_SERVER_NET_H
#define LABYRINTH_SERVER_NET_H

#include "libs.h"

typedef struct Socket {
    struct Socket* next;
    struct Socket* prev;
    int* socket;
    pthread_t* thread;
} Socket;

typedef struct SocketList {
    Socket* head;
    Socket* tail;
    int size;
} SocketList;

Socket* create_socket(int*, pthread_t*);

SocketList *create_socketlist();

Socket* put_socket(Socket*);

void delete_socket(Socket*);

int create_server();

void stop_server();

int bind_connection_handler(int, void(*));

//void *connection_handler(void *);

void broadcast(char*, int);

void net_error_handler();

#endif //LABYRINTH_SERVER_NET_H
