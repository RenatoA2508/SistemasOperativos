// File Write with timing and mode selection
// Compare the performance of different modes: sync, direct, and both.
// Latency of a single 4KB write in different I/O modes.
// Performance differences between buffered (sync) and unbuffered (direct).
// To benchmark robustly, loop over multiple writes and compute averages.
//
// gcc -o timed_writer timed_writer.c -lrt
// ./timed_writer test_sync.bin sync
// ./timed_writer test_direct.bin direct
// ./timed_writer test_both.bin both


#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define BLOCK_SIZE 4096

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <filename> [sync|direct|both]\n", prog);
    exit(EXIT_FAILURE);
}

double time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
    if (argc < 3) usage(argv[0]);

    const char *filename = argv[1];
    const char *mode_str = argv[2];

    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int use_direct = 0;

    if (strcmp(mode_str, "sync") == 0) {
        flags |= O_SYNC;
    } else if (strcmp(mode_str, "direct") == 0) {
        flags |= O_DIRECT;
        use_direct = 1;
    } else if (strcmp(mode_str, "both") == 0) {
        flags |= O_SYNC | O_DIRECT;
        use_direct = 1;
    } else {
        usage(argv[0]);
    }

    // Open the file
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Allocate buffer
    void *buf;
    if (use_direct) {
        if (posix_memalign(&buf, BLOCK_SIZE, BLOCK_SIZE) != 0) {
            perror("posix_memalign");
            close(fd);
            return 1;
        }
    } else {
        buf = malloc(BLOCK_SIZE);
        if (!buf) {
            perror("malloc");
            close(fd);
            return 1;
        }
    }

    // Fill buffer with data
    memset(buf, 'X', BLOCK_SIZE);

    // Measure write time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    ssize_t written = write(fd, buf, BLOCK_SIZE);

    clock_gettime(CLOCK_MONOTONIC, &end);

    if (written < 0) {
        perror("write");
        free(buf);
        close(fd);
        return 1;
    }

    printf("Wrote %zd bytes to %s using mode: %s\n", written, filename, mode_str);
    printf("Write duration: %.6f seconds\n", time_diff(start, end));

    free(buf);
    close(fd);
    return 0;
}

