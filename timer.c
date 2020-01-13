#include "libs.h"
#include "timer.h"
#include "game.h"

void* tick_timer(void* arg) {
    printf("tick timer started\n");
    int done = 0;
    while(!done) {
        msleep(100);
        game_tick(game);
    }
    return NULL;
}

pthread_t create_tick_timer_thread() {
    pthread_t tick_timer_thread;
    pthread_create(&tick_timer_thread, NULL, tick_timer, NULL);
    return tick_timer_thread;
}