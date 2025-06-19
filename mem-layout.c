// Experimentar con layout de MEM:
// variables locales, globales y estaticas
// inicializadas y no inicializadas
//   *** size mem-layout - layout de memoria (text, data, bss)

#include <stdio.h>
#include <unistd.h>

int data = 10; // variable en DATA
int bss; // variable en BSS
 
int main(void) {
  static int staticInit = 99; // static variable en DATA
  static int staticZ; // static variable en BSS
  int local;     // variable local en stack

  int intV;
  float floatV;
  char charV;

  printf("direccion de main():     %p\n", main);
  printf("direccion de data:       %p\n", &data);
  printf("direccion de bss:        %p\n", &bss);
  printf("direccion de staticInit: %p\n", &staticInit);
  printf("direccion de staticZ:    %p\n", &staticZ);
  printf("direccion de local:      %p\n", &local);
  printf("direccion de intV:       %p\n", &intV);
  printf("direccion de floatV:     %p\n", &floatV);
  printf("direccion de charV:      %p\n", &charV);

  unsigned char a=1, b=1, c=5, d=7;

  printf("a= %d;  b= %d; c= %d, d= %d\n", a, b, c, d);
  printf("Dir-a= %p;  Dir-b= %p; Dir-c= %p\n", &a, &b, &c);
  int *p = (int*)&a;
  *p = 10300000;
  printf("a= %d;  b= %d; c= %d, d= %d\n", a, b, c, d);

  printf ("PID: %d\n", getpid());
  sleep (480);

}
