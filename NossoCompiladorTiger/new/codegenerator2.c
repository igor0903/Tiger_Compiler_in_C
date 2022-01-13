#include "pilha.c"
int transExp(Exp exp,int dest);
int temp_count = 0, label_count = 0; //var temporarias e labelcounts
node *PILHA;
node *PILHA_INSTRUCAO;

int newTemp() {
    return temp_count++;
}
int newLabel () {
    return label_count ++;
}


void emit1(Codigo opcode_,int t1){
    Instr final;
    final.arg2 = -1;
    final.arg3 = -1;
    final.arg4 = -1;
    final.arg5 = -1;

    Addr T1_ = t1;

    final.codigo = opcode_;
    final.arg1 = T1_;   

    node* el = aloca_instrucao(final);
    PILHA_INSTRUCAO = push(PILHA_INSTRUCAO,el);
}

Instr emit2(Codigo opcode_,int dest,int t1){
    Instr final;
    final.arg3 = -1;
    final.arg4 = -1;
    final.arg5 = -1;
    Addr T1_ = t1;
    Addr T2_ = dest;
    switch(opcode_.opcode){
        case MOVE:
        case MOVEI:
            /*criar struct Instr*/
            final.codigo = opcode_;
            final.arg1 = T2_; 
            final.arg2 = T1_; 
            break;
        default:
            break;
    }
    return final;
}

Instr emit3(Codigo opcode_,int dest,int t1, int t2){
    Instr final;
    final.arg4 = -1;
    final.arg5 = -1;
    Addr T1_ = t1;
    Addr T2_ = t2;
    Addr T3_ = dest;
    switch(opcode_.opcode){
        case COND:
        case OP_:
            final.codigo = opcode_;
            final.arg1 = T3_;
            final.arg2 = T1_; 
            final.arg3 = T2_; 
            break;
        case OPI:
            /*criar struct Instr*/
            printf("OPI T%d T%d\n",dest,t1);
            break;
        default:
            break;
    }
    return final;
}

Instr emit5(Codigo opcode_, int t1, int t2, int l1, int l2, int l3){
    Instr final;
    Addr JUMP_;
    Addr LABEL1_;
    Addr LABEL2_;
    Addr T1_;
    Addr T2_;
    switch (opcode_.opcode){
        case JUMP:
            JUMP_ = l1;
            LABEL1_ = l2;
            LABEL2_ = l3;
            T1_ = t1;
            T2_ = t2;
            final.codigo = opcode_;
            final.arg1 = JUMP_;
            final.arg2 = LABEL1_; 
            final.arg3 = LABEL2_;
            final.arg4 = T1_;
            final.arg5 = T2_;
            break;
        case LABEL:
            LABEL1_ = l1;
            LABEL2_ = l2;
            T1_ = t1;
            T2_ = t2;
            final.codigo = opcode_;
            final.arg1 = LABEL1_; 
            final.arg2 = LABEL2_;
            final.arg3 = T1_;
            final.arg4 = T2_;
        case COND:
            LABEL1_ = l1;
            LABEL2_ = l2;
            T1_ = t1;
            T2_ = t2;
            final.codigo = opcode_;
            final.arg2 = T1_;
            final.arg3 = T2_;
            final.arg4 = LABEL1_;
            final.arg5 = LABEL2_;
        default:
            break;
    }
    return final;
}

Codigo opcode_operacoes(operacoes op){
  Codigo c;
  c.op_ = op;
  switch(op){
    case ASSERT:
      c.opcode = MOVE;
      return c;
    case EXP_SEQ:
      c.opcode = MOVE;
      return c;
    case VAR_:
      c.opcode = MOVE;
      return c;
    case MINUS:
    case TIME:
    case DIV:
    case MOD:
    case PLUS:
      c.opcode = OP_;
      return c;
    default:
      c.opcode = COND;
      return c;
      break;
  }
}

Codigo opcode_ties_cond(ties_cond tc){
  Codigo c;
  c.tc_ = tc;
  switch(tc){
      case WHILE_DO:
        c.opcode = JUMP;
        break;
      default:
        c.opcode = LABEL;
        break;
  }
  return c;
}


void transCond(Exp exp,int dest, int label1,int label2){
  int t1 = newTemp();
  int t2 = newTemp();
  t1 = transExp(exp->operacoes.left, t1);
  t2 = transExp(exp->operacoes.right, t2);
  Codigo c;
  c.op_ = exp->operacoes.op;
  c.opcode = COND;
  c.opcode2 = LABEL;
  Instr final = emit5(c,t1,t2,label1,label2,-1);//uma parte da isntrução sera feita aqui
  node* el = aloca_instrucao(final);
  PILHA_INSTRUCAO = push(PILHA_INSTRUCAO,el);

}

