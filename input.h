#ifndef LABYRINTH_SERVER_INPUT_H
#define LABYRINTH_SERVER_INPUT_H

pthread_t create_input_reader_thread();

void input_reader(void*);

#endif //LABYRINTH_SERVER_INPUT_H
