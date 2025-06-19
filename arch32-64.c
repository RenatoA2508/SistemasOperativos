// Diferencia en output en 32-bit and 64-bit gcc
// gcc -m32 (-m64)
#include<stdio.h>
int main() {
  printf("Size = %u\n", sizeof(size_t));
}
