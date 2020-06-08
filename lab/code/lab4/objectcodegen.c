#include <assert.h>
#include "objectcodegen.h"

// registers
Register regs[32];
// stack in memory
StackDesc stk;
// function parameter list head
InterCodeNode* paramListHead = NULL;
// function arg list head
InterCodeNode* objectArgListHead = NULL;

extern InterCodeNode* interCodeListHead;

void insertParam(InterCode* ir){
    InterCodeNode* p = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    p->interCode = ir;
    p->next = NULL;

    if(!paramListHead){
        paramListHead = p;
    }else{
        InterCodeNode* q = paramListHead;
        while(q->next) q=q->next;
        q->next = p;
    }
}

void clearParamList(){
    if(paramListHead == NULL){
        return;
    }
    InterCodeNode* p = paramListHead;
    while(p->next){
        InterCodeNode* next = p->next;
        free(p);
        p = next;
    }
    free(p);
    p = NULL;
    paramListHead = NULL;
}

void insertArg(InterCode* ir){
    InterCodeNode* p = (InterCodeNode*)malloc(sizeof(InterCode));
    p->interCode = ir;
    p->next = objectArgListHead;
    objectArgListHead = p;

    /*
    p->next = NULL;
    if(!objectArgListHead){
        objectArgListHead = p;
    }else{
        InterCodeNode* q = objectArgListHead;
        while(q->next) q=q->next;
        q->next = p;
    }
    */
}

void clearObjectArgList(){
    if(objectArgListHead == NULL){
        return;
    }
    InterCodeNode* p = objectArgListHead;
    while(p->next){
        InterCodeNode* next = p->next;
        free(p);
        p = next;
    }
    free(p);
    p = NULL;
    objectArgListHead = NULL;
}

int countArgList(){
    int count = 0;
    InterCodeNode* p = objectArgListHead;
    while(p){
        count++;
        p=p->next;
    }
    return count;
}

void showAllRegs(){
    for(int i = 8; i <= 25; ++i){
        //if((i >= 8 && i <= 15) || i == 24 || i == 25){
            if(regs[i].content == NULL){
                printf("[%d] %s, age %d, content is NULL\n", regs[i].index, regs[i].name, regs[i].age);
            }else{
                printf("[%d] %s, age %d, content (type %d, value %s)\n", regs[i].index, regs[i].name, regs[i].age, regs[i].content->op->type, regs[i].content->op->value);
            }
        //}
    }
}

char* getStr(char* str){
    char* res = (char*)malloc(strlen(str)+1);
    strcpy(res, str);
    return res;
}

void initStk(){
    for(int i = 0; i < STK_SIZE; ++i){
        stk.stack[i] = NULL;
    }
    stk.top = 0;
}

void initRegs(){
    regs[0].name = getStr("$zero");
    
    regs[1].name = getStr("$at");
    
    regs[2].name = getStr("$v0");
    regs[3].name = getStr("$v1");
    
    regs[4].name = getStr("$a0");
    regs[5].name = getStr("$a1");
    regs[6].name = getStr("$a2");
    regs[7].name = getStr("$a3");

    regs[8].name = getStr("$t0");
    regs[9].name = getStr("$t1");
    regs[10].name = getStr("$t2");
    regs[11].name = getStr("$t3");
    regs[12].name = getStr("$t4");
    regs[13].name = getStr("$t5");
    regs[14].name = getStr("$t6");
    regs[15].name = getStr("$t7");

    regs[16].name = getStr("$s0");
    regs[17].name = getStr("$s1");
    regs[18].name = getStr("$s2");
    regs[19].name = getStr("$s3");
    regs[20].name = getStr("$s4");
    regs[21].name = getStr("$s5");
    regs[22].name = getStr("$s6");
    regs[23].name = getStr("$s7");

    regs[24].name = getStr("$t8");
    regs[25].name = getStr("$t9");

    regs[26].name = getStr("$k0");
    regs[27].name = getStr("$k1");

    regs[28].name = getStr("$gp");

    regs[29].name = getStr("$sp");

    regs[30].name = getStr("$fp");

    regs[31].name = getStr("$ra");

    for(int i = 0; i < 32; ++i){
        regs[i].content = NULL;
        regs[i].index = i;
        regs[i].age = 0;
    }
}

