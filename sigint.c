//  Ctrl+C: ejecuta signal_handler_fun en lugar de terminar el programa (default)
//
#include <stdio.h>
#include <signal.h>

void signal_handler_fun(int signum) {
    printf("catch signal %d\n", signum);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler_fun);
    while (1);
    return 0;
}
