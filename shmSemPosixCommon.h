// shmSemPosixCommon.h
#ifndef SHM_SEM_POSIX_COMMON_H
#define SHM_SEM_POSIX_COMMON_H

#include <semaphore.h>

#define SHM_NAME "/shm_with_sem"
#define MESSAGE_SIZE 256

typedef struct {
    char message[MESSAGE_SIZE];
    sem_t sem_empty; // indica si el buffer está vacío (escritor puede escribir)
    sem_t sem_full;  // indica si hay datos listos (lector puede leer)
} shared_data_t;

#endif

