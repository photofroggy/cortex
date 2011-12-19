#include "damn.h"


packet* parse(char* pkt) {
    packet *pack = malloc(sizeof(packet));
    char * pch = strstr(pkt, "\n\n");
    char head[8092];
    char line[1000];
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
    
    // Find where the first linebreak is, so we can get the command and param.
    pch = strchr(head, '\n');
    
    if(pch == 0) {
        // First line must always be the command and/or param.
        targ = parse_arg(head, ' ');  
        
        if(targ == NULL) {
            strcpy(pack->command, head);
        } else {
            strcpy(pack->command, targ->key);
            strcpy(pack->param, targ->value);
        }
            
        return pack;
    } else {
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
    
    
    // Process each line under the command and param in the header.
    while(pch != 0) {
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
        
        // Parse `line` as an arg.
        targ = parse_arg(line, '=');
        
        if(targ != NULL) {
            // Add the arg if parsed successfully.
            packet_arg_add(pack, targ);
        }
        
    }

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
