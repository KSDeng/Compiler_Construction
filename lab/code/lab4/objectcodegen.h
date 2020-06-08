#ifndef _OBJECTCODEGEN_H_
#define _OBJECTCODEGEN_H_

#include "intercodegen.h"

#define STK_SIZE 100

typedef struct VarDesc{
    Operand* op;
    int reg_no;
}VarDesc;

typedef struct Register{
    VarDesc* content;
    char* name;
    int index;
    int age;
}Register;

typedef struct StackDesc{
    VarDesc* stack[STK_SIZE];
    int top;
}StackDesc;


void initRegs();
void initStk();
void pushStack(VarDesc* vardesc);
VarDesc* popStack();
int getReg(Operand* operand, FILE* fp);
void writeAssemblyCode(char* fileName);


VarDesc* copyVarDesc(VarDesc* src);
void saveVarIntoStack(VarDesc* var, FILE* fp);
VarDesc* loadVarFromStackTop(FILE* fp);

void saveAllVarIntoStack(FILE* fp);
void loadAllVarFromStack(FILE* fp);
void translateInterCode(InterCode* ir, FILE* fp);

bool compareOperand(Operand* o1, Operand* o2);
void showAllRegs();

#endif
