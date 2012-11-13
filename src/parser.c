/** Simplified Pascal in the EBNF
 * 
 * program -> PROGRAM ID ';' block '.'
 *
 * block -> declarations modules stmtblock
 * declarations -> 
 *   [CONST ID = constant ';' { ID = constant ';'}]
 *   [TYPE ID = type ';' { ID = type ';'}]
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *
 * modules -> {procedure | function}
 *
 * procedure -> PROCEDURE ID formalparm ';' declarations stmtblock ';' 
 *
 * function -> FUNCTION ID formalparm ':' type ';' declarations stmtblock ';' 
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
 *  | repstmt | forstmt | gotostmt | casestmt 
 *  | idstmt
 *
 * type -> INTEGER | REAL | BOOLEAN
 *
 * idlist -> ID {',' ID}
 *
 * idstmt -> ID [ ASSGNMT expression | '(' exprlist ')' ]
 *
 * ifstmt -> IF expression THEN stmt [ ELSE stmt ]
 *
 * whilestmt -> WHILE expression DO stmt
 *
 * repstmt -> REPEAT stmtlist UNTIL expression
 *
 * expression -> expr = expr | expr '<' expr | expr '>' expr |
 *               expr GEQ expr | expr LEQ expr | 
 *               expr NEQ expr | expr
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

int offset;
int dtype;
extern FILE *target;

char programname[IDSIZE];

/**
 * program -> PROGRAM ID ';' block '.' 
 */
void program(void)
{
  /**
   * Seta variavel que exibi linha atual caso
   * aconteça algum erro ou alerta.
   */
  current_line = 1; 
  /** */

  match(PROGRAM);

  /**
   * Iremos salvar o nome do programa para ser utilizado na label
   * da tradução do bloco principal de código.
   */ 
  strcpy(programname, lexeme);
  gencode_program("start"); 
  /** */

  match(ID);
  match(';');

  block();

  match('.');
  gencode_end_program();
}

/**
 * block -> declarations stmtblock 
 */
void block(void)
{
  declarations();
  modules();

  /** */
  gencode_block("start");
  /** */

  stmtblock();
}

/**
 * declarations -> 
 *   [VAR idlist ':' type';' {idlist ':' type ';'}]
 *   modules
 */
