#ifndef _INTERCODEGEN_H_
#define _INTERCODEGEN_H_

#include "parseTree.h"

// type of operand
typedef enum OperandType {
    Variable,
    TempVariable,
    Constant,
    Address,
    TempAddress,
    Label,
    Function,
    Debug
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
    Write_IR                // write to console
} IRType;

// Operand
typedef struct Operand {
    OperandType type;
    union {
        int intValue;          // variable, temp, constant
        char* strValue;         // function, label
    }value;
}Operand;

// IR
typedef struct InterCode {
    int n_operand;          // number of operands
    IRType type;            // type of IR
    union{
        // 1 operand
        // label, function, goto, return, arg, param, read, write
        struct {
            Operand op1;
        }o1;
        // 2 operands
        // assign, getAddr, readMem, writeMem, functionCall
        struct {
            Operand op1;
            Operand op2;
        }o2;
        // 3 operands
        // add, minus, multiply, divide
        struct {
            Operand op1;
            Operand op2;
            Operand op3;
        }o3;
        // conditional jump
        struct {
            Operand op1;
            Operand op2;
            Operand op3;
            char* relop;
        }conJump;
        // memory dec
        struct {
            Operand op1;    // what does this operand do?
            int size;
        }memDec;
    }ops;
}InterCode;

typedef struct InterCodeNode{
    InterCode intercode;
    struct InterCodeNode* next;
    struct InterCodeNode* prev;
}InterCodeNode;


//void initIRList();
void insertInterCode(InterCode interCode);
void processOperand(Operand operand);
void genInterCodeTraverse(TreeNode* root);
void writeInterCode(char* fileName);
#endif
