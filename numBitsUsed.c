#include <stdio.h>
#include <stdint.h>

int main() {
    void *ptr = (void *)0xFFFFFFFFFFFFFFFF;
    printf("Pointer size: %zu bits\n", sizeof(ptr) * 8);
    return 0;
}
