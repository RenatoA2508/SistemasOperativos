// Posix Message Queue

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>    // For O_* constants
#include <sys/stat.h> // For mode constants

#define QUEUE_NAME  "/myqueue"
#define MAX_SIZE    256

int main() {
    mqd_t mq;
    char message[MAX_SIZE] = "Hello from sender!";
    
    mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, NULL);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    printf("Message sent: %s\n", message);
    mq_close(mq);

    return 0;
}

