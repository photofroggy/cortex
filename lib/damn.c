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
    
    arg->next = NULL;
    
    if(pch == 0) {
        return NULL;
    }
    
    strncpy(arg->key, line, pch - line);
    strcpy(arg->value, pch + 1);
    
    arg->key[pch - line] = '\0';
    //arg->value[strlen(pch + 1)] = '\0';
    
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

static void inspect_hash(const char *key, const char *value, const void *obj) {
    printf("%s = %s\n", key, value);
}

char* packet_event_name(packet * pack) {
        char * name = pack->command;
        
        if(strcmp(name, "recv")) {
            return name;
        }
        
        pack->subpacket = parse(pack->body);
            
        if(pack->subpacket == NULL) {
            return name;
        }
        
        strcat(name, "_");
        strcat(name, pack->subpacket->command);
            
        if(strcmp(pack->subpacket->command, "admin")) {
            return name;
        }
        
        strcat(name, "_");
        strcat(name, pack->subpacket->param);
        
        return name;
}

void ptoevent(packet * pack) {
    strcpy(pack->event, packet_event_name(pack));
}

packet_cbmap* new_packet_cbmap(char * event) {
    packet_cbmap * map = malloc(sizeof(packet_cbmap));
    strcpy(map->event, event);
    map->event[strlen(event)] = '\0';
    map->callback = NULL;
    map->next = NULL;
    return map;
}

packet_callback* new_packet_callback(dpcallback method) {
    packet_callback * cb = malloc(sizeof(packet_callback));
    cb->method = method;
    cb->next = NULL;
    return cb;
}

packet_callback* packet_callback_add(packet_cbmap * callbacks, char * event, dpcallback method) {
    packet_cbmap * chain;
    packet_callback * hop;
    
    chain = get_packet_callbacks(callbacks, event);
    
    if(chain->callback == NULL) {
        chain->callback = new_packet_callback(method);
        
        return chain->callback;
    }
    
    hop = chain->callback;
    while(1) {
        if(hop->next == NULL)
            break;
        hop = hop->next;
    }
    
    hop->next = new_packet_callback(method);
    return hop->next;
}

packet_cbmap* get_packet_callbacks(packet_cbmap * callbacks, char * event) {
    packet_cbmap * tmap = callbacks;
    int count = 0;
    
    while(1) {
        count++;
        if(tmap->event[0] == '\0') {
            strcpy(tmap->event, event);
            tmap->event[strlen(event)] = '\0';
            break;
        }
        
        if(!strcmp(tmap->event, event))
            return tmap;
        
        if(tmap->next == NULL)
            break;
        
        tmap = tmap->next;
    };
    
    if(!strcmp(tmap->event, event)) {
        tmap->next = new_packet_cbmap(event);
        tmap = tmap->next;
        strcpy(tmap->event, event);
        tmap->event[strlen(event)] = '\0';
    }
    
    return tmap;
}

packet_callback* cbmap_add_callback(packet_cbmap * callbacks, dpcallback method) {
    packet_callback * cb;
    
    if(callbacks->callback == NULL) {
        callbacks->callback = new_packet_callback(method);
        return callbacks->callback;
    }
    
    cb = callbacks->callback;
    
    while(1) {
        if(cb->next == NULL) {
            cb->next = new_packet_callback(method);
            return cb->next;
        }
        cb = cb->next;
    }
    
    return NULL;
}

void fire_pcallback(packet_cbmap * callbacks, packet * pkt, const void *obj) {
    packet_cbmap * map = get_packet_callbacks(callbacks, pkt->event);
    
    if(map->callback == NULL)
        return;
    
    packet_callback * callback = map->callback;
    
    while(1) {
        
        if(callback == NULL)
            break;
        
        callback->method(pkt, obj);
        callback = callback->next;
    }
}

int packet_map_count(packet_cbmap * callbacks) {
    packet_cbmap * map = callbacks;
    int count = 0;
    
    while(1) {
        if(map == NULL)
            return count;
        
        count++;
        map = map->next;
        
    }
}

void inspect(packet* pk) {
    /* Deprecated I guess. Needs a rewrite.
    printf("command = %s\nparam = %s\n", pk->command, pk->param);
    sm_enum(pk->args, inspect_hash, NULL);
    */
}
