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
 * formalparm -> ['(' [VAR] idlist ':' type 
 *                {';' [VAR] idlist ':' type } ')']
 *
 * stmtblock -> BEGIN stmtlist END
 *
 * stmtlist -> stmt {';' stmt}
 *
 * stmt -> 
 *    stmtblock | ifstmt | whilestmt
 *  | repstmt | forstmt | gotostmt | casestmt | idstmt
 *
 * type -> INTEGER | REAL
 *
 * idlist -> ID {',' ID}
 *
 * idstmt -> ID
 *
 * ifstmt -> IF expression THEN stmt [ ELSE stmt ]
 *
 * whilestmt -> WHILE expression DO stmt
 *
 * repstmt -> REPEAT stmtlist UNTIL expression
 *
 * expression -> expr = expr | expr < expr | expr > expr |
 *               expr GEQ expr | expr LEQ expr | 
 *               expr NEQ expr 
 *
 * expr -> [-] term { '+' term | '-' term | OR term }
 *
 * exprlist -> expression { ',' expression }
 *
 * term -> factor { '*' factor | '/' factor | AND factor | 
 *                  DIV factor | MOD factor }
 *
 * factor -> UINT | FLOAT | ID {'(' exprlist ')'} | 
 *           '(' expression ')' | NOT factor
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/**
 * program -> PROGRAM ID ';' block '.' 
 */
void program(void)
{
  match(PROGRAM);
  match(ID);
  match(';');

  block();

  match('.');
}

/**
 * block -> declarations stmtblock 
 */
void block(void)
{
  declarations();
  stmtblock();
}

/**
 * declartions -> 
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *   modules
 */
void declarations(void)
{
  if(lookahead == VAR) {
    match(VAR);/** symbol type 1: variable **/

    /** */ sympos = 0; /** */

    do {
      idlist();/** produce one symbol list **/
      match(':');
      
      type();/** to be used here **/
      match(';');
    } while(lookahead == ID);
  }
}

/**
 * modules -> {procedure | function} 
 */
void modules(void) 
{
  while(lookahead == PROCEDURE || lookahead == FUNCTION) {
    if(lookahead == PROCEDURE) procedure();
    else function();
  }
}

/**
 * procedure -> PROCEDURE ID formalparm ';' block ';' 
 */
void procedure(void)
{
  match(PROCEDURE);/** symbol type 3:  procedure**/

  match(ID);
  formalparm();

  match(';');

  block();
  match(';');

}

/**
 * function -> FUNCTION ID formalparm ':' typeid ';' block ';' 
 */
void function(void)
{
  match(FUNCTION);/** symbol type 4:  function**/

  match(ID);
  formalparm();

  match(':');
  type();

  match(';');

  block();
  match(';');

}

/**
 * formalparm -> ['(' [VAR] idlist ':' type ')'
 *                {';' [VAR] idlist ':' type } ')']
 */
void formalparm(void)
{
  if(lookahead == '(') {
    match('(');

    if(lookahead == VAR) match(VAR);
    idlist();
    match(':');
    type();

    while(lookahead == ';') {
      if(lookahead == VAR) match(VAR);
      idlist();
      match(':');
      type();
    }

    match(')');
  }
}

/**
 * idlist -> ID {',' ID} 
 */
void idlist(void)
{
  /** */strcpy(symlist[sympos++],lexeme);/** */
  match(ID);
  while(lookahead == ',') {
    match(',');

    /** */strcpy(symlist[sympos++],lexeme);/** */
    match(ID);
  }
}

void idstmt(void) 
{
  match(ID);
}

/**
 * type -> INTEGER | REAL 
 */
void type(void)
{
  /** datatype **/
  int dtype, i;

  switch(lookahead) {
    case INTEGER:
      match(dtype = INTEGER);
      break;
    case REAL:
      match(dtype = REAL);
  }
  /** */
  for(i = 0; i < sympos; i++) {
    if(symtab_lookup(symlist[i])){
      fprintf(stderr, "symbol \"%s\" already declared\n", symlist[i]);
    } else {
      symtab_insert(symlist[i], dtype);
    }
  }
  /** */
}

/**
 * stmtblock -> BEGIN stmtlist END
 */
void stmtblock(void) 
{
  match(BEGIN);
  stmtlist();
  match(END);
}

/**
 * stmtlist -> stmt {';' stmt}
 */
void stmtlist(void) 
{
  stmt();
  while(lookahead == ';'){
    match(';');
    stmt();
  }
}

/**
 * ifstmt -> IF expression THEN stmt [ ELSE stmt ]
 */
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

/**
 * whilestmt -> WHILE expression DO stmt
 */
void whilestmt(void)
{
  match(WHILE);
  expression();
  match(DO);

  stmt();
}

/**
 * repstmt -> REPEAT stmtlist UNTIL expression
 */
void repstmt(void)
{
  match(REPEAT);

  stmtlist();

  match(UNTIL);
  expression();
}

/*
 * expression -> expr = expr | expr '<' expr | 
 *               expr '>' expr | expr GEQ expr | 
 *               expr LEQ expr | expr NEQ expr 
 */
void expression(void)
{
  expr();
  if(isrelationalop(lookahead)){
    match(lookahead);
    expr();
  }
}

/** 
 * expr -> [-] term { '+' term | '-' term | OR term }
 */
void expr(void)
{
  if(lookahead == '-') match('-');
  term();
  while(isaddop(lookahead)) {
    match(lookahead);
    term();
  }
}

/** 
 * term -> factor { '*' factor | '/' factor | AND factor | 
 *                  DIV factor | MOD factor }
 */
void term(void)
{
  factor();
  while(ismulop(lookahead)) {
    match(lookahead);
    factor();
  }
}

/** 
 * factor -> UINT | FLOAT | ID {'(' exprlist ')'} | 
 *           '(' expression ')' | NOT factor
 */
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

/** 
 * exprlist -> expression { ',' expression }
 */
void exprlist(void)
{
  expression();
  while(lookahead == ',') {
    match(',');
    expression();
  }
}

/**
 * stmt -> 
 *    stmtblock | ifstmt | whilestmt
 *  | repstmt | forstmt | gotostmt | casestmt | idstmt
 */
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

/**
 * Funções de auxilio
 */

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

