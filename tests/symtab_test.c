#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/symtab.h"
#include "tap.h"

int main(int argc, char **argv)
{
  plan_tests(3);

  /* variavel nao declarada */
  ok1(!symtab_lookup("xurisso"));

  /* declarando variavel */
  ok1(symtab_insert("xurisso", INTEGER, 1));

  /* buscando variavel declarada */
  ok1(symtab_lookup("xurisso"));

  return exit_status();
}

