#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int sv[2];
    char buf[128];

    if (socketpair(PF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        exit(1);
    }

    if (fork() > 0) { // Padre
        close(sv[1]);  // Cierra el extremo del hijo
        write(sv[0], "MSG1", 5);
        write(sv[0], "MSG2", 5);
        close(sv[0]);
        exit(0);
    }

    // Hijo
    close(sv[0]);  // Cierra el extremo del padre
    int n = read(sv[1], buf, sizeof(buf));
    buf[n] = '\0'; // Asegura terminación de cadena
    //printf("HIJO (STREAM): leí %d bytes: '%s'\n", n, buf);
    write(STDOUT_FILENO, buf, n);  // muestra todos los bytes, incluso si hay '\0'
    close(sv[1]);
    exit(0);
}

