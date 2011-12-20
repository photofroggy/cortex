#include <stdio.h>
#include <string.h>
#include "lib/damn.h"

static void precv_msg(packet * pack, const void *obj);

int main(int argv, char** argc) {
    
    packet * pkt = malloc(sizeof(packet));
    pkt = parse("recv chat:Botdom\n\nmsg main\nfrom=photofroggy\n\nsomething else here\0");
    
    pkt->subpacket = malloc(sizeof(packet));
    pkt->subpacket = parse(pkt->body);
    
    ptoevent(pkt);
    
    printf("event: %s\n", pkt->event);
    
    // Testing out the callback mapping
    packet_cbmap * map = new_packet_cbmap("event");
    
    //cbmap_add_callback(map, precv_msg);
    packet_callback_add(map, "recv_msg", precv_msg);
    
    fire_pcallback(map, pkt, NULL);
    
    return 0;
}

static void precv_msg(packet * pack, const void *obj) {
    printf("---\npacket callback `precv_msg` called for %s\n", pack->event);
    printf("[#%s] <%s> %s\n----\n",
        pack->param + 5,
        packet_arg_find(pack->subpacket, "from"),
        pack->subpacket->body
    );
}

// EOF
