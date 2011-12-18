#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "utils.c"

int main(int argv, char** argc) {
    setvbuf(stdout, NULL, _IONBF, 0);

    struct addrinfo addr, *res;
    int sock;

    memset(&addr, 0, sizeof addr);
    addr.ai_family = AF_UNSPEC;
    addr.ai_socktype = SOCK_STREAM;

    getaddrinfo("chat.deviantart.com", "3900", &addr, &res);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        fprintf(stderr, "You screwed up.");
        return -1;
    }

    char *msg = "dAmnClient 0.3\nagent=incluye\n";

    printf("Wrote %d bytes.\n", write(sock, msg, strlen(msg) + 1));

    char *resp = read_all(sock, 44);

    printf("%s", resp);

    free(resp);

    close(sock);

    return 0;
}
