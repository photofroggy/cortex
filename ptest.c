#include <stdio.h>
#include <string.h>
#include "lib/damn.h"

int main(int argv, char** argc) {
    
    printf("Yo!\n");
    
    packet * pkt = malloc(sizeof(packet));
    pkt = parse("hello world\nfoo=bar\nlol=wot\n\nsomething else here\0");
    
    printf("cmd: %s\nparam: %s\nargs:", pkt->command, pkt->param);
    
    packet_arg * arg = pkt->parg;
    
    while(arg != NULL) {
        printf("\n-- %s = %s", arg->key, arg->value);
        arg = arg->next;
    }
    
    printf("\nbody: %s\n", pkt->body);
    
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
