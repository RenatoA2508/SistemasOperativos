#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global uninitialized variable (BSS)
int global_uninit;

// Global initialized variable (Data segment)
int global_init = 42;

// Static variable (Data or BSS depending on initialization)
static int static_var = 7;

void sample_function() {
    // Empty function just to get its address
}

int main() {
    // Local (stack) variable
    int local_var = 10;

    // Dynamically allocated memory (Heap)
    int *heap_var = malloc(sizeof(int) * 10);

    // Address of a string literal (usually in read-only text/rodata)
    const char *str = "Hello, memory!";

    printf("\n=== Memory Layout ===\n");

    printf("Function address         (text):     %p\n", (void *) sample_function);
    printf("Function Main address   (text):     %p\n", (void *) main);
    printf("Global init var addr     (data):     %p\n", (void *) &global_init);
    printf("Global uninit var addr   (bss):      %p\n", (void *) &global_uninit);
    printf("Static var addr          (data/bss): %p\n", (void *) &static_var);
    printf("String literal addr      (rodata):   %p\n", (void *) str);

    printf("Local var addr           (stack):    %p\n", (void *) &local_var);

    printf("Heap var addr            (heap):     %p\n", (void *) heap_var);

    // Print the program break (end of heap) using sbrk(0)
    printf("Heap program break (end of heap):    %p\n", sbrk(0));

    // Show memory map (optional)
    printf("\nYou can inspect more with:\n");
    printf("  cat /proc/%d/maps\n", getpid());

    // Prevent program from exiting immediately
    printf("\nPress ENTER to exit...");
    getchar();

    free(heap_var);
    return 0;
}

