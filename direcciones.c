#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// Función recursiva
void F(int n){
    printf("Llamada F(%d) - Dirección de n: %p\n", n, (void*)&n);
    if(n == 0) return;
    F(n - 1);
}

int main(int argc, char *argv[]){
    printf("Dirección de main: %p\n", (void*)main);
    printf("Dirección de F: %p\n", (void*)F);    
    printf("Dir original Heap %p\n", sbrk(0)); //Devuelve la direccion original del heap, donde termina la seccion de datos globales
    sleep(1);

    int *arr = (int*)malloc(atoi(argv[1])*sizeof(int));
	if(arr == NULL){
		perror("malloc");
		return 1;
	}
    printf("Direccion de malloc %p\n", arr);


    printf("Dir actualizada Heap %p\n", sbrk(0));

    int *c = (int*)malloc(atoi(argv[2])*sizeof(int));
    if(c == NULL){
                perror("malloc");
                return 1;
        }

    printf("Direccion de malloc %p\n", c);


    printf("Dir actualizada Heap %p\n", sbrk(0));

	

    arr[0] = 10;
    arr[1]= 100;
    arr[35000]=3;

    printf("%d, %d, %d\n", arr[0], arr[1], arr[3500]);


    int valor_inicial = 5;
    F(valor_inicial); // Llamamos a F con un valor inicial
	free(arr);
    return 0;
}
