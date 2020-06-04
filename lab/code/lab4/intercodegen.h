#ifndef _INTERCODEGEN_H_
#define _INTERCODEGEN_H_

#include "semantic.h"

// type of operand
typedef enum OperandType {
    Variable,
    Param,
    Temp,
    Constant,
    Address,
    Label,
    Function,
    Array
} OperandType;

// type of IR
typedef enum IRType {
    Label_IR,               // label
    Function_IR,            // function
    Assign_IR,              // =
    Plus_IR,                // +
    Minus_IR,               // -
    Multiply_IR,            // *
    Divide_IR,              // /
    Address_IR,             // &
    ReadMemory_IR,          // read memory (x := *y)
    WriteMemory_IR,         // write memory (*x := y)
    Goto_IR,                // goto
    ConJump_IR,             // conditional jump
    Return_IR,              // return statement
    DecMemory_IR,           // ask for memory
    Arg_IR,                 // parameter pass
    Call_IR,                // call function
    Param_IR,               // parameter declaration
    Read_IR,                // read from console
    Write_IR,               // write to console
} IRType;

// Operand
typedef struct Operand {
    OperandType type;
    char* value;
}Operand;


// IR
typedef struct InterCode {
    int n_operand;          // number of operands
    IRType type;            // type of IR
    union{
        // 1 operand
        // label, function, goto, return, arg, param, read, write
        struct {
            Operand* op1;
        }o1;
        // 2 operands
        // assign, getAddr, readMem, writeMem, functionCall
        struct {
            Operand* op1;
            Operand* op2;
        }o2;
        // 3 operands
        // add, minus, multiply, divide
        struct {
            Operand* op1;
            Operand* op2;
            Operand* op3;
        }o3;
        // conditional jump
        struct {
            Operand* op1;
            Operand* op2;
            Operand* op3;
            char* relop;
        }conJump;
        // memory dec
        struct {
            Operand* op1;    // owner of this declared space
            int size;
        }memDec;
    }ops;
}InterCode;

typedef struct InterCodeNode{
    InterCode* interCode;
    struct InterCodeNode* next;
    // struct InterCodeNode* prev;
}InterCodeNode;

// variable name map of source code and intercode
typedef struct NameMap{
    char* varName;
    char* interCodeVarName;
}NameMap;

typedef struct NameMapNode{
    NameMap nameMap;
    struct NameMapNode* next;
}NameMapNode;

typedef struct ArrayDesc{
    char* arrayName;
    int n_dim;
    int dim1;
    int dim2;
}ArrayDesc;

typedef struct ArrayDescNode{
    ArrayDesc* arrayDesc;
    struct ArrayDescNode* next;
}ArrayDescNode;

typedef struct SpecialParam{
    char* name;
    bool ifAddr;
}SpecialParam;

typedef struct SpecialParamNode{
    SpecialParam* param;
    struct SpecialParamNode* next;
}SpecialParamNode;

void insertArithmic(Operand* place, Operand* o1, Operand* o2, char* type);

void insertSpecialParam(char* name, bool ifAddr);
SpecialParam* getSpecialParam(char* name);

void insertArrayDesc(char* arrayName, int n_dim, int dim1, int dim2);
ArrayDesc* getArrayDesc(char* arrayName);

void insertInterCode(InterCode* interCode);
void writeInterCode(char* fileName);
void printInterCode();
void showAllInterCode();
char* getFormatStr(const char* prefix, int num);

void insertNameMap(char* varName, char* interCodeVarName);
void showAllNameMap();
char* getInterCodeVarName(char* varName);
char* getSourceCodeVarName(char* interCodeVarName);

// translate functions
void translate_Program(Node* program);
void translate_ExtDefList(Node* extdeflist);
void translate_ExtDef(Node* extdef);
void translate_ExtDecList(Node* extdeclist);
void translate_Specifier(Node* specifier);
void translate_OptTag(Node* opttag);
void translate_VarDec(Node* vardec);
void translate_FunDec(Node* fundec);
void translate_VarList(Node* varlist);
void translate_ParamDec(Node* paramdec);
void translate_CompSt(Node* compst);
void translate_StmtList(Node* stmtlist);
void translate_Stmt(Node* stmt);
void translate_DefList(Node* deflist);
void translate_Def(Node* def);
void translate_DecList(Node* declist);
void translate_Dec(Node* dec, char* varName);

void translate_Exp(Node* exp, Operand* place);
void translate_Cond(Node* exp, Operand* label_true, Operand* label_false);
void translate_Args(Node* args);

void translate_ArrayArg(char* arrayName);

void translate_ArrayAddr(Node* exp, Operand* place);

Operand* createLabel();
void insertLabelInterCode(Operand* label);
Operand* createTemp();
Operand* createNumber(int n);
Operand* createVar(char* id);

typedef struct ArgListNode{
    Operand* operand;
    struct ArgListNode* next;
}ArgListNode;

void insertIntoArgList(Operand* operand);
void clearArgList();

Operand* copyOperand(Operand* src);
InterCode* copyInterCode(InterCode* src);

char* getDecName(Node* dec);
char* getVarDecName(Node* vardec);
char* getArrayExpName(Node* exp);

void optimize();
#endif
