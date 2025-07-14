
// double_buffer.c
// Copia un archivo utilizando doble búfer en hilos separados (simulado)

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define BUFFER_SIZE 262144

typedef struct {
    int fd_in, fd_out;
    char *buffer1;
    char *buffer2;
    int turn;
    int done;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} buffer_data;

void *reader_thread(void *arg) {
    buffer_data *data = (buffer_data *)arg;
    int bytes;

    while (1) {
        pthread_mutex_lock(&data->lock);
        while (data->turn != 0) pthread_cond_wait(&data->cond, &data->lock);
        bytes = read(data->fd_in, data->buffer1, BUFFER_SIZE);
        if (bytes <= 0) {
            data->done = 1;
            pthread_cond_signal(&data->cond);
            pthread_mutex_unlock(&data->lock);
            break;
        }
        data->turn = 1;
        pthread_cond_signal(&data->cond);
        pthread_mutex_unlock(&data->lock);
    }

    return NULL;
}

void *writer_thread(void *arg) {
    buffer_data *data = (buffer_data *)arg;
    int bytes;

    while (1) {
        pthread_mutex_lock(&data->lock);
        while (data->turn != 1 && !data->done) pthread_cond_wait(&data->cond, &data->lock);
        if (data->done) {
            pthread_mutex_unlock(&data->lock);
            break;
        }
        bytes = write(data->fd_out, data->buffer1, BUFFER_SIZE);
        data->turn = 0;
        pthread_cond_signal(&data->cond);
        pthread_mutex_unlock(&data->lock);
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

    char *buf1 = malloc(BUFFER_SIZE);
    char *buf2 = malloc(BUFFER_SIZE);
    buffer_data data = {fd_in, fd_out, buf1, buf2, 0, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t reader, writer;
    pthread_create(&reader, NULL, reader_thread, &data);
    pthread_create(&writer, NULL, writer_thread, &data);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Tiempo total (double buffer simulado): %.4f segundos\n", elapsed);

    close(fd_in);
    close(fd_out);
    free(buf1);
    free(buf2);
    return 0;
}
