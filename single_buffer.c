
// single_buffer.c
// Copia un archivo utilizando un solo búfer y read/write

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 262144

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <origen> <destino>\n", argv[0]);
        return 1;
    }

    int in_fd = open(argv[1], O_RDONLY);
    int out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (in_fd < 0 || out_fd < 0) {
        perror("Error al abrir archivos");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    while ((bytes_read = read(in_fd, buffer, BUFFER_SIZE)) > 0) {
        write(out_fd, buffer, bytes_read);
	// Simulación de cómputo pesado
        for (volatile int i = 0; i < 100000000; i++);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Tiempo total (single buffer): %.4f segundos\n", elapsed);

    close(in_fd);
    close(out_fd);
    return 0;
}
