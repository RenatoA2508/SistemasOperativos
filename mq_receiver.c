// Posix Msg Queue
//
// #include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/myqueue"
#define MAX_SIZE    4096

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];

    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }

    printf("Message received: %s\n", buffer);

    mq_close(mq);
    mq_unlink(QUEUE_NAME); // Delete the queue

    return 0;
}

