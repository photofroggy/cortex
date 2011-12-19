#include "damn.h"


packet* parse(char* pkt) {
    packet *pack = malloc(sizeof(packet));
    char * pch = strstr(pkt, "\n\n");
    char head[8092];
    char line[1000];
    packet_arg * targ;
    
    if(pch == 0) {
        strcpy(head, pkt);
    } else {
        strncpy(head, pkt, pch - pkt);
        strcpy(pack->body, pch+2);
    }
    
    pch = strchr(head, '\n');
    
    if(pch == 0) {
        targ = parse_arg(head, ' ');  
        
        if(targ == NULL) {
            strcpy(pack->command, head);
        } else {
            strcpy(pack->command, targ->key);
            strcpy(pack->param, targ->value);
        }
            
        return pack;
    } else {
        printf("lol\n");
        strncpy(line, head, pch - head);
        targ = parse_arg(line, ' ');
        
        if(targ == NULL) {
            strcpy(pack->command, head);
        } else {
            strcpy(pack->command, targ->key);
            strcpy(pack->param, targ->value);
        
        }
        strcpy(head, pch + 1);
    }
    
    while(pch != 0) {
        pch = "";
        line[0] = '\0';
        pch = strchr(head, '\n');
        
        if(pch != 0) {
            strncpy(line, head, pch - head);
            line[pch - head] = '\0';
            head[0] = '\0';
            strcpy(head, pch + 1);
        } else {
            strcpy(line, head);
        }
        
        targ = parse_arg(line, '=');
        
        if(targ != NULL) {
            packet_arg_add(pack, targ);
        }
        
    }
    /*
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
    */

    return pack;
}

packet_arg* parse_arg(char * line, int sep) {
    packet_arg *arg = malloc(sizeof(packet_arg));
    char * pch = strchr(line, sep);
    
    if(pch == 0) {
        return NULL;
    }
    
    strncpy(arg->key, line, pch - line);
    strcpy(arg->value, pch + 1);
    
    arg->key[pch - line] = '\0';
    
    return arg;
}

void packet_arg_add(packet * pack, packet_arg * arg) {
    if(pack->parg == NULL) {
        pack->parg = arg;
        return;
    }
    packet_arg * targ;
    targ = pack->parg;
    
    while(targ->next != NULL) {
        targ = targ->next;
    }
    
    targ->next = arg;
}

char* packet_arg_find(packet * pack, char * key) {
    packet_arg * arg;
    arg = pack->parg;
    
    while(arg != NULL) {
        if(!strcmp(arg->key, key))
            return arg->value;
        
        arg = arg->next;
    }
    
    return NULL;
}

static void inspect_hash(const char *key, const char *value, const void *obj) {
    printf("%s = %s\n", key, value);
}

void inspect(packet* pk) {
    printf("command = %s\nparam = %s\n", pk->command, pk->param);
    sm_enum(pk->args, inspect_hash, NULL);
}
