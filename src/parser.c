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
 *
 * type -> INTEGER | REAL
 *
 * idlist -> ID {',' ID}
 *
 * ifstmt -> IF expression THEN stmt [ ELSE stmt ]
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
    match(VAR);

    do {
      idlist();
      match(':');
      
      type();
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
  match(PROCEDURE);

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
  match(FUNCTION);

  match(ID);
  formalparm();

  match(':');
  type();

  match(';');

  block();
  match(';');

}

/**
 * idlist -> ID {',' ID} 
 */
void idlist(void)
{
  match(ID);
  while(lookahead == ',') {
    match(',');
    match(ID);
  }
}

/**
 * type -> INTEGER | REAL 
 */
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

