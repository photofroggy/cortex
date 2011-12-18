#include "utils.h"

char* read_all(int socket, int length) {
    char *buf = malloc(sizeof(char[length]));

    if(recv(socket, buf, length, 0) == -1) {
        fprintf(stderr, "recv() failed: %s", strerror(errno));
        return NULL;
    } else {
        return buf;
    }
}
