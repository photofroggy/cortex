#include "utils.h"

char* read_all(int socket, int length) {
    char *buf = malloc(sizeof(char[length]));
    int total_bytes = 0;
    int current_bytes;

    current_bytes = recv(socket, buf, length, 0);
    if(current_bytes == -1) {
        fprintf(stderr, "recv() failed: %s", strerror(errno));
        return NULL;
    } else {
        return buf;
    }
}
