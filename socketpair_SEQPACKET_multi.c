#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int sv[2];
    char buf[128];

    if (socketpair(PF_UNIX, SOCK_SEQPACKET, 0, sv) < 0) {
        perror("socketpair");
        exit(1);
    }

    if (fork() > 0) { // Padre
        close(sv[1]);
        write(sv[0], "MSG1", 5);
        write(sv[0], "MSG2", 5);
        close(sv[0]);
        exit(0);
    }

    // Hijo
    close(sv[0]);

    int n1 = read(sv[1], buf, sizeof(buf));
    buf[n1] = '\0';
    printf("HIJO (SEQPACKET): primer mensaje (%d bytes): '%s'\n", n1, buf);

    int n2 = read(sv[1], buf, sizeof(buf));
    buf[n2] = '\0';
    printf("HIJO (SEQPACKET): segundo mensaje (%d bytes): '%s'\n", n2, buf);

    close(sv[1]);
    exit(0);
}

