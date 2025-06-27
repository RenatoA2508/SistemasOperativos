// usa un pipe para enviar un entero del proceso padre al hijo.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipefd[2];        // pipefd[0] is read end, pipefd[1] is write end
    pid_t pid;
    int number = 42;      // The integer to send

    number = atoi(argv[1]);
    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    printf ("Descriptores: %d --> %d\n", pipefd[0], pipefd[1]);

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        // Parent process
        close(pipefd[0]);  // Close unused read end

        // Write the integer to the pipe
        if (write(pipefd[1], &number, sizeof(number)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);  // Close write end after writing
        wait(NULL);        // Wait for child to finish
    }
    else {
        // Child process
        close(pipefd[1]);  // Close unused write end

        int received;
        if (read(pipefd[0], &received, sizeof(received)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Child received integer: %d\n", received);
        close(pipefd[0]);  // Close read end
    }

    return 0;
}

