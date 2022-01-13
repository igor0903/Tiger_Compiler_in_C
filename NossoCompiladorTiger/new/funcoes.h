#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h" 

Exp mk_num(int v){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  e->tag = NUM;
  e->val = v;
  return e;
}

Exp mk_id(char* txt){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  char* str = malloc(strlen(txt)+1);
  strcpy(str,txt);
  e->tag = ID;
  e->id = str;
  return e;
}

Exp mk_op(operacoes op, Exp e1, Exp e2){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  e->tag = OP;
  e->operacoes.op = op;
  e->operacoes.left = e1;
  e->operacoes.right = e2;
  return e;
}

Exp mk_tc(ties_cond tc, Exp left, Exp middle, Exp right){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  e->tag = TC;
  e->ties_cond.tc = tc;
  e->ties_cond.left = left;
  e->ties_cond.middle = middle;
  e->ties_cond.right = right;
  return e;
}

Exp mk_io(io_ io, Exp value){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  e->tag = IO;
  e->io_.io = io;
  e->io_.value = value;
  return e;
}

Exp mk_decl(operacoes op, Exp e1, Exp e2){
  Exp e = (Exp)malloc(sizeof(struct _exp));
  e->tag = DECL;
  e->operacoes.op = op;
  e->operacoes.left = e1;
  e->operacoes.right = e2;
  return e;
}

Programa mk_pg(Exp decl, Exp exp){
  Programa p = (Programa)malloc(sizeof(struct _programa));
  p->declaracao = decl;
  p->exprecoes = exp;
  return p;
}


void printaEnumOp(operacoes op){
  switch(op){
    case PLUS:
      printf("PLUS ");
      break;
    case MINUS:
      printf("MINUS ");
      break;
    case TIME:
      printf("TIME ");
      break;
    case DIV:
      printf("DIV ");
      break;
    case MOD:
      printf("MOD ");
      break;
    case GT:
      printf("GT ");
      break;
    case LT:
      printf("LT ");
      break;
    case GE:
      printf("GE ");
      break;
    case LE:
      printf("LE ");
      break;
    case NE:
      printf("NE ");
      break;
    case EQ:
      printf("EQ ");
      break;
    case VAR_:
      printf("VAR ");
      break;
    case ASSERT:
      printf("ASSERT ");
      break;
    case EXP_SEQ:
      printf("EXP_SEQ ");
      break;
    default:
      printf("NAO IMPLEMENTADO ");
      break;
  }
}

void printaEnumTc(ties_cond tc){
  switch (tc){
  case IF_:
    printf("IF ");    
    break; 
  case IF_ELSE:
    printf("IF ELSE ");
    break;
  case WHILE_DO:
    printf("WHILE DO ");
    break;
  default:
    printf("NAO IMPLEMENTADO ");
    break;
  }
}

void printaEnumIo(io_ io){
  switch (io){
  case PRINTI_:
    printf("PRINTI");
    break;
  case SCANI_:
    printf("SCANI");
    break;
  default:
    printf("NAO IMPLEMENTADO ");
    break;
  }
}

void printaExp(Exp e){
  switch(e->tag){
    case NUM:
      printf("[NUM %d]", e->val);
      break;
    case ID:
      printf("[ID %s]", e->id);
      break;
    case OP:
      printf("(");
      printaEnumOp(e->operacoes.op);
      printaExp(e->operacoes.left);
      printf(" ");
      printaExp(e->operacoes.right);
      printf(")");
      break;
    case TC:
      printf("(");
      printaEnumTc(e->ties_cond.tc);
      printaExp(e->ties_cond.left);
      printf(" ");
      printaExp(e->ties_cond.middle);
      if (e->ties_cond.right != NULL){
        printf(" ");
        printaExp(e->ties_cond.right);
      }
      printf(")");
      break;
    case IO:
      printf("(");
      printaEnumIo(e->io_.io);
      if (e->io_.value != NULL){
        printf(" ");
        printaExp(e->io_.value);
      }
      printf(")");
      break;
    case DECL:
      printf("(");
      printaEnumOp(e->operacoes.op);
      printaExp(e->operacoes.left);
      if(e->operacoes.right != NULL){
        printf("); ");
        printaExp(e->operacoes.right);
        
       }else{
        printf(");");
      }
      
      break;
    default:
      printf("erro \n");
      break;
  }
}

void printaPrg(Programa p){
  printf("LET \n");
  printaExp(p->declaracao);
  printf("\n IN \n");
  printaExp(p->exprecoes);
  printf("\n");
}

void printa_instrucao(Instr instr_){
  switch (instr_.codigo.opcode){
    case MOVEI:
      printf("MOVEI T%ld %ld\n",instr_.arg1,instr_.arg2);
      break;
    case OP_:
      printf("OP ");
      printaEnumOp(instr_.codigo.op_);
      printf("T%ld T%ld T%ld \n",instr_.arg1,instr_.arg2,instr_.arg3);
      break;
    case COND:
      printf("COND ");
      printaEnumOp(instr_.codigo.op_);
      if(instr_.codigo.opcode2 == LABEL){
        printf("T%ld T%ld label%ld label%ld \n",instr_.arg2,instr_.arg3,instr_.arg4,instr_.arg5);
      }else{
         printf("T%ld T%ld T%ld \n",instr_.arg1,instr_.arg2,instr_.arg3);
      }
      break;
    case LABEL:
      printf("LABEL label%ld\n",instr_.arg1);
      break;
    case JUMP:
      printf("JUMP label%ld\n",instr_.arg1);
      break;
    case MOVE:
      printf("MOVE T%ld T%ld\n",instr_.arg1,instr_.arg2);
      break;
    case CALL:
      if(instr_.codigo.i_o == SCANI_){
        printf("CALL SCANI\n");
      }else{
        printf("CALL PRINTI T%ld\n",instr_.arg1);
      }
      break;
    default:
      printf("\nDEFAULT_PRINTA_INSTRUCOES\n");
      break;
  }
}

void itera_instrucao(node *PILHA){
  if(PILHA->prox == NULL){
    printa_instrucao(PILHA->instr);
    return;
  }else{
    printa_instrucao(PILHA->instr);
    itera_instrucao(PILHA->prox);
  }

}

/*
SCANI -> CALL scani 
printi(algo) -> CALL printi tx



*/