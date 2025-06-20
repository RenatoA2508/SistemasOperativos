#include <stdio.h>

void print_stack_growth(int depth) {
    int local_var = depth;  // Each call creates a new local variable on the stack
    printf("Depth: %3d | Address of local_var: %p\n", depth, (void *)&local_var);

    if (depth < 10) {  // Adjust this limit based on your stack size
        print_stack_growth(depth + 1);
    }
}

int main() {
    printf("=== Stack Growth Observation ===\n");
    print_stack_growth(0);
    return 0;
}

