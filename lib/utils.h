#include <stdio.h>
#ifdef WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char* read_all(int sock, int size_lim);
