#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/symtab.h"
#include "tap.h"

int main(int argc, char **argv)
{
  plan_tests(1);

  ok1("Symtab");

  return exit_status();
}

