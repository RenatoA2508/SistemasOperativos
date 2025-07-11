// Orden de ejecucion entre padre e hijo
// semctl: depending on cmd, more parameters may be required. 
// For SETVAL cmd, provide union semun structure as fourth parameter (value to be set)
//   union semun {
//      int val;               // Value used for SETVAL command
//      struct semid_ds *buf;  // Buffer used for IPC_STAT and IPC_SET commands
//      unsigned short *array; // Array used for GETALL and SETALL commands
//   };

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

union semun {
    int val;
};

int main(void) {
    int semid;
    int key;
    pid_t pid;
    struct sembuf sem;
    union semun semun_union;

    key = ftok("./a.c", 0666);
    semid = semget(key, 1, 0666 | IPC_CREAT);
    semun_union.val = 0;
    semctl(semid, 0, SETVAL, semun_union);
    pid = fork();
    if (pid > 0) {
        sem.sem_num = 0;
        sem.sem_op = -1;  // P operation
        sem.sem_flg = 0;
        semop(semid, &sem, 1);
        printf("Padre\n");
        sem.sem_num = 0;
        sem.sem_op = 1;
        sem.sem_flg = 0;
        semop(semid, &sem, 1);
    }

    if (pid == 0) {
        sleep(2);
        sem.sem_num = 0;
        sem.sem_op = 1; //V operation
        sem.sem_flg = 0;
        semop(semid, &sem, 1);
        printf("Hijo\n");
    }
    return 0;
}

