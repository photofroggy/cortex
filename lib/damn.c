#include "damn.h"

struct _packet {
    char command[11];
    char param[64];
    char body[8192];
    StrMap *args;
    void *subpacket;
};

packet* parse(char* pkt) {
    packet *pack = malloc(sizeof(packet));
    pack->args = sm_new(20);
    char *pch;
    char key[20];
    char val[64];
    char *start;
    int field = 0;

    pch = strtok(pkt, "\n");
    while(pch != NULL) {
        if(field == 0) {
            sscanf(pch, "%s%s", pack->command, pack->param);
            ++field;
        } else if(field == 1) {
            start = strchr(pch, '=');
            strncpy(key, pch, start - pch);
            key[start - pch] = '\0';
            strcpy(val, start + 1);
            sm_put(pack->args, key, val);
        }
        pch = strtok(NULL, "\n");
    }

    return pack;
}

static void inspect_hash(const char *key, const char *value, const void *obj) {
    printf("%s = %s\n", key, value);
}

void inspect(packet* pk) {
    printf("command = %s\nparam = %s\n", pk->command, pk->param);
    sm_enum(pk->args, inspect_hash, NULL);
}
