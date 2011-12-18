#include "damn.h"

packet* parse(char* pkt) {
    packet *pack = malloc(sizeof(packet));

    sscanf(pkt, "%s %s", pack->command, pack->param);
    return pack;
}

void inspect(packet* pk) {
    printf("command = %s\nparam = %s", pk->command, pk->param);
}
