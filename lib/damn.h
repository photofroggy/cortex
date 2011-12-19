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

typedef struct _packet packet;

packet* parse(char* pkt);

void inspect(packet* pkt);

#ifdef __cplusplus
}
#endif

#endif
