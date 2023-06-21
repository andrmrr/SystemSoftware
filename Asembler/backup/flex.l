%option noyywrap

%{
#include <stdio.h>
#include "bison.tab.h"
%}


digit [0-9]
hex_digit [0-9a-fA-F]
letter [a-zA-Z]


%x COMMENT
%x DIR

%%

"#"                                     {BEGIN(COMMENT);}
<COMMENT>.                              {}
<COMMENT>\n                             {BEGIN(INITIAL); return EOL;}

[ \t\r]                                 {}


"."                                     {BEGIN(DIR);}
<DIR>"global"                           {BEGIN(INITIAL); return GLOBAL;}
<DIR>"extern"                           {BEGIN(INITIAL); return EXTERN;}
<DIR>"section"                          {BEGIN(INITIAL); return SECTION;}
<DIR>"word"                             {BEGIN(INITIAL); return WORD;}
<DIR>"skip"                             {BEGIN(INITIAL); return SKIP;}
<DIR>"ascii"                            {BEGIN(INITIAL); return ASCII;}
<DIR>"equ"                              {BEGIN(INITIAL); return EQU;}
<DIR>"end"                              {BEGIN(INITIAL); return END;}


"halt"                                  {return HALT;}
"int"                                   {return INT;}
"iret"                                  {return IRET;}
"call"                                  {return CALL;}
"ret"                                   {return RET;}
"jmp"                                   {return JMP;}
"beq"                                   {return BEQ;}
"bne"                                   {return BNE;}
"bgt"                                   {return BGT;}
"push"                                  {return PUSH;}
"pop"                                   {return POP;}
"xchg"                                  {return XCHG;}
"add"                                   {return ADD;}
"sub"                                   {return SUB;}
"mul"                                   {return MUL;}
"div"                                   {return DIV;}
"not"                                   {return NOT;}
"and"                                   {return AND;}
"or"                                    {return OR;}
"xor"                                   {return XOR;}
"shl"                                   {return SHL;}
"shr"                                   {return SHR;}
"ld"                                    {return LD;}
"st"                                    {return ST;}
"csrrd"                                 {return CSRRD;}
"csrwr"                                 {return CSRWR;}


{letter}({letter}|{digit}|_)*           {yylval.stringValue = yytext; return IDENT;}
"$"{letter}({letter}|{digit})*          {return SYM_CONST;}
0x{hex_digit}+                          {return HEX;}
{digit}+                                {return DEC;}
"$"0x{hex_digit}+                       {return LIT_HEX;}
"$"{digit}+                             {return LIT_DEC;}
"%"r{digit}{1,2}                        {return REG;}
"%"sp                                   {return REG;}
"%"pc                                   {return REG;}
"["                                     {return BR_OPEN;}
"]"                                     {return BR_CLOSE;}
","                                     {return COMMA;}
":"                                     {return COLON;}
"+"                                     {return PLUS;}
"-"                                     {return MINUS;}
"\""                                    {return DBL_QUOTES;}
\n                                      {return EOL;}




%%

