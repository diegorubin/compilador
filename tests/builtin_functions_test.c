#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <builtin_functions.h>
#include "tap.h"

int main(int argc, char **argv)
{
  plan_tests(1);
  ok1("pending.");

  return exit_status();
}

