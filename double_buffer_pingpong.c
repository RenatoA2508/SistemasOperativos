// double_buffer_pingpong.c
// Implementación real de doble búfer (ping-pong) con hilos

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define BUFFER_SIZE 262144  // 256 KB
#define NUM_BUFFERS 2

typedef struct {
    char *data;
    ssize_t size;
    int full;
} buffer_t;

typedef struct {
    int fd_in, fd_out;
    buffer_t buffers[NUM_BUFFERS];
    int read_index;
    int write_index;
    int done_reading;
    pthread_mutex_t lock;
    pthread_cond_t can_read;
    pthread_cond_t can_write;
} shared_data;

void *reader_thread(void *arg) {
    shared_data *sd = (shared_data *)arg;

    while (1) {
        pthread_mutex_lock(&sd->lock);
        while (sd->buffers[sd->read_index].full) {
            pthread_cond_wait(&sd->can_read, &sd->lock);
        }

        ssize_t bytes = read(sd->fd_in, sd->buffers[sd->read_index].data, BUFFER_SIZE);
        if (bytes <= 0) {
            sd->done_reading = 1;
            pthread_cond_signal(&sd->can_write);
            pthread_mutex_unlock(&sd->lock);
            break;
        }

        // Simulación de cómputo pesado
        for (volatile int i = 0; i < 100000000; i++);

        sd->buffers[sd->read_index].size = bytes;
        sd->buffers[sd->read_index].full = 1;
        sd->read_index = (sd->read_index + 1) % NUM_BUFFERS;

        pthread_cond_signal(&sd->can_write);
        pthread_mutex_unlock(&sd->lock);
    }

    return NULL;
}

void *writer_thread(void *arg) {
    shared_data *sd = (shared_data *)arg;

    while (1) {
        pthread_mutex_lock(&sd->lock);
        while (!sd->buffers[sd->write_index].full && !sd->done_reading) {
            pthread_cond_wait(&sd->can_write, &sd->lock);
        }

        if (!sd->buffers[sd->write_index].full && sd->done_reading) {
            pthread_mutex_unlock(&sd->lock);
            break;
        }

        write(sd->fd_out, sd->buffers[sd->write_index].data, sd->buffers[sd->write_index].size);
        sd->buffers[sd->write_index].full = 0;
        sd->write_index = (sd->write_index + 1) % NUM_BUFFERS;

        pthread_cond_signal(&sd->can_read);
        pthread_mutex_unlock(&sd->lock);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <origen> <destino>\n", argv[0]);
        return 1;
    }

    int fd_in = open(argv[1], O_RDONLY);
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_in < 0 || fd_out < 0) {
        perror("Error al abrir archivos");
        return 1;
    }

    shared_data sd = {
        .fd_in = fd_in,
        .fd_out = fd_out,
        .read_index = 0,
        .write_index = 0,
        .done_reading = 0,
        .lock = PTHREAD_MUTEX_INITIALIZER,
        .can_read = PTHREAD_COND_INITIALIZER,
        .can_write = PTHREAD_COND_INITIALIZER
    };

    for (int i = 0; i < NUM_BUFFERS; i++) {
        sd.buffers[i].data = malloc(BUFFER_SIZE);
        sd.buffers[i].size = 0;
        sd.buffers[i].full = 0;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t reader, writer;
    pthread_create(&reader, NULL, reader_thread, &sd);
    pthread_create(&writer, NULL, writer_thread, &sd);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Tiempo total (ping-pong double buffer): %.4f segundos\n", elapsed);

    close(fd_in);
    close(fd_out);
    for (int i = 0; i < NUM_BUFFERS; i++) {
        free(sd.buffers[i].data);
    }

    return 0;
}
