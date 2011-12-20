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
    char event[20];
    char command[11];
    char param[64];
    char body[8192];
    struct _packet_arg * arg;
    struct _packet *subpacket;
} packet;

typedef void(*dpcallback) (packet * pkt, const void *obj);

typedef struct _packet_callback {
    dpcallback method;
    struct _packet_callback * next;
} packet_callback;

typedef struct _packet_cbmap {
    char event[20];
    struct _packet_callback * callback;
    struct _packet_cbmap * next;
} packet_cbmap;


/*typedef _packet_arg packet_arg;
typedef _packet packet;*/

packet* parse(char* pkt);
packet_arg* parse_arg(char* line, int sep);

void packet_arg_add(packet * pack, packet_arg * arg);
char* packet_arg_find(packet * pack, char * key);

char* packet_event_name(packet * pack);
void ptoevent(packet * pack);

packet_callback* packet_callback_add(packet_cbmap * callbacks, char * event, dpcallback method);
packet_cbmap* get_packet_callbacks(packet_cbmap * callbacks, char * event);
packet_cbmap* new_packet_cbmap(char * event);
packet_callback* cbmap_add_callback(packet_cbmap * callbacks, dpcallback method);
packet_callback* new_packet_callback(dpcallback method);
void fire_pcallback(packet_cbmap * callbacks, packet * pkt, const void *obj);
int packet_map_count(packet_cbmap * callbacks);

void inspect(packet* pkt);

#ifdef __cplusplus
}
#endif

#endif
