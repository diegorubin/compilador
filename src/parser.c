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
