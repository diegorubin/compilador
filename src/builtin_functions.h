#ifndef __BUILTIN_FUNCTIONS_H__
#define __BUILTIN_FUNCTIONS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

enum {
  WRITE=4096,
  READ,
};

int isbuiltin(char const *identifier);

void builtin_write(FILE *target);
void builtin_read(FILE *target);

#endif /*__BUILTIN_FUNCTIONS_H__*/
