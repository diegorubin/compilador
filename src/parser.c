/** Simplified Pascal in the EBNF
 * 
 * program -> PROGRAM ID ';' block '.'
 *
 * block -> declarations stmtblock
 * declartions -> 
 *   [CONST ID = constant ';' { ID = constant ';'}]
 *   [TYPE ID = type ';' { ID = type ';'}]
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *   modules
 *
 * modules -> {procedure | function}
 *
 * procedure -> PROCEDURE ID formalparm ';' block ';' 
 *
 * function -> FUNCTION ID formalparm ':' typeid ';' block ';' 
 *
 * stmtblock -> BEGIN stmtlist END
 *
 * stmtlist -> stmt {';' stmt}
 *
 * stmt -> 
 *    stmtblock | ifstmt | whilestmt
 *  | repstmt | forstmt | gotostmt | casestmt | idstmt
 */

/** further simplifacations for today only:
 * program -> PROGRAM ID ';' block '.'
 *
 * block -> declarations stmtblock
 *
 * declartions -> 
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *   modules
 *
 * modules -> {procedure | function}
 *
 * procedure -> PROCEDURE ID formalparm ';' block ';' 
 *
 * function -> FUNCTION ID formalparm ':' typeid ';' block ';' 
 *
 * stmtblock -> BEGIN stmtlist END
 *
 * stmtlist -> stmt {';' stmt}
 *
 * stmt -> 
 *    stmtblock | ifstmt | whilestmt | repstmt | idstmt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/* program -> PROGRAM ID ';' block '.' */
void program(void)
{
  match(PROGRAM);
  match(ID);
  match(';');

  block();

  match('.');
}

/* block -> declarations stmtblock */
void block(void)
{
  declarations();
  stmtblock();
}

/* declartions -> 
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *   modules
 */
void declarations(void)
{
  if(lookahead == VAR) {
    match(VAR);

// XXX: trocar por um do while
rep_idlist:
    idlist();
    match(':');
    
    type();
    match(';');
    if(lookahead == ID) goto rep_idlist;
  }
}

void modules(void) 
{
  while(lookahead == PROCEDURE || lookahead == FUNCTION) {
    if(lookahead == PROCEDURE) procedure();
    else function();
  }
}

void procedure(void)
{
  match(PROCEDURE);

  match(ID);
  formalparm();

  match(';');

  block();
  match(';');

}

void function(void)
{
  match(FUNCTION);

  match(ID);
  formalparm();

  match(':');
  type();

  match(';');

  block();
  match(';');

}

void idlist(void)
{
  match(ID);
  while(lookahead == ',') {
    match(',');
    match(ID);
  }
}

void type(void)
{
  switch(lookahead) {
    case INTEGER:
      match(INTEGER);
      break;
    case REAL:
      match(REAL);
  }
}

void stmtblock(void) 
{
  match(BEGIN);
  stmtlist();
  match(END);
}

void stmtlist(void) 
{
  stmt();
  while(lookahead == ';'){
    match(';');
    stmt();
  }
}

void ifstmt(void)
{
  match(IF);
  expression();
  match(THEN);

  stmt();

  if(lookahead == ELSE) {
    match(ELSE);
    stmt();
  }

}

isrelationalop(token_t token)
{
  switch(token) {
    case '=':
    case '<':
    case '>':
    case GEQ:
    case LEQ:
    case NEQ:
      return token;
  }
  return 0;
}

isaddop(token_t token)
{
  switch(token) {
    case '+':
    case '-':
    case OR:
      return token;
  }
  return 0;
}

ismulop(token_t token)
{
  switch(token) {
    case '*':
    case '/':
    case AND:
    case DIV:
    case MOD:
      return token;
  }
  return 0;
}

void expression(void)
{
  expr();
  if(isrelationalop(lookahead)){
    match(lookahead);
    expr();
  }
}

void expr(void)
{
  if(lookahead == '-') match('-');
  term();
  while(isaddop(lookahead)) {
    match(lookahead);
    term();
  }
}

void term(void)
{
  factor();
  while(ismulop(lookahead)) {
    match(lookahead);
    factor();
  }
}

void factor(void) 
{
  switch(lookahead){
    case UINT:
      match(UINT);
      break;
    case FLOAT:
      match(FLOAT);
      break;
    case ID:
      match(ID);
      if(lookahead == '('){
        match('(');
        exprlist();
        match(')');
      }
      break;
    case '(':
      match('(');
      expression();
      match(')');
      break;
    default:
      match(NOT); factor();
  }
}

void exprlist(void)
{
  expression();
  while(lookahead == ',') {
    match(',');
    expression();
  }
}

void stmt(void)
{
  switch(lookahead){
    case BEGIN:
      stmtblock();
      break;
    case IF:
      ifstmt();
      break;
    case WHILE:
      whilestmt();
      break;
    case REPEAT:
      repstmt();
      break;
    case ID:
      idstmt();
  }
}
