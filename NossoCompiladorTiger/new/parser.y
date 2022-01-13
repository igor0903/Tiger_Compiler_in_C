%{
#include "codegenerator2.c"
#include <stdio.h>
#include <stdlib.h>
int yylex (void);
void yyerror (char const *);
%}

%token TOK_NUM TOK_CHAR T_NEWLINE IF THEN ELSE DO WHILE PRINTI SCANI VAR IN LET

%union {
    int ival;
    char *sval;
    struct _exp * exp_;
    struct _programa * programa_;
    //struct _assert * assert_;
}


%type <sval> TOK_CHAR
%type <ival> TOK_NUM
%type <exp_> term exp exp_seq assert op cond decl
%type <programa_> new_line
%start programa

%%

programa :new_line              {intermedio($1);/*printaPrg($1)*/}
    ;

new_line :LET decl IN exp_seq       {$$ = mk_pg($2,$4);}
    ;


decl: VAR assert                { $$ = mk_decl(VAR_,$2,NULL); }
    | VAR assert decl           { $$ = mk_decl(VAR_,$2,$3); }
    ;

exp : assert                            { $$ = $1; }
    | op                                { $$ = $1; }
    | cond                              { $$ = $1; }
    | '(' exp_seq ')'                   { $$ = $2; }
    | WHILE exp DO exp                  { $$ = mk_tc(WHILE_DO,$2, $4,NULL); }
    | PRINTI'(' exp ')'                 { $$ = mk_io(PRINTI_,$3);}
    ;



cond:IF op THEN exp ELSE exp            { $$ = mk_tc(IF_ELSE,$2, $4,$6); }
    | IF op THEN exp                    { $$ = mk_tc(IF_,$2, $4,NULL); }
    ;

op:term                               { $$ = $1; }
    |'(' op ')'                       { $$ = $2; }
    | op '+' op                       { $$ = mk_op(PLUS,$1, $3); }
    | op '-' op                       { $$ = mk_op(MINUS,$1, $3); }
    | op '/' op                       { $$ = mk_op(DIV,$1, $3); }
    | op '%' op                       { $$ = mk_op(MOD,$1, $3); }
    | op '*' op                       { $$ = mk_op(TIME,$1, $3); }
    | op '>' op                       { $$ = mk_op(GT,$1, $3); }
    | op '<' op                       { $$ = mk_op(LT,$1, $3); }
    | op '=' op                       { $$ = mk_op(EQ,$1, $3); }
    | op '>' '=' op                   { $$ = mk_op(GE,$1, $4); }
    | op '<' '=' op                   { $$ = mk_op(LE,$1, $4); }
    | op '<' '>' op                   { $$ = mk_op(NE,$1, $4); }
    ;

assert : TOK_CHAR ':' '=' op           { $$ = mk_op(ASSERT,mk_id($1),$4);}
    ;

exp_seq : exp           { $$ = $1; }
    | exp_seq ';' exp   {$$ = mk_op(EXP_SEQ,$1,$3);}

term : TOK_NUM      {$$ = mk_num($1);}
    | TOK_CHAR      {$$ = mk_id($1);}
    | SCANI         { $$ = mk_io(SCANI_,NULL);}
    ;



%%

void yyerror (char const *msg) {
  printf("parse error: %s \n", msg);
  exit(-1);
}