void declarations(void)
{
  if(lookahead == VAR) {

    match(VAR);

    /** symbol type 1: variable **/
    idtype = SYMTAB_IDTYPE_VARIABLE;

    do {
      /** */ sympos = 0; /** */
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
  builtin_write_functions(target);
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

  /** 
   * Add to symtab 
   **/
  idtype = SYMTAB_IDTYPE_PROCEDURE;
  offset = 0;
  symtab_insert(lexeme, 0, idtype, offset);
  /** */

  match(ID);
  
  /** */ sympos = 0; /** */

  formalparm();

  match(';');

  declarations();
  stmtblock();
  match(';');

  symtab_dispose_local_variables();
}

/**
 * function -> FUNCTION ID formalparm ':' typeid ';' block ';' 
 */
void function(void)
{
  int pos;
  match(FUNCTION);

  /** */
  offset = 0;
  idtype = SYMTAB_IDTYPE_FUNCTION;
  pos = symtab_insert(lexeme, 0, idtype, offset);
  /** */

  match(ID);
  
  /** */ 
  sympos = 0; 
  idtype = SYMTAB_IDTYPE_PARAMETER;
  /** */
  formalparm();

  match(':');

  /** */
  sympos = 0;
  /** */

  type();

  /** */
  symtab_update_dtype(pos,dtype);
  /** */

  match(';');

  declarations();
  stmtblock();
  match(';');

  symtab_dispose_local_variables();
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
 * stmt -> 
 *    stmtblock | ifstmt | whilestmt
 *  | repstmt | forstmt | gotostmt | casestmt
 *  | idstmt
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
      break;
  }
}

/**
 * type -> INTEGER | REAL | BOOLEAN
 */
void type(void)
{
  /** datatype **/
  int i;

  switch(lookahead) {
    case INTEGER:
      match(dtype = INTEGER);
      break;
    case REAL:
      match(dtype = REAL);
      break;
    case BOOLEAN:
      match(dtype = BOOLEAN);
  }
  /** */
  for(i = 0; i < sympos; i++) {

    if(symtab_lookup(symlist[i])){
      fprintf(stderr, 
          "in line %d:\n"
          "symbol \"%s\" already declared\n",current_line, symlist[i]);
    } else {
      symtab_insert(symlist[i], dtype, idtype, offset);
    }
  }
  /** */
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

/**
 * idstmt -> ID [ ASSGNMT expression | '(' exprlist ')' ]
 */
void idstmt(void) 
{
  int idaddress;

  /** */
  if((idaddress = symtab_lookup(lexeme) == 0)) {
    fprintf(stderr, "variable not declared: %s\n", lexeme);
  }
  currenttype = symtab[idaddress][SYMTAB_COL_DATA_TYPE];
  /** */

  match(ID);

  switch(lookahead) {
    case '(':
      match('(');
      expression();
      match(')');
      break;
    case ASSGNMT:
      match(ASSGNMT);
      expression();
      break;
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
 *               expr LEQ expr | expr NEQ expr | expr
 */
void expression(void)
{
  /** an inherited attribute is get from the left side of
   * an assignment if any **/
  expr();
  if(isrelationalop(lookahead)){
    match(lookahead);
    expr();
  }
  /** at this point we get a resulting data type as a 
   * synthesized attribute **/
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
 * exprlist -> expression { ',' expression }
 */
void exprlist(void)
{
  expression(); 
  
  /** expression result stored in accumulator **/
  /** push the accumulator onto the stack **/

  while(lookahead == ',') {
    match(',');
    expression();

    /** expression result stored in accumulator **/
    /** push the accumulator onto the stack **/
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
      /** in this context the symbol must be a 
       * variable or a function using symtab_lookup call */

      /** */
      if((symbol_entry = symtab_lookup(lexeme)) == 0){
        fprintf(stderr, 
               "in line %d\n"
               "semantic: symbol '%s' not found\n", current_line, lexeme);
      } else {
        switch(symtab[symbol_entry][SYMTAB_COL_IDENTIFIER_TYPE]) {
          case SYMTAB_IDTYPE_VARIABLE:
          case SYMTAB_IDTYPE_FUNCTION:
          case SYMTAB_IDTYPE_PARAMETER:
          case SYMTAB_IDTYPE_PROCEDURE:
            break;
          default:
            fprintf(stderr, "symbol in ilegal context");
        }
      }
      strcpy(symbol, lexeme);
      /** */

      match(ID);

      if(lookahead == '('){
        /** this is context for function call **/
        match('(');
        exprlist();
        match(')');
        /** subroutine call is here **/
      }else {
        /** this is context for simple variable **/
        /** **/
        offset = symtab[symbol_entry][SYMTAB_COL_OFFSET];
        if(offset == 0) {
          /** the variable is global, then its identification
           * itself is the variable address **/
          /** **/
          //fprintf(target, "\tmov %s, %%eax\n", symbol);
          /** **/
        }else {
          /** in this context all symbols are looked up in the
           * symbol table in order to compose the stack address:
           *  <offset>(%ebp)
           * 
           * for instance, a variable at offset -4 would be written as
           * -4(%ebp)
           *
           * a second variable with size 4 would be translated to 
           * -8(%ebp)
           *
           * on the other hand, a parameter in the first slot would be
           * interpreted as
           * 8(%ebp)
           *
           **/
          /** **/
          fprintf(target, "\tmov %d(%%ebp), %%eax\n", offset);
          /** **/
        }
      }
      break;
    case '(':
      match('(');
      expression();
      /** at this point, expression has synthesized an
       * attribute storing the resulting data type in the
       * expression. however, the just calculated attribute
       * might be greater than the l-attribute along the current
       * expression, so that a promotion must occur.
       **/
      match(')');
      break;
    default:
      match(NOT); factor();
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

/**
 * Função para checagem de tipos
 */
int typecheck(int type1, int type2)
{
	if (type1 == INTEGER && type2 == INTEGER) return INTEGER;
	else if((type1 == REAL && type2 == INTEGER) ||
	        (type1 == INTEGER && type2 == REAL) || 
	        (type1 == REAL && type2 == REAL)) return REAL;
	else if (type1 == BOOLEAN && type2 == BOOLEAN) return BOOLEAN;
	else return ERR_TYPE_INVALID;
}

/**
 * Funções de debug
 */
int printtape(int len) 
{
  int j;
  for(j = 0; j < len; j++) {
    if(lextape[j])
      printf("%c", lextape[j]);
    else
      printf("0");
  }
  printf("\n");
}