void pushStack(VarDesc* var){
    stk.stack[stk.top] = var;
    stk.top++;
}

VarDesc* popStack(){
    VarDesc* tmp = copyVarDesc(stk.stack[stk.top-1]);
    stk.stack[stk.top-1] = NULL;
    stk.top--;
    return tmp;
}

VarDesc* copyVarDesc(VarDesc* src){
    VarDesc* res = (VarDesc*)malloc(sizeof(VarDesc));
    res->op = copyOperand(src->op);
    res->reg_no = src->reg_no;
    return res;
}

void saveVarIntoStack(VarDesc* var, FILE* fp){
    pushStack(var);

    fputs("  addi $sp, $sp, -4\n", fp);
    char str[30];
    memset(str, 0, sizeof(str));
    sprintf(str, "  sw %s, 0($sp)\n", regs[var->reg_no].name);
    fputs(str, fp);
    regs[var->reg_no].content = NULL;
    regs[var->reg_no].age = 0;
}

VarDesc* loadVarFromStackTop(FILE* fp){
    VarDesc* var = popStack();
    regs[var->reg_no].content = var;
    regs[var->reg_no].age = 0;

    char str[30];
    memset(str, 0, sizeof(str));
    sprintf(str, "  lw %s, 0($sp)\n", regs[var->reg_no].name);
    fputs(str, fp);
    fputs("  addi $sp, $sp, 4\n", fp);
    return var;
}

void saveAllVarIntoStack(FILE* fp){
    // t0 - t7
    /*
    for(int i = 8; i <= 15; ++i){
        if(regs[i].content){
            saveVarIntoStack(regs[i].content, fp);
        }
    }
    // t8, t9
    if(regs[24].content){
        saveVarIntoStack(regs[24].content, fp);
    }
    if(regs[25].content){
        saveVarIntoStack(regs[25].content, fp);
    }
    */
    for(int i = 8; i <= 25; ++i){
        if(regs[i].content){
            saveVarIntoStack(regs[i].content, fp);
        }
    }
}

void loadAllVarFromStack(FILE* fp){
    while(stk.top > 0){
        loadVarFromStackTop(fp);
    }
}

bool compareOperand(Operand* o1, Operand* o2){
    if((o1 == o2) || (o1->type == o2->type && strcmp(o1->value, o2->value)==0)){
        return true;
    }else if(strcmp(o1->value, o2->value)==0 && ((o1->type==Param && o2->type==Variable) || (o1->type==Variable && o2->type==Param)) ){
        return true;
    }else return false;
}

int getReg(Operand* operand, FILE* fp){
    // $t0-$t7: 8-15
    // $s0-$s7: 16-23
    // $t8-$t9: 24-25
    
    int res = -1;
    for(int i = 8; i <= 25; ++i){
        //if((i >= 8 && i <= 15) || i == 24 || i == 25){
            if(regs[i].content != NULL && compareOperand(regs[i].content->op, operand)){
                // already in register
                regs[i].age = 0;
                res = i;
                break;
            }
        //}
    }

    if(res == -1){
        for(int i = 8; i <= 25; ++i){
            //if((i >= 8 && i <= 15) || i == 24 || i == 25){
                if(regs[i].content == NULL){
                    // found empty register
                    res = i; 
                    break;
                }
            //}
        }

        if(res == -1){
            // no empty register found
            int maxAge = -1;
            for(int i = 8; i <= 25; ++i){
                //if((i >= 8 && i <= 15) || i == 24 || i == 25){
                    if(regs[i].age > maxAge){
                        maxAge = regs[i].age;
                        res = i;
                    }
                //}
            }
        }else{
            // found empty register, aging other registers
            for(int i = 8; i <= 25; ++i){
                //if((i >= 8 && i <= 15) || i == 24 || i == 25){
                    if(regs[i].content != NULL && i != res){
                        regs[i].age++;
                    }
                //}
            }
        }
    }else{
        // already in register, aging other registers
        for(int i = 8; i <= 25; ++i){
            //if((i >= 8 && i <= 15) || i == 24 || i == 25){
                if(regs[i].content != NULL && i != res){
                    regs[i].age++;
                }
            //}
        }
    }

    if(res != -1){
        if(operand->type == Constant){
            // load constant
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  li %s, %s\n", regs[res].name, subStr(operand->value, 1));
            fputs(str, fp);
        }
        // set register
        regs[res].content = (VarDesc*)malloc(sizeof(VarDesc));
        regs[res].content->op = operand;
        regs[res].content->reg_no = res;
        regs[res].age = 0;
    }else{
        assert(0);
    }

    return res;
}

