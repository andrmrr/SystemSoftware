
%option header-file
%option token-type=Token

%top{
  #include <stdio.h>
  #include "tokens.h"
%}

%class{

%}


digit [0-9]
hex_digit [0-9a-fA-F]
letter [a-zA-Z]


%x COMMENT
%x DIR

%%

"#"                                     {start(COMMENT);}
<COMMENT>.                              {}
<COMMENT>\n                             {start(INITIAL); return Token(TokenType::EOL, str());}


[ \t\r]                                 {}


"."                                     {start(DIR);}
<DIR>"global"                           {start(INITIAL); return Token(TokenType::GLOBAL, str());}
<DIR>"extern"                           {start(INITIAL); return Token(TokenType::EXTERN, str());}
<DIR>"section"                          {start(INITIAL); return Token(TokenType::SECTION, str());}
<DIR>"word"                             {start(INITIAL); return Token(TokenType::WORD, str());}
<DIR>"skip"                             {start(INITIAL); return Token(TokenType::SKIP, str());}
<DIR>"ascii"                            {start(INITIAL); return Token(TokenType::ASCII, str());}
<DIR>"equ"                              {start(INITIAL); return Token(TokenType::EQU, str());}
<DIR>"end"                              {start(INITIAL); return Token(TokenType::END, str());}


"halt"                                  {return Token(TokenType::HALT, str());}
"int"                                   {return Token(TokenType::GLOBAL, str());}
"iret"                                  {return Token(TokenType::GLOBAL, str());}
"call"                                  {return Token(TokenType::GLOBAL, str());}
"ret"                                   {return Token(TokenType::GLOBAL, str());}
"jmp"                                   {return Token(TokenType::GLOBAL, str());}
"beq"                                   {return Token(TokenType::GLOBAL, str());}
"bne"                                   {return Token(TokenType::GLOBAL, str());}
"bgt"                                   {return Token(TokenType::GLOBAL, str());}
"push"                                  {return Token(TokenType::GLOBAL, str());}
"pop"                                   {return Token(TokenType::GLOBAL, str());}
"xchg"                                  {return Token(TokenType::GLOBAL, str());}
"add"                                   {return Token(TokenType::GLOBAL, str());}
"sub"                                   {return Token(TokenType::GLOBAL, str());}
"mul"                                   {return Token(TokenType::GLOBAL, str());}
"div"                                   {return Token(TokenType::GLOBAL, str());}
"not"                                   {return Token(TokenType::GLOBAL, str());}
"and"                                   {return Token(TokenType::GLOBAL, str());}
"or"                                    {return Token(TokenType::GLOBAL, str());}
"xor"                                   {return Token(TokenType::GLOBAL, str());}
"shl"                                   {return Token(TokenType::GLOBAL, str());}
"shr"                                   {return Token(TokenType::GLOBAL, str());}
"ld"                                    {return Token(TokenType::GLOBAL, str());}
"st"                                    {return Token(TokenType::GLOBAL, str());}
"csrrd"                                 {return Token(TokenType::GLOBAL, str());}
"csrwr"                                 {return Token(TokenType::GLOBAL, str());}


{letter}({letter}|{digit}|_)*           {return Token(TokenType::IDENT, str());}
"$"{letter}({letter}|{digit})*          {return Token(TokenType::SYM_CONST, str());}
0x{hex_digit}+                          {return Token(TokenType::HEX, str());}
{digit}+                                {return Token(TokenType::DEC, str());}
"$"0x{hex_digit}+                       {return Token(TokenType::LIT_HEX, str());}
"$"{digit}+                             {return Token(TokenType::LIT_DEC, str());}
"%"r{digit}{1,2}                        {return Token(TokenType::REG, str());}
"%"sp                                   {return Token(TokenType::REG, str());}
"%"pc                                   {return Token(TokenType::REG, str());}
"["                                     {return Token(TokenType::BR_OPEN, str());}
"]"                                     {return Token(TokenType::BR_CLOSE, str());}
","                                     {return Token(TokenType::COMMA, str());}
":"                                     {return Token(TokenType::COLON, str());}
"+"                                     {return Token(TokenType::PLUS, str());}
"-"                                     {return Token(TokenType::MINUS, str());}
"\""                                    {return Token(TokenType::DBL_QUOTES, str());}
\n                                      {return Token(TokenType::EOL, str());}




%%