Instr analisa_io(Exp exp){
    Instr final;
    int t1;
    t1 = newTemp();

    Codigo c;
    c.i_o = exp->io_.io;
    c.opcode = CALL;

    switch (exp->io_.io){
        case SCANI_:
            emit1(c,t1);
            break;
        case PRINTI_:
            t1 = transExp(exp->io_.value,t1);
            emit1(c,t1);
            break;
        default:
            break;
    }

    return final;
}

void analisa_tc(Exp exp){
    Instr final;
    int t1,t2,t3;
    int l1,l2,l3;
    l3 = -1;
    switch(exp->ties_cond.tc){
      case WHILE_DO:
        l1 = newLabel();
        l2 = newLabel();
        l3 = newLabel();
        
        t1 = newTemp();
        t2 = newTemp();
        Codigo c;
        c.opcode = LABEL;
        emit1(c,l1);//label inicio while
        transCond(exp->ties_cond.left, t1,l2,l3);
        emit1(c,l2);// label dentro while
        
        
        t2 = transExp(exp->ties_cond.middle, t2);
        emit1(opcode_ties_cond(exp->ties_cond.tc),l1); //jump
        emit1(c,l3); //label final

        break;
      default:
        t1 = newTemp();
        t2 = newTemp();

        l1 = newLabel();
        l2 = newLabel();

        transCond(exp->ties_cond.left, t1,l1,l2);
        emit1(opcode_ties_cond(exp->ties_cond.tc),l1);

        t2 = transExp(exp->ties_cond.middle, t2);

        emit1(opcode_ties_cond(exp->ties_cond.tc),l2);
        if(exp->ties_cond.right != NULL){
          t3 = newTemp();
          l3 = newLabel();
          t3 = transExp(exp->ties_cond.right, t3);
          emit1(opcode_ties_cond(exp->ties_cond.tc),l3);
        }

    }
    
}

Instr registra(Exp exp,int dest){ 
    Instr final;
    Exp esq = exp->operacoes.left; //nome da variavel
    int t1 = newTemp();
    node *el = aloca(esq->id,t1);
    PILHA=push(PILHA,el);

    Exp dir = exp->operacoes.right; // valor da variavel
    int t2 = newTemp();
    t2 = transExp(dir,t2);
    final = emit2(opcode_operacoes(exp->operacoes.op),t1,t2);
    return final;
}

int transExp(Exp exp,int dest) {
    Instr final;
    Codigo c;
    int t1,t2;
    node *el;
    switch(exp->tag) {
        case OP:
            t1 = newTemp();
            t2 = newTemp();
            t1 = transExp(exp->operacoes.left, t1); // lado esquerdo
            t2 = transExp(exp->operacoes.right, t2); // lado direito
            if(exp->operacoes.op == EXP_SEQ) return dest;
            if(opcode_operacoes(exp->operacoes.op).opcode != MOVE){
                final = emit3(opcode_operacoes(exp->operacoes.op), dest, t1, t2); // instrução final
            }else{
                final = emit2(opcode_operacoes(exp->operacoes.op), t1, t2); // instrução final
            }
            break;
        case NUM:
            c.opcode = MOVEI;
            final = emit2(c,dest,exp->val);
            break;
        case TC:
            analisa_tc(exp);
            return dest;
            break;
        case ID:
            el = search(PILHA, exp->id);
            if(el == NULL){
                printf("ERRO, VARIAVEL NÃO DECLARADA ANTERIORMENTE\n");
                exit(0);
            }
            return el->registro;
            break;
        case DECL:
            /*colocar na tabela*/
            final = registra(exp->operacoes.left,dest);
            if (exp->operacoes.right != NULL){
                t2 = transExp(exp->operacoes.right,dest);
            }
            break;
        case IO:
            final = analisa_io(exp);
            return dest;
            break;
        default:
            break;
    }
    el = aloca_instrucao(final);
    PILHA_INSTRUCAO = push(PILHA_INSTRUCAO,el);
    return dest;
}


void intermedio(Programa pg){
    PILHA = (node *) malloc(sizeof(node));
    PILHA_INSTRUCAO = (node *) malloc(sizeof(node));
    PILHA = inicia(PILHA);
    PILHA_INSTRUCAO = inicia(PILHA_INSTRUCAO);
    Exp dec = pg->declaracao;
    Exp exp = pg->exprecoes;
    transExp(dec,newTemp());
    transExp(exp,newTemp());
    /*
    Exp id_ = mk_id("X");
    //Exp num_ = mk_num(9);
    Exp scani_ = mk_io(SCANI_,NULL);
    Exp assert_ = mk_op(ASSERT,id_,scani_);
    Exp decl_ = mk_decl(VAR_,assert_,NULL);
    transExp(decl_,newTemp());
    */
    itera_instrucao(PILHA_INSTRUCAO);
}
/*
int main(){
    intermedio(NULL);
    return 0;
}*/