char* subStr(char* src, int begin){
    char tmp[20];
    memset(tmp, 0, sizeof(tmp));
    int copyLen = strlen(src)-begin;
    strncpy(tmp, src+begin, copyLen);
    tmp[copyLen] = '\0';

    char* res = (char*)malloc(strlen(tmp)+1);
    strcpy(res, tmp);
    return res;
}

void clearRegs(){
    for(int i = 8; i <= 25; ++i){
        //if((i>=8 && i<=15) || i==24 || i==25){
            regs[i].age = 0;
            regs[i].content = NULL;
        //}
    }
}

void translateInterCode(InterCode* ir, FILE* fp){
    switch(ir->type){
        case Label_IR:{
            char* label = ir->ops.o1.op1->value;
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "%s:\n", label);
            fputs(str, fp);
            break;
        }
        case Function_IR:{
            Operand* op = ir->ops.o1.op1;
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "\n%s:\n", op->value);
            fputs(str, fp);

            clearRegs();
            if(paramListHead){  // function with params
                int count = 0;
                InterCodeNode* p = paramListHead;
                while(p){
                    Operand* param = p->interCode->ops.o1.op1;
                    int regParam = getReg(param, fp);
                    memset(str, 0, sizeof(str));
                    sprintf(str, "  move %s, $a%d\n", regs[regParam].name, count);
                    fputs(str, fp);
                    count++;
                    p=p->next;
                }
            }

            break;
        }
        case Assign_IR:{
            Operand* leftOp = ir->ops.o2.op1;
            Operand* rightOp = ir->ops.o2.op2;
            if(rightOp->type == Constant){
                int leftReg = getReg(leftOp, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  li %s, %s\n", regs[leftReg].name, subStr(rightOp->value, 1));
                fputs(str, fp);
            }else{
                int leftReg = getReg(leftOp, fp);
                int rightReg = getReg(rightOp, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  move %s, %s\n", regs[leftReg].name, regs[rightReg].name);
                fputs(str, fp);
            }
            break;
        }
        case Plus_IR:{
            Operand* res = ir->ops.o3.op1;
            Operand* op2 = ir->ops.o3.op2;
            Operand* op3 = ir->ops.o3.op3;
            if(op3->type == Constant){
                int reg2 = getReg(op2, fp);
                int regRes = getReg(res, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  addi %s, %s, %s\n", regs[regRes].name, regs[reg2].name, subStr(op3->value, 1));
                fputs(str, fp);
            }else{
                int regRes = getReg(res, fp);
                int reg2 = getReg(op2, fp);
                int reg3 = getReg(op3, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  add %s, %s, %s\n", regs[regRes].name, regs[reg2].name, regs[reg3].name);
                fputs(str, fp);
            }
            break;
        }
        case Minus_IR:{
            Operand* res = ir->ops.o3.op1;
            Operand* op2 = ir->ops.o3.op2;
            Operand* op3 = ir->ops.o3.op3;
            if(op3->type == Constant){
                int reg2 = getReg(op2, fp);
                int regRes = getReg(res, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  addi %s, %s, %d\n", regs[regRes].name, regs[reg2].name, -atoi(subStr(op3->value, 1)));
                fputs(str, fp);
            }else{
                int regRes = getReg(res, fp);
                int reg2 = getReg(op2, fp);
                int reg3 = getReg(op3, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  sub %s, %s, %s\n", regs[regRes].name, regs[reg2].name, regs[reg3].name);
                fputs(str, fp);
            }
            break;
        }
        case Multiply_IR:{
            Operand* res = ir->ops.o3.op1;
            Operand* op2 = ir->ops.o3.op2;
            Operand* op3 = ir->ops.o3.op3;
            int regRes = getReg(res, fp);
            int reg2 = getReg(op2, fp);
            int reg3 = getReg(op3, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  mul %s, %s, %s\n", regs[regRes].name, regs[reg2].name, regs[reg3].name);
            fputs(str, fp);
            break;
        }
        case Divide_IR:{
            Operand* op2 = ir->ops.o3.op2;
            Operand* op3 = ir->ops.o3.op3;
            int reg2 = getReg(op2, fp);
            int reg3 = getReg(op3, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  div %s, %s\n", regs[reg2].name, regs[reg3].name);
            fputs(str, fp);

            memset(str, 0, sizeof(str));
            Operand* res = ir->ops.o3.op1;
            int regRes = getReg(res, fp);
            sprintf(str, "  mflo %s\n", regs[regRes].name);
            fputs(str, fp);
            break;
        }
        case Address_IR:
        case ReadMemory_IR:{
            Operand* op1 = ir->ops.o2.op1;
            Operand* op2 = ir->ops.o2.op2;
            int reg1 = getReg(op1, fp);
            int reg2 = getReg(op2, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  lw %s, 0(%s)\n", regs[reg1].name, regs[reg2].name);
            fputs(str, fp);
            break;
        }
        case WriteMemory_IR:{
            Operand* op1 = ir->ops.o2.op1;
            Operand* op2 = ir->ops.o2.op2;
            int reg1 = getReg(op1, fp);
            int reg2 = getReg(op2, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  sw %s, 0(%s)\n", regs[reg1].name, regs[reg2].name);
            fputs(str, fp);
            break;
        }
        case Goto_IR:{
            char* target = ir->ops.o1.op1->value;
            char str[30];
            memset(str, 0, sizeof(str));
            sprintf(str, "  j %s\n", target);
            fputs(str, fp);
            break;
        }
        case ConJump_IR:{
            Operand* op1 = ir->ops.conJump.op1;
            Operand* op2 = ir->ops.conJump.op2;
            Operand* op3 = ir->ops.conJump.op3;
            char* relop = ir->ops.conJump.relop;
            int reg1 = getReg(op1, fp);
            int reg2 = getReg(op2, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            if(strcmp(relop,"==")==0){
                sprintf(str, "  beq %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else if(strcmp(relop,"!=")==0){
                sprintf(str, "  bne %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else if(strcmp(relop,">")==0){
                sprintf(str, "  bgt %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else if(strcmp(relop,"<")==0){
                sprintf(str, "  blt %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else if(strcmp(relop,">=")==0){
                sprintf(str, "  bge %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else if(strcmp(relop,"<=")==0){
                sprintf(str, "  ble %s, %s, %s\n", regs[reg1].name, regs[reg2].name, op3->value);
                fputs(str, fp);
            }else{
                assert(0);
            }
            break;
        }
        case Return_IR:{
            Operand* op1 = ir->ops.o1.op1;
            if(op1->type == Constant){
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  move $v0, $%s\n", subStr(op1->value, 1));
                fputs(str, fp);

                memset(str, 0, sizeof(str));
                sprintf(str, "  jr $ra\n");
                fputs(str, fp);

            }else{
                int reg1 = getReg(op1, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  move $v0, %s\n", regs[reg1].name);
                fputs(str, fp);
                
                memset(str, 0, sizeof(str));
                sprintf(str, "  jr $ra\n");
                fputs(str, fp);
            }
            break;
        }
        case DecMemory_IR:break;
        case Arg_IR: break;
        case Call_IR:{
            int argNum = countArgList();

            if(argNum == 0){
                Operand* op1 = ir->ops.o2.op1;
                Operand* op2 = ir->ops.o2.op2;
                int reg1 = getReg(op1, fp);
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  jal %s\n", op2->value);
                fputs(str, fp);

                memset(str, 0, sizeof(str));
                sprintf(str, "  move %s, $v0\n", regs[reg1].name);
                fputs(str, fp);
            }else{
                doCall(ir, fp);
            }
            
            /*
            else if(argNum == 1){
                int stackSpace = 4 * argNum + 4;


            }else if(argNum == 2){
                // allocate stack space
                int stackSpace = 4 * argNum + 4;
                char str[30];
                memset(str, 0, sizeof(str));
                sprintf(str, "  addi $sp, $sp, -%d\n", stackSpace);
                fputs(str, fp);

                // save register into stack
                fputs("  sw $ra, 0($sp)\n", fp);
                fputs("  sw $a0, 4($sp)\n", fp);
                fputs("  sw $a1, 8($sp)\n", fp);

                // pass args reversely
                InterCodeNode* p = objectArgListHead;
                Operand* arg2 = p->interCode->ops.o1.op1;
                Operand* arg1 = p->next->interCode->ops.o1.op1;

                int regIndex1 = getReg(arg1, fp);
                int regIndex2 = getReg(arg2, fp);
                memset(str, 0, sizeof(str));
                sprintf(str, "  move $a0, %s\n", regs[regIndex1].name);
                fputs(str, fp);
                memset(str, 0, sizeof(str));
                sprintf(str, "  move $a1, %s\n", regs[regIndex2].name);
                fputs(str, fp);

                // save current temporary registers
                saveAllVarIntoStack(fp);

                // call function
                Operand* op1 = ir->ops.o2.op1;
                Operand* op2 = ir->ops.o2.op2;
                memset(str, 0, sizeof(str));
                sprintf(str, "  jal %s\n", op2->value);
                fputs(str, fp);

                // restore current temporary registers
                loadAllVarFromStack(fp);

                // restore registers from stack
                fputs("  lw $a1, 8($sp)\n", fp);
                fputs("  lw $a0, 4($sp)\n", fp);
                fputs("  lw $ra, 0($sp)\n", fp);

                // deallocate stack space
                memset(str, 0, sizeof(str));
                sprintf(str, "  addi $sp, $sp, %d\n", stackSpace);
                fputs(str, fp);

                // set return value of function
                int returnRegNo = getReg(op1, fp);
                memset(str, 0, sizeof(str));
                sprintf(str, "  move %s, $v0\n", regs[returnRegNo].name);
                fputs(str, fp);

            }else if(argNum == 3){

            }else if(argNum == 4){

            }
            */
            clearObjectArgList();
            break;
        }
        case Param_IR: break;
        case Read_IR:{
            // save $ra into stack
            fputs("  addi $sp, $sp, -4\n", fp);
            fputs("  sw $ra, 0($sp)\n", fp);
            // jump to read() function
            fputs("  jal read\n", fp);

            int reg = getReg(ir->ops.o1.op1, fp);
            // restore $ra
            fputs("  lw $ra, 0($sp)\n", fp);
            fputs("  addi $sp, $sp, 4\n", fp);
            char str[30];
            memset(str, 0, sizeof(str));
            // get return value of read()
            sprintf(str, "  move %s, $v0\n", regs[reg].name);
            fputs(str, fp);
            break;
        }
        case Write_IR:{
            int regIndex = getReg(ir->ops.o1.op1, fp);
            char str[30];
            memset(str, 0, sizeof(str));
            // pass parameter
            sprintf(str, "  move $a0, %s\n", regs[regIndex].name);
            fputs(str, fp);
            // save $ra into stack
            fputs("  addi $sp, $sp, -4\n", fp);
            fputs("  sw $ra, 0($sp)\n", fp);
            // jump to write() function
            fputs("  jal write\n", fp);
            // restore $ra
            fputs("  lw $ra, 0($sp)\n", fp);
            fputs("  addi $sp, $sp, 4\n", fp);
            break;
        };
        default: {assert(0);break;}
    }
}

void writeAssemblyCode(char* fileName){
    initRegs();

    FILE* fp;
    if(strcmp(fileName, "stdout")==0){
        fp = stdout;
    }else{
        fp = fopen(fileName, "w");
    }
    assert(fp);

    fputs(".data\n", fp);
    fputs("_prompt: .asciiz \"Enter an integer:\"\n", fp);
    fputs("_ret: .asciiz \"\\n\"\n", fp);
    fputs(".globl main\n", fp);
    fputs(".text\n\n", fp);

    fputs("read:\n", fp);
    fputs("  li $v0, 4\n", fp);
    fputs("  la $a0, _prompt\n", fp);
    fputs("  syscall\n", fp);
    fputs("  li $v0, 5\n", fp);
    fputs("  syscall\n", fp);
    fputs("  jr $ra\n\n", fp);

    fputs("write:\n", fp);
    fputs("  li $v0, 1\n", fp);
    fputs("  syscall\n", fp);
    fputs("  li $v0, 4\n", fp);
    fputs("  la $a0, _ret\n", fp);
    fputs("  syscall\n", fp);
    fputs("  move $v0, $0\n", fp);
    fputs("  jr $ra\n\n", fp);
    
    InterCodeNode* p = interCodeListHead;
    while(p){
        if(p->interCode->type == Function_IR){
            int paramCount = 0;
            InterCodeNode* q = p->next;
            while(q->interCode->type == Param_IR){
                insertParam(q->interCode);
                paramCount++;
                q=q->next;
            }
            translateInterCode(p->interCode, fp);
            clearParamList();
            for(int i = 0; i < paramCount; ++i){
                p=p->next;
            }
        }else if(p->interCode->type == Arg_IR){
            insertArg(p->interCode);
            int argCount = 1;
            InterCodeNode* q = p->next;
            while(q->interCode->type == Arg_IR){
                insertArg(q->interCode);
                argCount++;
                q=q->next;
            }
            for(int i = 0; i < argCount-1; ++i){
                p=p->next;
            }
        }else{
            translateInterCode(p->interCode, fp);
        }
        p=p->next;
    }
    fclose(fp);
}

void doCall(InterCode* ir, FILE* fp){
    int argNum = countArgList();

    // allocate stack space
    int stackSpace = 4 * argNum + 4;
    char str[30];
    memset(str, 0, sizeof(str));
    sprintf(str, "  addi $sp, $sp, -%d\n", stackSpace);
    fputs(str, fp);

    // save registers into stack
    fputs("  sw $ra, 0($sp)\n", fp);
    for(int i = 0; i < argNum; ++i){
        memset(str, 0, sizeof(str));
        sprintf(str, "  sw $a%d, %d($sp)\n", i, 4*(i+1));
        fputs(str, fp);
    }

    // pass args reversely
    InterCodeNode* p = objectArgListHead;
    int count = 0;
    while(p){
        Operand* argOperand = p->interCode->ops.o1.op1;
        int regIndex = getReg(argOperand, fp);
        memset(str, 0, sizeof(str));
        sprintf(str, "  move $a%d, %s\n", count, regs[regIndex].name);
        fputs(str, fp);

        p=p->next;
        count++;
    }

    // save current temporary registers
    saveAllVarIntoStack(fp);

    // call function
    Operand* funcOperand = ir->ops.o2.op2;
    memset(str, 0, sizeof(str));
    sprintf(str, "  jal %s\n", funcOperand->value);
    fputs(str, fp);

    // restore current temporary registers
    loadAllVarFromStack(fp);

    // restore registers from stack
    for(int i = argNum-1; i >= 0; i--){
        memset(str, 0, sizeof(str));
        sprintf(str, "  lw $a%d, %d($sp)\n", i, 4*(i+1));
        fputs(str, fp);
    }
    fputs("  lw $ra, 0($sp)\n", fp);

    // deallocate stack space
    memset(str, 0, sizeof(str));
    sprintf(str, "  addi $sp, $sp, %d\n", stackSpace);
    fputs(str, fp);

    // set return value of function
    Operand* retOperand = ir->ops.o2.op1;
    int retRegNo = getReg(retOperand, fp);
    memset(str, 0, sizeof(str));
    sprintf(str, "  move %s, $v0\n", regs[retRegNo].name);
    fputs(str, fp);

}

