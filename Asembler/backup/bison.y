%{
/*
%require "3.2"
%language "c++"
*/
#include <stdio.h>
#include "niz.h"
extern int yylex();
extern FILE* yyin;

void yyerror(const char* message);

int niz_counter = 0;
%}

%union {
    int intValue;
    char* stringValue;
}

%token <stringValue> IDENT DIR SYM_CONST HEX DEC LIT_HEX LIT_DEC REG BR_OPEN BR_CLOSE COMMA COLON PLUS MINUS EOL DBL_QUOTES
%token <stringValue> HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR
%token <stringValue> GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END

%start program

%%

program:          statementList
                  ;

statementList:    statement
                  | statementList statement
                  ;

statement:        label directive EOL
                  |
                  label EOL
                  ;

label:          IDENT COLON
                |
                ;

directive:      GLOBAL symbol_list
                |
                EXTERN symbol_list
                |
                SECTION IDENT {printf("naziv sekcije: %s\n", yylval.stringValue); Token t = {.type=34}; memcpy(tokens[niz_counter].text, yylval.stringValue, sizeof(yylval.stringValue)); tokens[niz_counter++] = t;}
                |
                WORD sym_lit_list
                |
                SKIP DEC
                |
                SKIP HEX
                |
                ASCII DBL_QUOTES IDENT DBL_QUOTES
                |
                EQU IDENT COMMA expr
                ;

symbol_list:    IDENT
                |
                IDENT COMMA symbol_list
                ;

sym_lit_list:   sym_or_lit
                |
                sym_or_lit COMMA sym_lit_list
                ;

sym_or_lit:     IDENT
                |
                HEX
                |
                DEC
                ;

expr:           sym_or_lit
                |
                sym_or_lit PLUS expr
                |
                sym_or_lit MINUS expr
                ;            



%%

void yyerror(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    // Handle error as per your requirement
}
