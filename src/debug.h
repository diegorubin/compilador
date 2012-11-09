#ifndef __MYPAS_DEBUG_H__
#define __MYPAS_DEBUG_H__

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

enum {
  WAITING=260,
  SYMTABCLEAR,
  SYMTABINSERT,
};

int debugfd;
char *gbuffer;

void debug_init();
void debug_finalize();
void debug_send_message(char *message);

int debug_isinitialized();

#endif //__MYPAS_DEBUG_H__

