#include "libs.h"
#include "net.h"
#include "input.h"
#include "timer.h"
#include "game.h"

void signal_handler(int sig)
{
//    stop_server();
}

void create_signal_handler() {

    printf("pid: %d\n", getpid());
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sigaction(SIGINT, &sa, NULL);


}

int main(int argc , char *argv[])
{

    create_signal_handler();
    create_input_reader_thread();
    create_game();
    create_tick_timer_thread();


    int listen_socket = create_server();
    bind_connection_handler(listen_socket, connection_handler);

    return 0;
}
