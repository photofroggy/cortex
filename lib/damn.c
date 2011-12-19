#include "damn.h"


packet* parse(char* pkt) {
    packet *pack = malloc(sizeof(packet));
    char * pch = strstr(pkt, "\n\n");
    char head[8092];
    char line[1000];
    int item = 0;
    packet_arg * targ;
    
    /* If there is a packet body, we may as well handle that first, as this
     * parser is not interested in subpackets (yet). So, we look for two line
     * breaks, as in strstr above, and then copy the header into a separate
     * string.
     */
    if(pch == 0) {
        strcpy(head, pkt);
    } else {
        strncpy(head, pkt, pch - pkt);
        strcpy(pack->body, pch+2);
    }
    
    
    // Process each line under the command and param in the header.
    // Use a do...while to make sure single-line headers are processed.
    do {
        // Find the end of the line.
        pch = strchr(head, '\n');
        
        if(pch != 0) {
            // Copy the first line to `line`.
            strncpy(line, head, pch - head);
            // Make sure the last character is nul.
            line[pch - head] = '\0';
            // Copy the rest of the header into `head`.
            strcpy(head, pch + 1);
        } else {
            // Copy the header into `line` if we are on the last line.
            strcpy(line, head);
        }
        
        // Parse `line` as an arg or as command and/or param.
        if(item == 0) {
            targ = parse_arg(line, ' ');
        } else {
            targ = parse_arg(line, '=');
        }
        
        if(targ != NULL) {
            if(item == 0) {
                strcpy(pack->command, targ->key);
                strcpy(pack->param, targ->value);
            } else {
                // Add the arg if parsed successfully.
                packet_arg_add(pack, targ);
            }
        } else if(item == 0) {
            strcpy(pack->command, line);
        }
        
        item++;
        
    } while(pch != 0);

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
    if(pack->arg == NULL) {
        pack->arg = arg;
        return;
    }
    packet_arg * targ;
    targ = pack->arg;
    
    while(targ->next != NULL) {
        targ = targ->next;
    }
    
    targ->next = arg;
}

char* packet_arg_find(packet * pack, char * key) {
    packet_arg * arg;
    arg = pack->arg;
    
    while(arg != NULL) {
        if(!strcmp(arg->key, key))
            return arg->value;
        
        arg = arg->next;
    }
    
    return NULL;
}

/* static void inspect_hash(const char *key, const char *value, const void *obj) {
    printf("%s = %s\n", key, value);
}

void inspect(packet* pk) {
     Deprecated I guess. Needs a rewrite.
    printf("command = %s\nparam = %s\n", pk->command, pk->param);
    sm_enum(pk->args, inspect_hash, NULL);
    
} */
