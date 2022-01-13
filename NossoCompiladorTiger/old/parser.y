%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10000000

int yylex (void);
void yyerror (char const *);//%type <name> TOK_CHAR
int val;
%}

%union {
	int ival;
  char *sval;
  }

%token  TOK_NUM TOK_CHAR IF ELSE THEN WHILE DO INT PRINTI SCANI LET END VAR

%type <ival>  TOK_NUM
%type <sval>  op_expC TOK_CHAR op_expN op_arith op_logic op_cond head2 assign op_while types separator /*printi op_input_output*/
%start input
%%

input:
  %empty
  | input head
  ;

head : '\n'
    |head2  {printf("%s \n", $1);}
    ;

head2: assign {$$ = $1;}

    |  op_cond  {$$ = $1;}

    | '\n' op_while {$$ = $2;}
    | op_while {$$ = $1;}

    |types {$$ = $1;}

    |  op_arith  {$$ = $1;}
    | op_logic {$$ = $1;}
    | separator {$$ = $1;}
    | '\n' head2 {$$ = $2;}
    | head2 '\n'{$$ = $1;}
    ;

op_arith: op_expN {$$ = $1;}
    | op_expC {$$ = $1;}
    | op_expN '+' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(ADD %s %s)", $1,$3);$$ = buf;}
    | op_expC '+' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(ADD %s %s)", $1,$3);$$ = buf;}

    | op_expN '-' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(DIFF %s %s)", $1,$3);$$ = buf;}
    | op_expC '-' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(DIFF %s %s)", $1,$3);$$ = buf;}

    | op_expN '%' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(MOD %s %s)", $1,$3);$$ = buf;}
    | op_expC '%' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(MOD %s %s)", $1,$3);$$ = buf;}

    | op_expN '/' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(DIV %s %s)", $1,$3);$$ = buf;}
    | op_expC '/' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(DIV %s %s)", $1,$3);$$ = buf;}

    | op_expN '*' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(TIMES %s %s)", $1,$3);$$ = buf;}
    | op_expC '*' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(TIMES %s %s)", $1,$3);$$ = buf;}
    ;

op_logic : op_expN {$$ = $1;}
    | op_expC {$$ = $1;}

    | op_expN '<' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LT %s %s)", $1,$3);$$ = buf;}
    | op_expC '<' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LT %s %s)", $1,$3);$$ = buf;}
    | op_arith '<' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LT %s %s)", $1,$3);$$ = buf;}

    | op_expN '>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GT %s %s)", $1,$3);$$ = buf;}
    | op_expC '>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GT %s %s)", $1,$3);$$ = buf;}
    | op_arith '>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GT %s %s)", $1,$3);$$ = buf;}

    | op_expN '=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(EQUALS %s %s)", $1,$3);$$ = buf;}
    | op_expC '=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(EQUALS %s %s)", $1,$3);$$ = buf;}
    | op_arith '=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(EQUALS %s %s)", $1,$3);$$ = buf;}

    | op_expN '>''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GE %s %s)", $1,$4);$$ = buf;}
    | op_expC '>''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GE %s %s)", $1,$4);$$ = buf;}
    | op_arith '>''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(GE %s %s)", $1,$4);$$ = buf;}

    | op_expN '<''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LE %s %s)", $1,$4);$$ = buf;}
    | op_expC '<''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LE %s %s)", $1,$4);$$ = buf;}
    | op_arith '<''=' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(LE %s %s)", $1,$4);$$ = buf;}

    | op_expN '<''>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(NE %s %s)", $1,$4);$$ = buf;}
    | op_expC '<''>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(NE %s %s)", $1,$4);$$ = buf;}
    | op_arith '<''>' op_arith {char *buf = (char*)malloc(MAX*sizeof(char));
                            snprintf(buf, MAX, "(NE %s %s)", $1,$4);$$ = buf;}



op_cond: IF op_logic THEN head2 {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(IF %s THEN %s)", $2,$4);
                                $$ = buf;}
    |IF op_logic THEN head2 ELSE head2 {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(IF %s THEN %s ELSE %s)", $2,$4,$6);
                                $$ = buf;}
  ;

op_while: WHILE op_logic DO head2 {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(WHILE %s DO %s)", $2,$4);
                                $$ = buf;}
    ;

assign:op_expC ':' '=' op_arith     {char *buf = (char*)malloc(MAX*sizeof(char));
                                    snprintf(buf, MAX, "(ASSIGN %s %s)", $1,$4);$$ = buf;}
    | op_expC ':' '=' op_logic     {char *buf = (char*)malloc(MAX*sizeof(char));
                                    snprintf(buf, MAX, "(ASSIGN %s %s)", $1,$4);$$ = buf;}
    | op_expC ':' '=' SCANI '(' ')'     {char *buf = (char*)malloc(MAX*sizeof(char));
                                    snprintf(buf, MAX, "(SCANI %s)", $1);$$ = buf;}       
     ;

op_expN: TOK_NUM {char *buf = (char*)malloc(MAX*sizeof(char));snprintf(buf, MAX, "(NUM %d)", $1); $$ = buf;}
    ;

op_expC: TOK_CHAR {char *buf = (char*)malloc(MAX*sizeof(char));snprintf(buf, MAX, "(CHAR %s)", $1); $$ = buf;}
    ;

types: INT op_expC {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(INT %s)", $2);
                                $$ = buf;}
    | INT assign {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(INT %s)", $2);
                                $$ = buf;}
    | VAR op_expC {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(VAR %s)", $2);
                                $$ = buf;}
    | VAR assign {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(VAR %s)", $2);
                                $$ = buf;}
    ;
//op_input_output : PRINTI op_expC ':' 

separator : '(' head2 ')' {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "PARENTHESES %s", $2);
                                $$ = buf;}
    |'{' head2 '}' {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "BRACES %s", $2);
                                $$ = buf;}
    |'[' head2 ']' {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "BRACKETS %s", $2);
                                $$ = buf;}
    |head2 ';' head2 {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(SUM_EXP %s %s)", $1,$3);
                                $$ = buf;}
          ;
/*
printi: PRINTI '(' head2 ')' {char *buf = (char*)malloc(MAX*sizeof(char));
                                snprintf(buf, MAX, "(PRINTI %s)", $3);
                                $$ = buf;}
        ;
        */
   
%%



void yyerror (char const *msg) {
  printf("parse error: %s\n", msg);
  exit(-1);
}


