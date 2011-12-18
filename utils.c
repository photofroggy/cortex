#include "utils.h"

char* read_all(int socket, int length) {
    char *buf = malloc(sizeof(char[length]));
    int total_bytes = 0;
    int current_bytes;

    if(recv(socket, buf, length, 0) == -1) {
        fprintf(stderr, "recv() failed: %s", strerror(errno));
        return NULL;
    } else {
        return buf;
    }
}
