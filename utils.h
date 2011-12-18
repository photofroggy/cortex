#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>

char* read_all(int sock, int size_lim);
