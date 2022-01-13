#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum op{PLUS,MINUS,TIME,DIV,MOD,GT,LT,GE,LE,EQ,NE,ASSERT,EXP_SEQ,VAR_} ;
enum ties_cond{IF_,IF_ELSE,WHILE_DO};
enum io{PRINTI_,SCANI_};

typedef enum io io_;
typedef enum op operacoes;
typedef enum ties_cond ties_cond;

struct _exp{
  enum {ID,NUM,OP,TC,IO,DECL} tag;
  union{
    int val;
    char*id;
    struct{
      operacoes op;
      struct _exp *left, *right;
    } operacoes;

    struct{
      ties_cond tc;
      struct _exp *left,*middle, *right;
    } ties_cond;

    struct{
      io_ io;
      struct _exp *value;
    } io_;
  };
};typedef struct _exp *Exp;


struct _programa{
  Exp declaracao;
  Exp exprecoes;
}; typedef struct _programa *Programa;

/*---------------------------------------------------------------------------------------------------------*/


typedef intptr_t Addr; // endereço (inteiro ou apontador)
typedef enum { MOVE, MOVEI, OPI, OP_, LABEL, JUMP, COND, CALL } Opcode; // código da instrução
typedef struct {
    Opcode opcode;
    Opcode opcode2;
    Opcode opcode3;
    operacoes op_;
    ties_cond tc_;
    io_ i_o;

} Codigo;

typedef struct {
    Codigo codigo;
    Addr arg1, arg2, arg3,arg4,arg5;
} Instr; //opcode = MOVEI arg1 = dest arg2 = exp->val arg3 = NULL;

/*---------------------------------------------------------------------------------------------------------*/


struct Node{
 char* id;
 int registro;
 struct Node *prox;
 Instr instr;
};
typedef struct Node node;