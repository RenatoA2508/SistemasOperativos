// pipe con nombre (fifo) - escribidor 
// argv[1]: nombre del pipe
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int ret;
    char buf[32] = {0};
    int fd;
    if (argc != 2) { printf("Usage:%s <fifo name> \n", argv[0]); return -1; }

    if (access(argv[1], F_OK) == -1) {
        ret = mkfifo(argv[1], 0666);
        if (ret == -1) {
            perror("mkfifo");
            return -2;
        }
        printf("mkfifo is ok \n");
    }
    fd = open(argv[1], O_WRONLY);
    while (1) {
        sleep(1);
        write(fd, "hello", 5);
    }
    close(fd);
    return 0;
}
