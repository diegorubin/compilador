#include <stdio.h>
#include "parser.h"

/** Topdown recursive parser
 * 
 * requires an LL(1) normalized grammar:
 */

/*
 * Expr -> Term R 
 */
void Expr()
{
  Term();
  R();
}

/*
 * R -> OPLUS Term R | "" 
 */
void R()
{
  if (lookahead == OPLUS) {
    match(OPLUS);
    Term();
    R();
  }
}

/*
 * Term -> Factor Q
 */
void Term()
{
  Factor();
  Q();
}
/*
 * Q -> OTIMES Factor Q | ""
 */
void Q()
{
  if (lookahead == OTIMES) {
    match(OTIMES);
    Factor();
    Q();
  }
}
/*
 * F -> NUM | OCTA | HEXA | ID | (E)
 */
void Factor()
{
  switch (lookahead) {
  case NUM:
  case OCTA:
  case HEXA:
  case ID:
    match(lookahead);
    break;
  default:
    match('(');
    Expr();
    match(')');
  }
}

