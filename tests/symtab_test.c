#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <symtab.h>
#include "tap.h"

void test_clearenv() 
{
  lextape_nextentry = 0;
  symtab_nextentry = 1;
}

int main(int argc, char **argv)
{
  plan_tests(10);

  /* variavel nao declarada */
  ok1(!symtab_lookup("xurisso"));

  /* declarando variavel */
  ok1(symtab_insert("xurisso", INTEGER, 1, 4));

  /* buscando variavel declarada */
  ok1(symtab_lookup("xurisso"));
  
  test_clearenv();

  /*limpeza da variaveis declaradas dentro de um modulo*/
  ok1(symtab_insert("funcao", INTEGER, SYMTAB_IDTYPE_FUNCTION, 4));

  /*a funcao tem dois parametros*/
  ok1(symtab_insert("x", INTEGER, SYMTAB_IDTYPE_PARAMETER, 4));
  ok1(symtab_insert("y", INTEGER, SYMTAB_IDTYPE_PARAMETER, 4));

  /* e variaveis locais*/
  ok1(symtab_insert("a", INTEGER, SYMTAB_IDTYPE_VARIABLE, 4));
  ok1(symtab_insert("b", INTEGER, SYMTAB_IDTYPE_VARIABLE, 4));

  symtab_dispose_local_variables();

  /* encerrado scopo da funcao, symtab deve ter apenas 
   * o registro da funcao que é global*/
  ok1(symtab_nextentry == 2);

  /* variavel local da funcao nao pode ser acessada mais*/
  ok1(!symtab_lookup("a"));

  return exit_status();
}

