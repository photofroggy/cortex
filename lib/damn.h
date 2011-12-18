#include <stdio.h>
#include <string.h>

typedef struct _packet {
    char command[13];
    char param[64];
    char body[8192];
    char **args;
    void *subpacket;
} packet;

packet* parse(char* pkt);

void inspect(packet* pkt);
