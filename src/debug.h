#ifndef __MYPAS_DEBUG_H__
#define __MYPAS_DEBUG_H__

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

enum {
  DEBUG_WAITING=260,
  DEBUG_SYMTABCLEAR,
  DEBUG_SYMTABINSERT,
  DEBUG_INSERTINPUTCODE,
  DEBUG_CLEAR,
};

int debugfd;
char *gbuffer;

int pidofcdb;

void debug_init();
void debug_finalize();
void debug_send_message(char *message);
void debug_clear();

int debug_isinitialized();

char *debug_clone_sourcecode(FILE *target);

/* threads */
pthread_t t_cdb;

/* Methods */
void *call_cdb();

#endif //__MYPAS_DEBUG_H__

