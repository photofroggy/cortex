#ifndef _DAMN_H_
#define _DAMN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strmap.h"

typedef struct _packet_arg {
    char key[30];
    char value[30];
    struct _packet_arg * next;
} packet_arg;

typedef struct _packet {
    char command[11];
    char param[64];
    char body[8192];
    struct _packet_arg * arg;
    struct _packet *subpacket;
} packet;

typedef struct _packet_event {
    char name[20];
    packet * pkt;
} packet_event;

/*typedef _packet_arg packet_arg;
typedef _packet packet;*/

packet* parse(char* pkt);
packet_arg* parse_arg(char* line, int sep);

void packet_arg_add(packet * pack, packet_arg * arg);
char* packet_arg_find(packet * pack, char * key);

char* packet_event_name(packet * pack);
packet_event* ptoevent(packet * pack);

void inspect(packet* pkt);

#ifdef __cplusplus
}
#endif

#endif
