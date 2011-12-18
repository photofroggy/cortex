#include "damn.h"

packet* parse(char* pkt) {
    packet *pack;

    sscanf(pkt, "%s %*s", pack->command);
    return pack;
}

void inspect(packet* pk) {
    printf("command = %s\nparam = %s", pk->command, pk->param);
}
