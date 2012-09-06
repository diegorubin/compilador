#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define MEMSIZE 0x100000

int nextentry = 0;
char symbol[MEMSIZE][IDSIZE];
double memory[MEMSIZE];

lookup(char *key)
{
  int i;
  for(i = 0; i < nextentry; i++){
    if(strcmp(key, symbol[i]) == 0) return i;
  }
  return -1;
}

double store(double val, char *key)
{
  int address;
  if((address = lookup(key)) == -1){
    if(nextentry < MEMSIZE){
      strcpy(symbol[address=nextentry++], key);
    }else{
      fprintf(stderr,"memory room space exceeded... exiting\n");
      exit(-5);
    }
  }
  return memory[address] = val;
}

/** Topdown recursive parser
 * 
 * requires an LL(1) normalized grammar:
 */

/*
 * mybc -> Expr() <enter>
 */
void mybc(void)
{
  Expr();
  match('\n');
}

/*
 * Expr -> [-] Term {plus Term} 
 */
void Expr(void)
{
  int negate = 0;
  int oplus = 0;

  /** */if(negate = (lookahead == '-')) match('-');/** */
  Term();
  /** */{negate && printf(" %c ", '-');}/** */

  while(lookahead == '+' || lookahead == '-') {
    match(oplus = lookahead);
    Term();

    /** */{printf(" %c ", oplus);}/** */
  }
}

/*
 * Term -> Factor {times Factor}
 */
void Term(void)
{
  int otimes = 0;

  Factor();
  while(lookahead == OTIMES){
    otimes = lexeme[0];
    match(OTIMES); 
    Factor();/** */{printf(" %c ", otimes);}/** */
  }
}
/*
 * Factor -> NUM | OCTA | HEXA | ID | FLOAT | (E)
 */
void Factor()
{
  switch (lookahead) {
  case NUM:
  case FLOAT:
  case OCTA:
  case HEXA:
    /** */{printf(" %s ", lexeme);}/** */
    match(lookahead);
    break;
  case ID:
    /** */{printf(" %s ", lexeme);}/** */
    match(ID);
    break;
  default:
    match('(');
    Expr();
    match(')');
  }
}

