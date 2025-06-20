#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    //printf("=== Heap Growth Visualization ===\n");

    void *initial_brk = sbrk(0);
    printf("Initial program break:    %p\n\n", initial_brk);

    const int ALLOCS = 5;
    const int CHUNK_SIZE = 130000;

    for (int i = 0; i < ALLOCS; i++) {
        void *before = sbrk(0);

        void *ptr = malloc(CHUNK_SIZE);  // allocate small chunk
        if (!ptr) {
            perror("malloc");
            return 1;
        }

        void *after = sbrk(0);

        printf("malloc(%d): %p\n", CHUNK_SIZE, ptr);
        printf("Program break before:     %p\n", before);
        printf("Program break after:      %p\n", after);

        printf("Break changed? %s\n\n", (before != after) ? "YES" : "NO");
    }

    printf("Final program break:      %p\n", sbrk(0));

    printf("\nNote: malloc may allocate more than requested!\n");
    printf("      Heap grows only when needed, often in large blocks.\n");
    system("cat /proc/self/maps");
	getchar();
    return 0;
}

