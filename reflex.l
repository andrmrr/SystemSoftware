
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
%x STRING

%%

"#"                                     {start(COMMENT);}
<COMMENT>.                              {}
<COMMENT>\n                             {start(INITIAL); return Token(TokenType::EOL, str());}

"\""                                    {start(STRING);}
<STRING>[^"\n]+                         {return Token(TokenType::STRING, str());}
<STRING>"\""                            {start(INITIAL);}
<STRING>\n                              {start(INITIAL); return Token(TokenType::ERROR, str());}

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
"int"                                   {return Token(TokenType::INT, str());}
"iret"                                  {return Token(TokenType::IRET, str());}
"call"                                  {return Token(TokenType::CALL, str());}
"ret"                                   {return Token(TokenType::RET, str());}
"jmp"                                   {return Token(TokenType::JMP, str());}
"beq"                                   {return Token(TokenType::BEQ, str());}
"bne"                                   {return Token(TokenType::BNE, str());}
"bgt"                                   {return Token(TokenType::BGT, str());}
"push"                                  {return Token(TokenType::PUSH, str());}
"pop"                                   {return Token(TokenType::POP, str());}
"xchg"                                  {return Token(TokenType::XCHG, str());}
"add"                                   {return Token(TokenType::ADD, str());}
"sub"                                   {return Token(TokenType::SUB, str());}
"mul"                                   {return Token(TokenType::MUL, str());}
"div"                                   {return Token(TokenType::DIV, str());}
"not"                                   {return Token(TokenType::NOT, str());}
"and"                                   {return Token(TokenType::AND, str());}
"or"                                    {return Token(TokenType::OR, str());}
"xor"                                   {return Token(TokenType::XOR, str());}
"shl"                                   {return Token(TokenType::SHL, str());}
"shr"                                   {return Token(TokenType::SHR, str());}
"ld"                                    {return Token(TokenType::LD, str());}
"st"                                    {return Token(TokenType::ST, str());}
"csrrd"                                 {return Token(TokenType::CSRRD, str());}
"csrwr"                                 {return Token(TokenType::CSRWR, str());}


{letter}({letter}|{digit}|_)*           {return Token(TokenType::IDENT, str());}
{letter}({letter}|{digit}|_)*":"        {return Token(TokenType::LABEL, str());}
"$"{letter}({letter}|{digit})*          {return Token(TokenType::SYM_CONST, str());}
0x{hex_digit}+                          {return Token(TokenType::HEX, str());}
{digit}+                                {return Token(TokenType::DEC, str());}
"$"0x{hex_digit}+                       {return Token(TokenType::LIT_HEX, str());}
"$"{digit}+                             {return Token(TokenType::LIT_DEC, str());}
"%"status                               {return Token(TokenType::STATUS, str());}
"%"handler                              {return Token(TokenType::HANDLER, str());}
"%"cause                                {return Token(TokenType::CAUSE, str());}
"%"r{digit}                             {return Token(TokenType::GPR, str());}
"%"r1[0-5]                              {return Token(TokenType::GPR, str());}
"%"sp                                   {return Token(TokenType::GPR, str());}
"%"pc                                   {return Token(TokenType::GPR, str());}
"["                                     {return Token(TokenType::BR_OPEN, str());}
"]"                                     {return Token(TokenType::BR_CLOSE, str());}
","                                     {return Token(TokenType::COMMA, str());}
":"                                     {return Token(TokenType::COLON, str());}
"+"                                     {return Token(TokenType::PLUS, str());}
"-"                                     {return Token(TokenType::MINUS, str());}
\n                                      {return Token(TokenType::EOL, str());}

.                                      {return Token(TokenType::ERROR, str());}


%%

