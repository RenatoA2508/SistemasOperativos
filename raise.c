// Enviar signal (9) al mismo proceso

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(void) {
    printf("Anes de raise\n");
    raise(9);
    printf("Despues de raise\n");

    return 0;
}
