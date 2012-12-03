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
 *  | repstmt | forstmt | idstmt
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
int datatype;
extern FILE *target;

char programname[IDSIZE];

/**
 * L-Attribute de formalparm.
 */
int nparams;

/**
 * L-Attribute de function ou procedure.
 */
int current_module_pos;

/**
 * program -> PROGRAM ID ';' block '.' 
 */
void program(void)
{
  /**
   * Seta variável que exibe linha atual caso
   * aconteça algum erro ou alerta.
   */
  current_line = 1; 
  /** */

  match(PROGRAM);

  /**
   * Iremos salvar o nome do programa para ser utilizado na rótulo
   * da tradução do bloco principal de código.
   */ 
  strcpy(programname, lexeme);
  /** */

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
  /** tipo do simbolo 1: variavel */
  idtype = SYMTAB_IDTYPE_GLOBAL_VARIABLE;
  /** */

  declarations();
  modules();

  /** */
  gencode_set_main_entry_point("start"); 
  /** */

  stmtblock();

  /** */
  gencode_end_program();
  /** */

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

    do {
      /** */ 
      sympos = 0; 
      /** */
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
  /**
   * O objetivo desta função é adicionar no arquivo
   * de saída do compilador as funções que foram 
   * desenvolvidas diretamente em assembly e que 
   * podem ser utilizadas pelo desenvolvedor.
   * Como a função write.
   */
  builtin_write_functions(target);
  /** */
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
   * Adicionar procedimento na tabela de simbolos
   **/
  idtype = SYMTAB_IDTYPE_PROCEDURE;
  offset = 1;
  current_module_pos = symtab_insert(lexeme, 0, idtype, offset);
  /** */

  /** */
  gencode_procedure_start(lexeme);
  /** */

  match(ID);
  
  /** */ 
  sympos = 0; 
  offset = 4;
  idtype = SYMTAB_IDTYPE_PARAMETER;
  /** */

  formalparm();

  match(';');

  /** tipo do simbolo 6: variavel local */
  idtype = SYMTAB_IDTYPE_LOCAL_VARIABLE;
  offset = 0;
  /** */
  declarations();

  stmtblock();
  match(';');

  /** */
  gencode_procedure_end();
  symtab_dispose_local_variables();
  /** */
}

/**
 * function -> FUNCTION ID formalparm ':' typeid ';' block ';' 
 */
void function(void)
{
  match(FUNCTION);

  /** */
  offset = 1;
  idtype = SYMTAB_IDTYPE_FUNCTION;
  current_module_pos = symtab_insert(lexeme, 0, idtype, offset);
  /** */

  /** */
  gencode_function_start(lexeme);
  /** */

  match(ID);
  
  /** */ 
  sympos = 0; 
  offset = 4;
  idtype = SYMTAB_IDTYPE_PARAMETER;
  /** */
  formalparm();

  match(':');

  /** */
  sympos = 0;
  /** */

  type();

  /** */
  symtab_update_dtype(current_module_pos, dtype);
  symtab_update_nparams(current_module_pos, nparams);
  /** */

  match(';');

  /** tipo do simbolo 6: variavel local */
  idtype = SYMTAB_IDTYPE_LOCAL_VARIABLE;
  offset = 0;
  /** */
  declarations();

  stmtblock();
  match(';');

  /** */
  gencode_function_end();
  symtab_dispose_local_variables();
  /** */
}

/**
 * formalparm -> ['(' [VAR] idlist ':' type ')'
 *                {';' [VAR] idlist ':' type } ')']
 */
void formalparm(void)
{
  /** */
  nparams = 0;
  /** */

  if(lookahead == '(') {
    match('(');

    if(lookahead == VAR) match(VAR);
    idlist();
    match(':');
    type();

    while(lookahead == ';') {
      match(';');

      /** */ 
      sympos = 0; 
      /** */

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
 *  | repstmt | forstmt | idstmt
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
  int i, len;

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

      /** 
       * Recuperando tamanho do tipo do dado
       */

      len = data_len(dtype);
      if(idtype == SYMTAB_IDTYPE_GLOBAL_VARIABLE) {
        offset = 0;
        gencode_declare_global_var(symlist[i], len);
      } else {

        len = data_len(dtype);
        if(idtype == SYMTAB_IDTYPE_PARAMETER) {
          offset = offset + len;
        }
        else {
          offset = offset - len;
        }
      }
      symtab_insert(symlist[i], dtype, idtype, offset);

      if(idtype == SYMTAB_IDTYPE_PARAMETER)
        symtab_param_insert(current_module_pos, dtype);
    }
  }
  /** */
}

/**
 * idlist -> ID {',' ID} 
 */
void idlist(void)
{
  /** */
  strcpy(symlist[sympos++],lexeme);
  nparams++;
  /** */
  match(ID);
  while(lookahead == ',') {
    match(',');

    /** */
    strcpy(symlist[sympos++],lexeme);
    nparams++;
    /** */
    match(ID);
  }
}

/**
 * idstmt -> ID [ ASSGNMT expression | '(' exprlist ')' ]
 */
void idstmt(void) 
{
  int idaddress;
  char idlabel[IDSIZE];

  /** */
  if((idaddress = symtab_lookup(lexeme)) == 0) {
    fprintf(stderr, "symbol not declared: %s\n", lexeme);
  }

  /** 
   * L-Attribute: utilizado na checagem de tipos.
   */
  currenttype = symtab[idaddress][SYMTAB_COL_DATA_TYPE];

  strcpy(idlabel,lexeme);
  /** */

  match(ID);

  switch(lookahead) {
    case '(':
      match('(');
      exprlist();
      match(')');

      /** */
      gencode_callprocedure(idlabel);
      /** */

      break;
    case ASSGNMT:
      match(ASSGNMT);
      expression();

      /** */
      if(symtab[idaddress][SYMTAB_COL_OFFSET] == 0)
        gencode_global_assgnmt(idlabel);
      else
        gencode_local_assgnmt(symtab[idaddress][SYMTAB_COL_OFFSET]);
      /** */

      break;
  }
}

/**
 * ifstmt -> IF expression THEN stmt [ ELSE stmt ]
 */
void ifstmt(void)
{
  char label_if[100];
  char label_else[100];
  int lbl;

  match(IF);
  expression();
  match(THEN);

  /** */
  lbl = gencode_start_if_expression();
  sprintf(label_if, "IF%d", lbl);
  /** */

  stmt();

  if(lookahead == ELSE) {
    match(ELSE);

    /** */
    lbl = gencode_start_else_expression(lbl);
    sprintf(label_else, "ELSE%d", lbl);
    /** */

    stmt();

    /** */
    gencode_start_label(label_else);
    /** */
  } else {
    /** */
    gencode_start_label(label_if);
    /** */
  }

}

/**
 * whilestmt -> WHILE expression DO stmt
 */
void whilestmt(void)
{
  int lbl;

  match(WHILE);

  /** */
  lbl = gencode_start_while();
  /** */

  expression();

  match(DO);

  /** */
  gencode_start_do(lbl);
  /** */


  stmt();

  /** */
  gencode_end_while(lbl);
  /** */
}

/**
 * repstmt -> REPEAT stmtlist UNTIL expression
 */
void repstmt(void)
{
  int lbl;

  match(REPEAT);

  /** */
  lbl = gencode_start_repeat();
  /** */

  stmtlist();

  match(UNTIL);
  expression();

  /** */
  gencode_end_repeat(lbl);
  /** */

}

/*
 * expression -> expr = expr | expr '<' expr | 
 *               expr '>' expr | expr GEQ expr | 
 *               expr LEQ expr | expr NEQ expr | expr
 */
void expression(void)
{
  int op;

  /** */
  datatype = INTEGER;
  /** */
  expr();

  if(isrelationalop(lookahead)){
    match(op = lookahead);
    expr();

    /** */
    gencode_execute_relational(op);
    /** */

  }
}

/** 
 * expr -> [-] term { '+' term | '-' term | OR term }
 */
void expr(void)
{

  int neg = 0;
  int op;

  if(lookahead == '-') {
    match('-');

    /** */
    neg = 1;
    /** */
  }

  term();

  while(isaddop(lookahead)) {
    match(op = lookahead);
    term();

    /** */
    gencode_execute_add(op);
    /** */

  }

  /** */
  if(neg) gencode_neg();
  /** */
}

/** 
 * exprlist -> expression { ',' expression }
 */
void exprlist(void)
{
  /**
   * Contexto de passagem de parametros para procedimentos ou 
   * funções.
   */
  expression(); 
  /** */
  gencode_push_accumulator_onto_stack();
  /** */
  while(lookahead == ',') {
    match(',');
    expression();
    /** */
    gencode_push_accumulator_onto_stack();
    /** */
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
  int currenttype;
  char functionname[IDSIZE];

  switch(lookahead){
    case UINT:

      /** */
      if(((currenttype = typecheck(INTEGER, datatype)) == ERR_TYPE_INVALID)
          && (datatype != 0)){
        fprintf(stderr, "Invalid type\n");
      } else {
        datatype = currenttype;
      }
      gencode_uint_push(lexeme);
      /** */

      match(UINT);
      break;
    case FLOAT:

      /** */
      if(((currenttype = typecheck(FLOAT, datatype)) == ERR_TYPE_INVALID)
          && (datatype != 0)){
        fprintf(stderr, "Invalid type\n");
      } else {
        datatype = currenttype;
      }
      /** */

      match(FLOAT);
      break;
    case ID:
      if((symbol_entry = symtab_lookup(lexeme)) == 0){
        fprintf(stderr, 
               "in line %d\n"
               "semantic: symbol '%s' not found\n", current_line, lexeme);
      } else {
        switch(symtab[symbol_entry][SYMTAB_COL_IDENTIFIER_TYPE]) {
          case SYMTAB_IDTYPE_GLOBAL_VARIABLE:
          case SYMTAB_IDTYPE_LOCAL_VARIABLE:
          case SYMTAB_IDTYPE_FUNCTION:
          case SYMTAB_IDTYPE_PARAMETER:

            /** */
            if(((currenttype = typecheck(symtab[symbol_entry][SYMTAB_COL_DATA_TYPE], datatype)) == ERR_TYPE_INVALID)
                && (datatype != 0)){
              fprintf(stderr, "Invalid type");
            } else {
              datatype = currenttype;
            }
            /** */

            break;
          default:
            fprintf(stderr, 
                    "in line %d:\n"
                    "symbol in ilegal context\n", current_line);
        }
      }
      /** */
      strcpy(symbol, lexeme);
      /** */

      match(ID);

      if(lookahead == '('){
        /** 
         * iremos salvar o identificador da 
         * funcao para nao conflitar com as 
         * identificadores utilizados na
         * expressao.
         */
        strcpy(functionname, symbol);
        /** */

        match('(');
        exprlist();
        match(')');

        /** chamada de função **/
        gencode_callfunction(functionname);
        /** */

      }else {
        /** contexto de variavel simples **/
        /** **/
        offset = symtab[symbol_entry][SYMTAB_COL_OFFSET];
        if(offset == 0) {
          /** */
          gencode_global_var_push(symbol);
          /** */
        }else {
          /** */
          gencode_local_var_push(offset);
          /** */
        }
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
 * Função para calcular tamanho do tipo do dado.
 */
int data_len(int data_type)
{
  int len;

  switch(data_type) {
    case INTEGER:
    case BOOLEAN:
      len = 4;
      break;
    case REAL:
      len = 8;
      break;
  }

  return len;
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

