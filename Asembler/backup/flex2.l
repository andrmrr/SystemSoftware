"#"                                     {BEGIN(COMMENT);}
<COMMENT> [^\n]                         {}
<COMMENT> \n                            {BEGIN(INITIAL); return EOL;}




%option noyywrap

%{
#include <stdio.h>
%}

%{
enum TokenType {
    IDENTIFIER,
    DECIMAL,
    HEX_NUMBER,
    LABEL
};
%}

DIGIT       [0-9]
HEX_DIGIT   [0-9a-fA-F]
LETTER      [a-zA-Z]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
DECIMAL     {DIGIT}+
HEX_NUMBER  0x{HEX_DIGIT}+
LABEL       {IDENTIFIER}:

%%

{IDENTIFIER}    { return IDENTIFIER; }
{DECIMAL}       { return DECIMAL; }
{HEX_NUMBER}    { return HEX_NUMBER; }
{LABEL}         { return LABEL; }
.               { /* Ignore unrecognized characters */ }

%%

int main() {
    int token;
    while ((token = yylex()) != 0) {
        switch (token) {
            case IDENTIFIER:
                printf("IDENTIFIER: %s\n", yytext);
                break;
            case DECIMAL:
                printf("DECIMAL: %s\n", yytext);
                break;
            case HEX_NUMBER:
                printf("HEX_NUMBER: %s\n", yytext);
                break;
            case LABEL:
                printf("LABEL: %s\n", yytext);
                break;
            default:
                printf("Unrecognized token.\n");
                break;
        }
    }
    return 0;
}
