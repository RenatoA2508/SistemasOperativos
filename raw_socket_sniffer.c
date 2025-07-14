#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h> // struct iphdr

#define BUFSIZE 65536

int main() {
    int sock_raw;
    struct sockaddr_in source;
    int saddr_size, data_size;
    unsigned char *buffer = (unsigned char *)malloc(BUFSIZE);

    // Crear socket RAW para protocolo IP
    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sock_raw < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Asociarlo a una interfaz (ej. "eth0")
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = 0;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // usa tu IP local si es necesario

    if (bind(sock_raw, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Error al hacer bind");
        return 1;
    }

    printf("Escuchando paquetes...\n");

    saddr_size = sizeof(source);
    while (1) {
        data_size = recvfrom(sock_raw, buffer, BUFSIZE, 0,
                             (struct sockaddr *)&source, (socklen_t *)&saddr_size);
        if (data_size < 0) {
            perror("Error al recibir datos");
            return 1;
        }

        struct iphdr *iph = (struct iphdr *)buffer;
        struct in_addr src_addr, dst_addr;
        src_addr.s_addr = iph->saddr;
        dst_addr.s_addr = iph->daddr;

        printf("Paquete IP: %s -> %s\n",
               inet_ntoa(src_addr), inet_ntoa(dst_addr));
    }

    close(sock_raw);
    return 0;
}

