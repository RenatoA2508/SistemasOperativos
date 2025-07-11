// Pipe con nombre (fifo) - proceso lector
// arg[1]: nombre del pipe
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buf[32] = {0};
    int fd;
    if (argc != 2) { printf("USO:%s <fifo name> \n", argv[0]); return -1; }
    fd = open(argv[1], O_RDONLY);
    while (1) {
        sleep(1);
        read(fd, buf, 32);
        printf("buf is %s\n", buf);
        memset(buf, 0, sizeof(buf));
    }
    close(fd);
    return 0;
}
