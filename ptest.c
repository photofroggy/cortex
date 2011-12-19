#include <stdio.h>
#include <string.h>
#include "lib/damn.h"

int main(int argv, char** argc) {
    
    packet * pkt = malloc(sizeof(packet));
    pkt = parse("recv chat:Botdom\n\nmsg main\nfrom=photofroggy\n\nsomething else here\0");
    
    pkt->subpacket = malloc(sizeof(packet));
    pkt->subpacket = parse(pkt->body);
    
    printf("event: %s\nargs:", packet_event_name(pkt), pkt->subpacket->command);
    
    packet_arg * arg = pkt->subpacket->arg;
    
    while(1) {
        printf("\n-- %s = %s", arg->key, arg->value);
        arg = arg->next;
        if(arg == NULL) {
            break;
        }
    }
    
    printf("\nmessage: %s\n", pkt->subpacket->body);
    
    // Should succeed.
    printf("\n---------------\nSearching for arg 'foo'...\n");
    
    char * pargv;
    pargv = packet_arg_find(pkt, "foo");
    
    if(pargv == NULL) {
        printf("foo not found!");
    } else {
        printf("foo found! Foo = %s", pargv);
    }
    
    // Should fail.
    printf("\n---------------\nSearching for arg 'baz'...\n");
    
    pargv = packet_arg_find(pkt, "baz");
    
    if(pargv == NULL) {
        printf("baz not found!");
    } else {
        printf("baz found! Baz = %s", pargv);
    }
    
    return 0;
}

// EOF
