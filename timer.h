#ifndef LABYRINTH_SERVER_TIMER_H
#define LABYRINTH_SERVER_TIMER_H

pthread_t create_tick_timer_thread();

void *tick_timer(void*);

#endif //LABYRINTH_SERVER_TIMER_H
