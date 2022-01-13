%{
#include <stdlib.h>
#include "parser.tab.h"

/* variáveis de estado (globais)
   yylval : valor de um token (int)
*/



%}
%option noyywrap

alpha        [_a-zA-Z]
digit        [0-9]

%%

[ \t\r]+                       /* skip whitespace */

"let"                          { return LET; }
[ \n\t\r]*"in"[ \n\t\r]*                            { return IN; }
[ \n\t\r]*"var"                          { return VAR; }


"if"                           { return IF; }
[ \n]*"else"[ \n]*               { return ELSE; }
[ \n]*"then"[ \n]*               { return THEN; }  
"while"                        { return WHILE; }
[ \n]*"do"[ \n]*                 { return DO; }
"scani()"                      { return SCANI; }
"printi"                       { return PRINTI; }

{digit}+                       { yylval.ival = atoi(yytext); return TOK_NUM; }
({alpha}+{digit}*)+            { yylval.sval = (char*)malloc(100*sizeof(char)); strcpy(yylval.sval, yytext); return TOK_CHAR; }


"+"                            { return '+'; }
"*"                            { return '*'; }
"/"                            { return '/'; }
"%"                            { return '%'; }
"-"                            { return '-'; }


">"                            { return '>'; }
"<"                            { return '<'; }
"="                            { return '='; }


"("                            { return '('; }
")"                            { return ')'; }


":"                            { return ':'; }
";"[ \n\t\r]*                            { return ';'; }
"\n"                           { return T_NEWLINE; }
<<EOF>>                        { return EOF; }

[ \n]*[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/][ *\n]*    { printf("--------------------------------------------------------\n");}
.                              { printf("Mystery character %s\n", yytext); }


%%
