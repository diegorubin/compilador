#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define MEMSIZE 0x100000

int nextentry = 0;
char symbol[MEMSIZE][IDSIZE];
double memory[MEMSIZE];

double stack[MEMSIZE];
int sp = -1;

void execute_single(char op, double value)
{
  double result = 0;
  switch(op){
    case '-':
      result = - value;
      break;
  }
  push(result);
}

void execute_double(char op, double val1, double val2)
{
  double result = 0;
  switch(op){
    case '+':
      result = val1 + val2;
      break;
    case '-':
      result = val1 - val2;
      break;
    case '*':
      result = val1 * val2;
      break;
    case '/':
      result = val1 / val2;
      break;
  }
  push(result);
}

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

double recall(char *key)
{
  int address;
  if((address = lookup(key)) == -1){
    if(nextentry < MEMSIZE){
      memcpy(symbol[nextentry++],key,strlen(key)+1);
      return 0;
    }else{
      fprintf(stderr,"memory room space exceeded... exiting\n");
      exit(-5);
    }
  }
  return memory[address];
}

void push(double val)
{
  stack[++sp] = val;
}

double pop(void)
{
  return stack[sp--];
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
  /*match('\n');*/
  /** */{printf("%f\n", pop());}/** */;
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
  /** */{if(negate) execute_single('-', pop());}/** */

  while(lookahead == '+' || lookahead == '-') {
    match(oplus = lookahead);
    Term();

    /** */{execute_double(oplus,pop(),pop());}/** */
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
    Factor();
    
    /** */{execute_double(otimes,pop(),pop());}/** */
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
    /** */{push(atof(lexeme));}/** */
    match(lookahead);
    break;
  case ID:
    /** */{push(recall(lexeme));}/** */
    match(ID);
    break;
  default:
    match('(');
    Expr();
    match(')');
  }
}

