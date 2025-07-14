#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> // ETH_P_ALL
#include <arpa/inet.h>

#define BUFSIZE 65536

int main() {
    int sockfd;
    unsigned char *buffer = (unsigned char *)malloc(BUFSIZE);

    // Crear socket de captura de paquetes (raw)
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Error al crear socket raw");
        return 1;
    }

    printf("Escuchando paquetes (AF_PACKET)...\n");

    while (1) {
        ssize_t data_size = recvfrom(sockfd, buffer, BUFSIZE, 0, NULL, NULL);
        if (data_size < 0) {
            perror("Error al recibir datos");
            break;
        }

        // Saltar el encabezado Ethernet (14 bytes)
        struct iphdr *iph = (struct iphdr *)(buffer + 14);
        struct in_addr src, dst;
        src.s_addr = iph->saddr;
        dst.s_addr = iph->daddr;

        printf("IP: %s -> %s | Protocolo: %d | Tamaño: %ld bytes\n",
               inet_ntoa(src), inet_ntoa(dst), iph->protocol, data_size);
    }

    close(sockfd);
    free(buffer);
    return 0;
}

