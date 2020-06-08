#include <stdlib.h>
#include <assert.h>
#include "semantic.h"
#include "intercodegen.h"

bool debug_intercode = true;
// intercode list head
InterCodeNode* interCodeListHead = NULL;
// variable names map list head
NameMapNode* nameMapListHead = NULL;
// args list head
ArgListNode* argListHead = NULL;
// array description list head
ArrayDescNode* arrayDescListHead = NULL;
// special param list head
SpecialParamNode* specialParamListHead = NULL;

// intercode var name count
int labelCount = 0;     // labelx
int tempCount = 0;      // tx
int varCount = 0;       // vx
int funcCount = 0;      // fx
int paramCount = 0;     // px

void insertSpecialParam(char* name, bool ifAddr){
    SpecialParam* param = (SpecialParam*)malloc(sizeof(SpecialParam));
    param->name = (char*)malloc(strlen(name)+1);
    strcpy(param->name, name);
    param->ifAddr = ifAddr;

    SpecialParamNode* p = (SpecialParamNode*)malloc(sizeof(SpecialParamNode));
    p->param = param;
    p->next = NULL;

    if(specialParamListHead == NULL){
        specialParamListHead = p;
    }else{
        SpecialParamNode* q = specialParamListHead;
        while(q->next) q = q->next;
        q->next = p;
    }
}

SpecialParam* getSpecialParam(char* name){
    SpecialParamNode* p = specialParamListHead;
    while(p){
        SpecialParam* param = p->param;
        if(strcmp(param->name, name) == 0){
            return param;
        }
        p = p->next;
    }
    return NULL;
}

void insertArrayDesc(char* arrayName, int n_dim, int dim1, int dim2){
    ArrayDesc* desc = (ArrayDesc*)malloc(sizeof(ArrayDesc));
    desc->arrayName = (char*)malloc(strlen(arrayName)+1);
    strcpy(desc->arrayName, arrayName);
    desc->n_dim = n_dim;
    desc->dim1 = dim1;
    desc->dim2 = dim2;

    ArrayDescNode* p = (ArrayDescNode*)malloc(sizeof(ArrayDescNode));
    p->arrayDesc = desc;
    p->next = NULL;
    if(arrayDescListHead == NULL){
        arrayDescListHead = p;
    }else{
        ArrayDescNode* q = arrayDescListHead;
        while(q->next) q = q->next;
        q->next = p;
    }
}

ArrayDesc* getArrayDesc(char* arrayName){
    ArrayDescNode* p = arrayDescListHead;
    while(p){
        ArrayDesc* desc = p->arrayDesc;
        if(strcmp(desc->arrayName, arrayName) == 0){
            return desc;
        }
        p = p->next;
    }
    return NULL;
}

Operand* copyOperand(Operand* src){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->value = (char*)malloc(strlen(src->value)+1);
    strcpy(res->value, src->value);
    res->type = src->type;
    return res;
}

// insert a new node to the head of arglist
void insertIntoArgList(Operand* operand){
    ArgListNode* argNode = (ArgListNode*)malloc(sizeof(ArgListNode));
    argNode->next = NULL;
    argNode->operand = copyOperand(operand);
    if(argListHead == NULL){
        argListHead = argNode;
    }else{
        argNode->next = argListHead;
        argListHead = argNode;
    }
}

void clearArgList(){
    if(argListHead == NULL){
        return;
    }
    ArgListNode* p = argListHead;
    while(p->next){
        ArgListNode* nextAddr = p->next;
        free(p);
        p = nextAddr;
    }
    free(p);
    p = NULL;
    argListHead = NULL;
}

void insertInterCode(InterCode* interCode){
    InterCodeNode* p = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    p->interCode = interCode;
    p->next = NULL;

    if(!interCodeListHead){
        //p->prev = NULL;
        interCodeListHead = p;
    }else{
        InterCodeNode* q = interCodeListHead;
        while(q->next) q = q->next;
        //p->prev = q;
        q->next = p;
    }
    // writeInterCode("stdout");
}

void showAllInterCode(){
    InterCodeNode* p = interCodeListHead;
    int count = 0;
    while(p){
        InterCode* ic = p->interCode;
        count++;
        printf("type: %d, n_operand: %d\n", ic->type, ic->n_operand);
        p = p->next;
    }
    printf("%d intercode in total.\n", count);
}
void replaceLabel(char* oldLabel, char* newLabel){
    InterCodeNode* p = interCodeListHead;
    while(p){
        InterCode* ir = p->interCode;
        if(ir->type == Label_IR){
            Operand* op = ir->ops.o1.op1;
            if(strcmp(op->value, oldLabel)==0){
                p->interCode->ops.o1.op1->value = (char*)malloc(strlen(newLabel)+1);
                strcpy(p->interCode->ops.o1.op1->value, newLabel);
            }
        }else if(ir->type == Goto_IR){
            Operand* op = ir->ops.o1.op1;
            if(strcmp(op->value, oldLabel)==0){
                p->interCode->ops.o1.op1->value = (char*)malloc(strlen(newLabel)+1);
                strcpy(p->interCode->ops.o1.op1->value, newLabel);
            }
        }else if(ir->type == ConJump_IR){
            Operand* op = ir->ops.conJump.op3;
            if(strcmp(op->value, oldLabel)==0){
                p->interCode->ops.conJump.op3->value = (char*)malloc(strlen(newLabel)+1);
                strcpy(p->interCode->ops.conJump.op3->value, newLabel);
            }
        }
        p=p->next;
    }
}

void optimize(){
    InterCodeNode* p = interCodeListHead;
    while(p){
        // redundant label
        // Label label1:
        // Label label2:
        if(p->next && p->interCode->type == Label_IR && p->next->interCode->type == Label_IR){
            Operand* opLabel1 = p->interCode->ops.o1.op1;
            char* remainLabel = opLabel1->value;
            Operand* opLabel2 = p->next->interCode->ops.o1.op1;
            char* deleteLabel = opLabel2->value;

            InterCodeNode* tmp = p->next;
            p->next = p->next->next;
            free(tmp);
            replaceLabel(deleteLabel, remainLabel);
            continue;
        }

        // label with goto
        // Label label1:
        // Goto label2
        if(p->next && p->next->next && p->next->interCode->type == Label_IR && p->next->next->interCode->type == Goto_IR){
            Operand* opLabel1 = p->next->interCode->ops.o1.op1;
            char* deleteLabel = opLabel1->value;
            Operand* opLabel2 = p->next->next->interCode->ops.o1.op1;
            char* remainLabel = opLabel2->value;

            InterCodeNode* tmp = p->next;
            p->next = p->next->next;
            free(tmp);
            replaceLabel(deleteLabel, remainLabel);
            continue;
        }

        p=p->next;
    }

}

void writeInterCode(char* fileName){
    FILE* fp;
    if(strcmp(fileName, "stdout") == 0){
        fp = stdout;
    }else {
        fp = fopen(fileName, "w");
    }
    assert(fp);

    optimize();
    InterCodeNode* p = interCodeListHead;
    char content[100];
    memset(content, 0, sizeof(content));
    while(p){
        InterCode* ic = p->interCode;
        switch(ic->type){
            case Label_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "LABEL %s :\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case Function_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "FUNCTION %s :\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case Assign_IR:
                assert(ic->n_operand == 2);
                sprintf(content, "%s := %s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                fputs(content, fp);
                break;
            case Plus_IR:
                assert(ic->n_operand == 3);
                sprintf(content, "%s := %s + %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                fputs(content, fp);
                break;
            case Minus_IR:
                assert(ic->n_operand == 3);
                sprintf(content, "%s := %s - %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                fputs(content, fp);
                break;
            case Multiply_IR:
                assert(ic->n_operand == 3);
                sprintf(content, "%s := %s * %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                fputs(content, fp);
                break;
            case Divide_IR:
                assert(ic->n_operand == 3);
                sprintf(content, "%s := %s / %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                fputs(content, fp);
                break;
            case Address_IR:
                assert(ic->n_operand == 2);
                sprintf(content, "%s := &%s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                fputs(content, fp);
                break;
            case ReadMemory_IR:
                assert(ic->n_operand == 2);
                sprintf(content, "%s := *%s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                fputs(content, fp);
                break;
            case WriteMemory_IR:
                assert(ic->n_operand == 2);
                sprintf(content, "*%s := %s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                fputs(content, fp);
                break;
            case Goto_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "GOTO %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case ConJump_IR:{
                assert(ic->n_operand == 3);
                char* s1 = ic->ops.conJump.op1->value;
                char* relop = ic->ops.conJump.relop;
                char* s2 = ic->ops.conJump.op2->value;
                char* s3 = ic->ops.conJump.op3->value;
                sprintf(content, "IF %s %s %s GOTO %s\n", s1, relop, s2, s3);
                fputs(content, fp);
                break;}
            case Return_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "RETURN %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case DecMemory_IR:{
                assert(ic->n_operand == 1);
                char* s1 = ic->ops.memDec.op1->value;
                int size = ic->ops.memDec.size;
                sprintf(content, "DEC %s %d\n", s1, size);
                fputs(content, fp);
                break;}
            case Arg_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "ARG %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case Call_IR:{
                assert(ic->n_operand == 2);
                char* s1 = ic->ops.o2.op1->value;
                char* s2 = ic->ops.o2.op2->value;
                sprintf(content, "%s := CALL %s\n", s1, s2);
                fputs(content, fp);
                break;}
            case Param_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "PARAM %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case Read_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "READ %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            case Write_IR:
                assert(ic->n_operand == 1);
                sprintf(content, "WRITE %s\n", ic->ops.o1.op1->value);
                fputs(content, fp);
                break;
            default: assert(0);
        }
        memset(content, 0, sizeof(content));
        p = p->next;
    }
    fclose(fp);
}
void printInterCode(){
    InterCodeNode* p = interCodeListHead;
    while(p){
        InterCode* ic = p->interCode;
        switch(ic->type){
            case Label_IR:
                assert(ic->n_operand == 1);
                printf("LABEL %s :\n", ic->ops.o1.op1->value);
                break;
            case Function_IR:
                assert(ic->n_operand == 1);
                printf("FUNCTION %s :\n", ic->ops.o1.op1->value);
                break;
            case Assign_IR:
                assert(ic->n_operand == 2);
                printf("%s := %s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                break;
            case Plus_IR:
                assert(ic->n_operand == 3);
                printf("%s := %s + %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                break;
            case Minus_IR:
                assert(ic->n_operand == 3);
                printf("%s := %s - %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                break;
            case Multiply_IR:
                assert(ic->n_operand == 3);
                printf("%s := %s * %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                break;
            case Divide_IR:
                assert(ic->n_operand == 3);
                printf("%s := %s / %s\n", ic->ops.o3.op1->value, ic->ops.o3.op2->value, ic->ops.o3.op3->value);
                break;
            case Address_IR:
                assert(ic->n_operand == 2);
                printf("%s := &%s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                break;
            case ReadMemory_IR:
                assert(ic->n_operand == 2);
                printf("%s := *%s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                break;
            case WriteMemory_IR:
                assert(ic->n_operand == 2);
                printf("*%s := %s\n", ic->ops.o2.op1->value, ic->ops.o2.op2->value);
                break;
            case Goto_IR:
                assert(ic->n_operand == 1);
                printf("GOTO %s\n", ic->ops.o1.op1->value);
                break;
            case ConJump_IR:{
                assert(ic->n_operand == 3);
                char* s1 = ic->ops.conJump.op1->value;
                char* relop = ic->ops.conJump.relop;
                char* s2 = ic->ops.conJump.op2->value;
                char* s3 = ic->ops.conJump.op3->value;
                printf("IF %s %s %s GOTO %s\n", s1, relop, s2, s3);
                break;}
            case Return_IR:
                assert(ic->n_operand == 1);
                printf("RETURN %s\n", ic->ops.o1.op1->value);
                break;
            case DecMemory_IR:{
                assert(ic->n_operand == 1);
                char* s1 = ic->ops.memDec.op1->value;
                int size = ic->ops.memDec.size;
                printf("DEC %s %d\n", s1, size);
                break;}
            case Arg_IR:
                assert(ic->n_operand == 1);
                printf("ARG %s\n", ic->ops.o1.op1->value);
                break;
            case Call_IR:{
                assert(ic->n_operand == 2);
                char* s1 = ic->ops.o2.op1->value;
                char* s2 = ic->ops.o2.op2->value;
                printf("%s := CALL %s\n", s1, s2);
                break;}
            case Param_IR:
                assert(ic->n_operand == 1);
                printf("PARAM %s\n", ic->ops.o1.op1->value);
                break;
            case Read_IR:
                assert(ic->n_operand == 1);
                printf("READ %s\n", ic->ops.o1.op1->value);
                break;
            case Write_IR:
                assert(ic->n_operand == 1);
                printf("WRITE %s\n", ic->ops.o1.op1->value);
                break;
            default: assert(0);
        }
        p = p->next;
    }
}

char* getFormatStr(const char* prefix, int num){
    char* tmp = (char*)malloc(30);
    sprintf(tmp, "%s%d", prefix, num);
    return tmp;
}

void insertNameMap(char* varName, char* interCodeVarName){
    NameMap* nm = (NameMap*)malloc(sizeof(NameMap));
    nm->varName = (char*)malloc(strlen(varName)+1);
    strcpy(nm->varName, varName);
    nm->interCodeVarName = (char*)malloc(strlen(interCodeVarName)+1);
    strcpy(nm->interCodeVarName, interCodeVarName);

    NameMapNode* p = (NameMapNode*)malloc(sizeof(NameMapNode));
    p->next = NULL;
    p->nameMap = *nm;

    if(nameMapListHead == NULL){
        nameMapListHead = p;
    }else{
        NameMapNode* q = nameMapListHead;
        while(q->next) q = q->next;
        q->next = p;
    }
}

void showAllNameMap(){
    NameMapNode* p = nameMapListHead;
    while(p){
        NameMap m = p->nameMap;
        printf("%s -> %s\n", m.varName, m.interCodeVarName);
        p=p->next;
    }
}

char* getInterCodeVarName(char* varName){
    NameMapNode* p = nameMapListHead;
    while(p){
        NameMap m = p->nameMap;
        if(strcmp(m.varName, varName) == 0){
            return m.interCodeVarName;
        }
        p = p->next;
    }
    assert(0);
    return NULL;
}

char* getSourceCodeVarName(char* interCodeVarName){
    NameMapNode* p = nameMapListHead;
    while(p){
        NameMap m = p->nameMap;
        if(strcmp(m.interCodeVarName, interCodeVarName) == 0){
            return m.varName;
        }
        p = p->next;
    }
}

// Translate Functions

void translate_Program(Node* program){
    translate_ExtDefList(program->children[0]);
    return;
}
void translate_ExtDefList(Node* extdeflist){
    if(!extdeflist){
        return;
    }else if(extdeflist->n_children == 2){
        translate_ExtDef(extdeflist->children[0]);
        translate_ExtDefList(extdeflist->children[1]);
        return;
    }else{
        assert(0);
    }
}
void translate_ExtDef(Node* extdef){
    if(extdef->n_children == 3){
        if(strcmp(extdef->children[1]->name, "ExtDecList") == 0){   // ExtDef -> Specifier ExtDecList SEMI
            translate_Specifier(extdef->children[0]);
            translate_ExtDecList(extdef->children[1]);
        }else if(strcmp(extdef->children[1]->name, "FunDec") == 0){ // ExtDef -> Specifier FunDec CompSt
            translate_Specifier(extdef->children[0]);
            translate_FunDec(extdef->children[1]);
            translate_CompSt(extdef->children[2]);
        }else{
            assert(0);
        }
    }else if(extdef->n_children == 2){  // ExtDef -> Specifier SEMI
        translate_Specifier(extdef->children[0]);
    }else{
        assert(0);
    }
}
void translate_ExtDecList(Node* extdeclist){
    if(extdeclist->n_children == 1){
        translate_VarDec(extdeclist->children[0]);
    }else if(extdeclist->n_children == 3){
        translate_VarDec(extdeclist->children[0]);
        translate_ExtDecList(extdeclist->children[2]);
    }else{
        assert(0);
    }
}
void translate_Specifier(Node* specifier){
    if(strcmp(specifier->children[0]->name, "TYPE") == 0){
        return;
    }else if(strcmp(specifier->children[0]->name, "StructSpecifier")==0){
        printf("Cannot translate: Code contains varialbes or parameters of structure type.\n");
        exit(0);
    }
}
void translate_OptTag(Node* opttag){

}
int getArraySize(Node* vardec){
    int sizeCount = 1;
    while(vardec->n_children == 4){
        int temp = atoi(vardec->children[2]->propertyValue);
        sizeCount = sizeCount * temp;
        vardec = vardec->children[0];
    }
    return sizeCount * 4;
}

void translate_VarDec(Node* vardec){
    if(vardec->n_children == 1){    // VarDec -> ID
        return;
    }else if(vardec->n_children == 4){  // VarDec -> VarDec LB INT RB
        char* varName = getVarDecName(vardec);
        int n_count = 1;
        int dim1 = 0, dim2 = 0;
        dim2 = atoi(vardec->children[2]->propertyValue);

        Node* subVardec = vardec->children[0];
        while(subVardec->n_children == 4){
            n_count++;
            dim1 = atoi(subVardec->children[2]->propertyValue);
            subVardec = subVardec->children[0];
        }
        if(n_count == 1){       // 1-dimensional array
            insertArrayDesc(varName, 1, -1, dim2);
            int size = getArraySize(vardec);
            Operand* varOperand = (Operand*)malloc(sizeof(Operand));
            varOperand->value = (char*)malloc(strlen(varName)+1);
            strcpy(varOperand->value, varName);
            varOperand->type = Array;

            InterCode* decIR = (InterCode*)malloc(sizeof(InterCode));
            decIR->n_operand = 1;
            decIR->type = DecMemory_IR;
            decIR->ops.memDec.op1 = varOperand;
            decIR->ops.memDec.size = size;
            insertInterCode(decIR);

        }else if(n_count == 2){ // 2-dimensional array
            insertArrayDesc(varName, 2, dim1, dim2);
            int size = getArraySize(vardec);
            Operand* varOperand = (Operand*)malloc(sizeof(Operand));
            varOperand->value = (char*)malloc(strlen(varName)+1);
            strcpy(varOperand->value, varName);
            varOperand->type = Array;

            InterCode* decIR = (InterCode*)malloc(sizeof(InterCode));
            decIR->n_operand = 1;
            decIR->type = DecMemory_IR;
            decIR->ops.memDec.op1 = varOperand;
            decIR->ops.memDec.size = size;
            insertInterCode(decIR);
        }else{          // 3 or more dimensional array
            assert(0);
        }
    }else{
        assert(0);
    }
}
void translate_VarList(Node* varlist){
    if(varlist->n_children == 3){   // VarList -> ParamDec COMMA VarList
        translate_ParamDec(varlist->children[0]);
        translate_VarList(varlist->children[2]);
    }else if(varlist->n_children == 1){ // VarList -> ParamDec
        translate_ParamDec(varlist->children[0]);
    }else{
        assert(0);
    }
}

void translate_ParamDec(Node* paramdec){
    // ParamDec -> Specifier VarDec
    char* varName = getVarDecName(paramdec->children[1]);
    Operand* paramOp = (Operand*)malloc(sizeof(Operand));
    paramOp->value = (char*)malloc(strlen(varName)+1);
    strcpy(paramOp->value, varName);
    paramOp->type = Param;

    InterCode* paramIR = (InterCode*)malloc(sizeof(InterCode));
    paramIR->n_operand = 1;
    paramIR->type = Param_IR;
    paramIR->ops.o1.op1 = paramOp;
    insertInterCode(paramIR);
    if(paramdec->children[1]->n_children == 4){ // array param
        insertSpecialParam(varName, true);
    }
}

void translate_CompSt(Node* compst){
    // CompSt -> LC DefList StmtList RC
    translate_DefList(compst->children[1]);
    translate_StmtList(compst->children[2]);
}

void translate_StmtList(Node* stmtlist){
    if(!stmtlist){
        return;
    }else if(stmtlist->n_children == 2){
        translate_Stmt(stmtlist->children[0]);
        translate_StmtList(stmtlist->children[1]);
    }else{
        assert(0);
    }
}

void translate_DefList(Node* deflist){
    if(!deflist){       // DefList -> empty
        return;
    }else if(deflist->n_children == 2){   // DefList -> Def DefList
        translate_Def(deflist->children[0]);
        translate_DefList(deflist->children[1]);
    }else{
        assert(0);
    }
}

void translate_Def(Node* def){
    // Def -> Specifier DecList SEMI
    translate_Specifier(def->children[0]);
    translate_DecList(def->children[1]);
}

char* getDecName(Node* dec){
    if(dec->n_children == 1){
        return getVarDecName(dec->children[0]);
    }else if(dec->n_children == 3){
        return getVarDecName(dec->children[0]);
    }else{
        assert(0);
    }
}
char* getVarDecName(Node* vardec){
    if(vardec->n_children == 1){    // VarDec -> ID
        char* varName = vardec->children[0]->propertyValue;
        return varName;
    }else if(vardec->n_children == 4){  // VarDec -> VarDec LB INT RB
        return getVarDecName(vardec->children[0]);
    }else{
        assert(0);
    }
}

void translate_DecList(Node* declist){
    if(declist->n_children == 1){   // DecList -> Dec
        Node* dec = declist->children[0];
        char* varName = getDecName(dec);
        translate_Dec(declist->children[0], varName);
    }else if(declist->n_children == 3){ // DecList -> Dec COMMA DecList
        char* varName = getDecName(declist->children[0]);
        translate_Dec(declist->children[0], varName);
        translate_DecList(declist->children[2]);
    }else{
        assert(0);
    }
}

char* getArrayExpName(Node* exp){
    if(exp->n_children == 4 && strcmp(exp->children[1]->name, "LB")==0){    // Exp -> Exp LB Exp RB
        return getArrayExpName(exp->children[0]);
    }else if(exp->n_children == 1 && strcmp(exp->children[0]->name, "ID")==0){  // Exp -> ID

        char* id = exp->children[0]->propertyValue;
        return id;
    }else{
        assert(0);
    }
}
void translate_Exp(Node* exp, Operand* placeOperand){
    switch(exp->n_children){
        case 1:{
            if(strcmp(exp->children[0]->name, "ID") == 0){  // Exp -> ID

                // intercode
                char* id = exp->children[0]->propertyValue;

                Operand* varOperand = (Operand*)malloc(sizeof(Operand));
                varOperand->value = (char*)malloc(strlen(id)+1);
                strcpy(varOperand->value, id);
                varOperand->type = Variable;

                InterCode* expID = (InterCode*)malloc(sizeof(InterCode));
                expID->n_operand = 2;
                expID->type = Assign_IR;
                expID->ops.o2.op1 = placeOperand;
                expID->ops.o2.op2 = varOperand;
                insertInterCode(expID);
                return;
            }else if(strcmp(exp->children[0]->name, "INT") == 0){  // Exp -> INT
                
                // intercode 
                int intValue = atoi(exp->children[0]->propertyValue);
                Operand* intOperand = createNumber(intValue);

                InterCode* intIR = (InterCode*)malloc(sizeof(InterCode));
                intIR->n_operand = 2;
                intIR->type = Assign_IR;
                intIR->ops.o2.op1 = placeOperand;
                intIR->ops.o2.op2 = intOperand;
                insertInterCode(intIR);

                return;
            }else if(strcmp(exp->children[0]->name, "FLOAT") == 0){ // Exp -> FLOAT
                assert(0);
                return;
            }else{
                printf("Exp() case 1, error with unknown production\n");
                return;
            }
            break;
        }
        case 2:{
            if(strcmp(exp->children[0]->name, "MINUS") == 0){   // Exp -> MINUS Exp
                Operand* t1 = createTemp();

                // code1
                translate_Exp(exp->children[1], t1);
                // code2
                Operand* zero = createNumber(0);

                InterCode* minusIR = (InterCode*)malloc(sizeof(InterCode));
                minusIR->type = Minus_IR;
                minusIR->n_operand = 3;
                minusIR->ops.o3.op1 = placeOperand;
                minusIR->ops.o3.op2 = zero;
                minusIR->ops.o3.op3 = t1;
                insertInterCode(minusIR);

                return;
            }else if(strcmp(exp->children[0]->name, "NOT") == 0){   // Exp -> NOT Exp
                 Operand* label1 = createLabel();
                 Operand* label2 = createLabel();

                 // code0
                 Operand* zero = createNumber(0);

                 InterCode* code0 = (InterCode*)malloc(sizeof(InterCode));
                 code0->type = Assign_IR;
                 code0->n_operand = 2;
                 code0->ops.o2.op1 = placeOperand;
                 code0->ops.o2.op2 = zero;
                 insertInterCode(code0);

                 // code1
                 translate_Cond(exp, label1, label2);

                 // code2
                 insertLabelInterCode(label1);

                 Operand* one = createNumber(1);
                 InterCode* code2_2 = (InterCode*)malloc(sizeof(InterCode));
                 code2_2->type = Assign_IR;
                 code2_2->n_operand = 2;
                 code2_2->ops.o2.op1 = placeOperand;
                 code2_2->ops.o2.op2 = one;
                 insertInterCode(code2_2);

                 insertLabelInterCode(label2);

                 return;
            }else{
                printf("Exp() case 2, error with unknown production\n");
                return;
            }
            break;
        }
        case 3:{
            if(strcmp(exp->children[1]->name, "ASSIGNOP") == 0){    // Exp -> Exp ASSIGNOP Exp
                Node* leftExp = exp->children[0];
                Node* rightExp = exp->children[2];
                if(leftExp->n_children == 1 && strcmp(leftExp->children[0]->name, "ID")==0){    //left side: Exp -> ID
                    if(rightExp->n_children==1 && strcmp(rightExp->children[0]->name, "INT")==0){   // optimize
                        char* id = leftExp->children[0]->propertyValue;
                        Operand* varOperand = (Operand*)malloc(sizeof(Operand));
                        varOperand->value = (char*)malloc(strlen(id)+1);
                        strcpy(varOperand->value, id);
                        varOperand->type = Variable;

                        int intValue = atoi(rightExp->children[0]->propertyValue);
                        Operand* intOperand = createNumber(intValue);

                        InterCode* ic1 = (InterCode*)malloc(sizeof(InterCode));
                        ic1->n_operand = 2;
                        ic1->type = Assign_IR;
                        ic1->ops.o2.op1 = varOperand;
                        ic1->ops.o2.op2 = intOperand;
                        insertInterCode(ic1);

                    }else if(rightExp->n_children==1 && strcmp(rightExp->children[0]->name, "ID")==0){
                        char* id1 = leftExp->children[0]->propertyValue;
                        char* id2 = rightExp->children[0]->propertyValue;
                        Operand* left = (Operand*)malloc(sizeof(Operand));
                        left->value = (char*)malloc(strlen(id1)+1);
                        strcpy(left->value, id1);
                        left->type = Variable;

                        Operand* right = (Operand*)malloc(sizeof(Operand));
                        right->value = (char*)malloc(strlen(id2)+1);
                        strcpy(right->value, id2);
                        right->type = Variable;

                        InterCode* ir = (InterCode*)malloc(sizeof(InterCode));
                        ir->n_operand = 2;
                        ir->type = Assign_IR;
                        ir->ops.o2.op1 = left;
                        ir->ops.o2.op2 = right;
                        insertInterCode(ir);
                    }else{ 
                        char* id = leftExp->children[0]->propertyValue;
                        Operand* temp = createTemp();

                        // code1
                        translate_Exp(exp->children[2], temp);

                        // code2
                        Operand* varOperand = (Operand*)malloc(sizeof(Operand));
                        varOperand->value = (char*)malloc(strlen(id)+1);
                        strcpy(varOperand->value, id);
                        varOperand->type = Variable;

                        InterCode* ic1 = (InterCode*)malloc(sizeof(InterCode));
                        ic1->n_operand = 2;
                        ic1->type = Assign_IR;
                        ic1->ops.o2.op1 = varOperand;
                        ic1->ops.o2.op2 = temp;
                        insertInterCode(ic1);
                    }
                }else if(leftExp->n_children == 4 && strcmp(leftExp->children[1]->name, "LB")==0){  // left side: Exp -> Exp LB Exp RB
                    Operand* rightSideTemp = createTemp();
                    // code1
                    translate_Exp(exp->children[2], rightSideTemp);
                    // code2: get target addr
                    Operand* targetAddr = createTemp();
                    translate_ArrayAddr(exp->children[0], targetAddr);

                    // set value to target addr
                    InterCode* writeMemIR = (InterCode*)malloc(sizeof(InterCode));
                    writeMemIR->type = WriteMemory_IR;
                    writeMemIR->n_operand = 2;
                    writeMemIR->ops.o2.op1 = targetAddr;
                    writeMemIR->ops.o2.op2 = rightSideTemp;
                    insertInterCode(writeMemIR);

                }else{
                    printf("Cannot translate: Code contains variables or parameters of structure type.\n");
                    exit(0);
                }

                return;

            }else if(strcmp(exp->children[1]->name, "AND") == 0 ||      // Exp -> Exp AND Exp
                     strcmp(exp->children[1]->name, "OR") == 0 ||        // Exp -> Exp OR Exp
                     strcmp(exp->children[1]->name, "RELOP") == 0){          // Exp -> Exp RELOP Exp
                     
                 Operand* label1 = createLabel();
                 Operand* label2 = createLabel();

                 // code0
                 Operand* zero = createNumber(0);
                 //Operand* zero = (Operand*)malloc(sizeof(Operand));
                 //zero->value = "#0";

                 InterCode* code0 = (InterCode*)malloc(sizeof(InterCode));
                 code0->type = Assign_IR;
                 code0->n_operand = 2;
                 code0->ops.o2.op1 = placeOperand;
                 code0->ops.o2.op2 = zero;
                 insertInterCode(code0);

                 // code1
                 translate_Cond(exp, label1, label2);

                 // code2
                 insertLabelInterCode(label1);

                 Operand* one = createNumber(1);
                 //Operand* one = (Operand*)malloc(sizeof(Operand));
                 //one->value = "#1";
                 InterCode* code2_2 = (InterCode*)malloc(sizeof(InterCode));
                 code2_2->type = Assign_IR;
                 code2_2->n_operand = 2;
                 code2_2->ops.o2.op1 = placeOperand;
                 code2_2->ops.o2.op2 = one;
                 insertInterCode(code2_2);

                 insertLabelInterCode(label2);
                 return;

            }else if(strcmp(exp->children[1]->name, "PLUS")==0 ||       // Exp -> Exp PLUS Exp
                    strcmp(exp->children[1]->name, "MINUS")==0 ||       // Exp -> Exp MINUS Exp
                    strcmp(exp->children[1]->name, "STAR")==0 ||        // Exp -> Exp STAR Exp
                    strcmp(exp->children[1]->name, "DIV")==0){          // Exp -> Exp DIV Exp
                // optimize
                Node* firstExp = exp->children[0];
                Node* secondExp = exp->children[2];
                
                bool firstIsID = firstExp->n_children==1 && strcmp(firstExp->children[0]->name, "ID")==0;
                bool firstIsINT = firstExp->n_children==1 && strcmp(firstExp->children[0]->name, "INT")==0;
                bool secondIsID = secondExp->n_children==1 && strcmp(secondExp->children[0]->name, "ID")==0;
                bool secondIsINT = secondExp->n_children==1 && strcmp(secondExp->children[0]->name, "INT")==0;
                char* type = exp->children[1]->name;
                Operand *first, *second;
                if(firstIsID){
                    first = createVar(firstExp->children[0]->propertyValue);
                }else if(firstIsINT){
                    first = createNumber(atoi(firstExp->children[0]->propertyValue));
                }else{
                    first = createTemp();
                    translate_Exp(exp->children[0], first);
                }

                if(secondIsID){
                    second = createVar(secondExp->children[0]->propertyValue);
                }else if(secondIsINT){
                    second = createNumber(atoi(secondExp->children[0]->propertyValue));
                }else{
                    second = createTemp();
                    translate_Exp(exp->children[2], second);
                }
                insertArithmic(placeOperand, first, second, type);
                return;

            }else if(strcmp(exp->children[1]->name, "Exp") == 0){   // Exp -> LP Exp RP
                translate_Exp(exp->children[1], placeOperand);
                return;
            }else if(strcmp(exp->children[1]->name, "LP") == 0){    // Exp -> ID LP RP
                // call function
                if(strcmp(exp->children[0]->propertyValue, "read")==0){
                    InterCode* readIR = (InterCode*)malloc(sizeof(InterCode));
                    readIR->type = Read_IR;
                    readIR->n_operand = 1;
                    readIR->ops.o1.op1 = placeOperand;
                    insertInterCode(readIR);
                    return;
                }else{
                    char* id = exp->children[0]->propertyValue;
                    Operand* funcOperand = (Operand*)malloc(sizeof(Operand));
                    funcOperand->value = (char*)malloc(strlen(id)+1);
                    strcpy(funcOperand->value,id);
                    funcOperand->type = Function;

                    InterCode* callIR = (InterCode*)malloc(sizeof(InterCode));
                    callIR->n_operand = 1;
                    callIR->type = Call_IR;
                    callIR->ops.o1.op1 = funcOperand;
                    insertInterCode(callIR);
                }

            }else if(strcmp(exp->children[1]->name, "DOT") == 0){   // Exp -> Exp DOT ID
            }else{
                printf("Exp() case 3, error with unknown production\n");
                return NULL;
            }
            break;
        }
        case 4:{
            if(strcmp(exp->children[0]->name, "ID") == 0){  // Exp -> ID LP Args RP
                // call function
                char* id = exp->children[0]->propertyValue;
                clearArgList();
                // code1
                translate_Args(exp->children[2]);
                if(strcmp(id, "write") == 0){
                    ArgListNode* p = argListHead;
                    Operand* writeFuncParam = copyOperand(p->operand);
                    InterCode* writeIR = (InterCode*)malloc(sizeof(InterCode));
                    writeIR->n_operand = 1;
                    writeIR->type = Write_IR;
                    writeIR->ops.o1.op1 = writeFuncParam;
                    insertInterCode(writeIR);
                    return;
                }else{
                    ArgListNode* p = argListHead;
                    while(p){
                        Operand* funcParam = copyOperand(p->operand);
                        InterCode* argIR = (InterCode*)malloc(sizeof(InterCode));
                        argIR->n_operand = 1;
                        argIR->type = Arg_IR;
                        argIR->ops.o1.op1 = funcParam;
                        insertInterCode(argIR);

                        p = p->next;
                    }
                    Operand* funcOp = (Operand*)malloc(sizeof(Operand));
                    funcOp->value = (char*)malloc(strlen(id)+1);
                    strcpy(funcOp->value, id);
                    funcOp->type = Function;

                    InterCode* callIR = (InterCode*)malloc(sizeof(InterCode));
                    callIR->n_operand = 2;
                    callIR->type = Call_IR;
                    callIR->ops.o2.op1 = placeOperand;
                    callIR->ops.o2.op2 = funcOp;
                    insertInterCode(callIR);
                    return;
                }

            }else if(strcmp(exp->children[0]->name, "Exp") == 0){   // Exp -> Exp LB Exp RB
                Operand* targetAddr = createTemp();
                translate_ArrayAddr(exp, targetAddr);

                // read value from memory
                InterCode* readMemIR = (InterCode*)malloc(sizeof(InterCode));
                readMemIR->type = ReadMemory_IR;
                readMemIR->n_operand = 2;
                readMemIR->ops.o2.op1 = placeOperand;
                readMemIR->ops.o2.op2 = targetAddr;
                insertInterCode(readMemIR);

                return;
            }else{
                printf("Exp() case 4, error with unknown production\n");
                return;
            }
            break;
        }
        default:{
            printf("Exp(), error with unknown production\n");
            return;
        }
    }
}

void insertArithmic(Operand* place, Operand* o1, Operand* o2, char* type){
    InterCode* ir = (InterCode*)malloc(sizeof(InterCode));
    ir->n_operand = 3;
    ir->ops.o3.op1 = place;
    ir->ops.o3.op2 = o1;
    ir->ops.o3.op3 = o2;

    if(strcmp(type, "PLUS")==0){
        ir->type = Plus_IR;
    }else if(strcmp(type, "MINUS")==0){
        ir->type = Minus_IR;
    }else if(strcmp(type, "STAR")==0){
        ir->type = Multiply_IR;
    }else if(strcmp(type, "DIV")==0){
        ir->type = Divide_IR;
    }else{
        assert(0);
    }
    insertInterCode(ir);
}

// Set target address of array to place
void translate_ArrayAddr(Node* exp, Operand* place){    // Exp -> Exp LB Exp RB
    int n_count = 1;
    Node* subExp = exp->children[0];
    while(subExp->n_children == 4 && strcmp(subExp->children[0]->name, "Exp")==0){
        n_count++;
        subExp = subExp->children[0];
    }
    if(n_count == 1){       // 1-d array
        Operand* subScriptValue = createTemp();
        translate_Exp(exp->children[2], subScriptValue);
        
        Operand* four = createNumber(4);
        Operand* offsetTemp = createTemp();

        // calculate offset
        InterCode* offset = (InterCode*)malloc(sizeof(InterCode));
        offset->type = Multiply_IR;
        offset->n_operand = 3;
        offset->ops.o3.op1 = offsetTemp;
        offset->ops.o3.op2 = subScriptValue;
        offset->ops.o3.op3 = four;
        insertInterCode(offset);

        // get start address
        char* arrayName = getArrayExpName(exp);
        Operand* startAddr = createTemp();
        Operand* arrayOp = (Operand*)malloc(sizeof(Operand));
        arrayOp->value = (char*)malloc(strlen(arrayName)+1);
        strcpy(arrayOp->value, arrayName);
        arrayOp->type = Array;
        InterCode* startAddrIR = (InterCode*)malloc(sizeof(InterCode));

        SpecialParam* spParam = getSpecialParam(arrayName);
        if(spParam != NULL && spParam->ifAddr){
            startAddrIR->type = Assign_IR;
            startAddrIR->n_operand = 2;
            startAddrIR->ops.o2.op1 = startAddr;
            startAddrIR->ops.o2.op2 = arrayOp;
            insertInterCode(startAddrIR);
        }else{
            startAddrIR->type = Address_IR;
            startAddrIR->n_operand = 2;
            startAddrIR->ops.o2.op1 = startAddr;
            startAddrIR->ops.o2.op2 = arrayOp;
            insertInterCode(startAddrIR);
        }

        // calculate target addr

        InterCode* targetAddrIR = (InterCode*)malloc(sizeof(InterCode));
        targetAddrIR->type = Plus_IR;
        targetAddrIR->n_operand = 3;
        targetAddrIR->ops.o3.op1 = place;
        targetAddrIR->ops.o3.op2 = startAddr;
        targetAddrIR->ops.o3.op3 = offsetTemp;
        insertInterCode(targetAddrIR);

    }else if(n_count == 2){ // 2-d array
        // Exp -> Exp1 LB Exp2 RB
        // Exp1 -> Exp3 LB Exp4 RB
        Node* exp1 = exp->children[0];
        Node* exp2 = exp->children[2];
        Node* exp3 = exp1->children[0];
        Node* exp4 = exp1->children[2];

        // process subscript
        Operand* subSubScript = createTemp();
        Operand* subScript = createTemp();
        translate_Exp(exp2, subSubScript);
        translate_Exp(exp4, subScript);

        // calculate offset
        char* arrayName = getArrayExpName(exp);
        ArrayDesc* arrayInfo = getArrayDesc(arrayName);
        int dim2 = arrayInfo->dim2;

        Operand* dim2Op = createNumber(dim2);
        
        Operand* mulTemp1 = createTemp();
        InterCode* mul1 = (InterCode*)malloc(sizeof(InterCode));
        mul1->type = Multiply_IR;
        mul1->n_operand = 3;
        mul1->ops.o3.op1 = mulTemp1;
        mul1->ops.o3.op2 = subScript;
        mul1->ops.o3.op3 = dim2Op;
        insertInterCode(mul1);

        Operand* addTemp1 = createTemp();
        InterCode* add1 = (InterCode*)malloc(sizeof(InterCode));
        add1->type = Plus_IR;
        add1->n_operand = 3;
        add1->ops.o3.op1 = addTemp1;
        add1->ops.o3.op2 = mulTemp1;
        add1->ops.o3.op3 = subSubScript;
        insertInterCode(add1);

        Operand* four = createNumber(4);
        Operand* mulTemp2 = createTemp();
        InterCode* mul2 = (InterCode*)malloc(sizeof(InterCode));
        mul2->type = Multiply_IR;
        mul2->n_operand = 3;
        mul2->ops.o3.op1 = mulTemp2;
        mul2->ops.o3.op2 = addTemp1;
        mul2->ops.o3.op3 = four;
        insertInterCode(mul2);

        // get start address
        Operand* startAddr = createTemp();
        Operand* arrayOp = (Operand*)malloc(sizeof(Operand));
        arrayOp->value = (char*)malloc(strlen(arrayName)+1);
        strcpy(arrayOp->value, arrayName);
        arrayOp->type = Array;

        InterCode* startAddrIR = (InterCode*)malloc(sizeof(InterCode));
        SpecialParam* spParam = getSpecialParam(arrayName);
        if(spParam != NULL && spParam->ifAddr){
            startAddrIR->type = Assign_IR;
            startAddrIR->n_operand = 2;
            startAddrIR->ops.o2.op1 = startAddr;
            startAddrIR->ops.o2.op2 = arrayOp;
            insertInterCode(startAddrIR);
        }else{
            startAddrIR->type = Address_IR;
            startAddrIR->n_operand = 2;
            startAddrIR->ops.o2.op1 = startAddr;
            startAddrIR->ops.o2.op2 = arrayOp;
            insertInterCode(startAddrIR);
        }

        // calculate target addr
        InterCode* targetAddrIR = (InterCode*)malloc(sizeof(InterCode));
        targetAddrIR->type = Plus_IR;
        targetAddrIR->n_operand = 3;
        targetAddrIR->ops.o3.op1 = place;
        targetAddrIR->ops.o3.op2 = startAddr;
        targetAddrIR->ops.o3.op3 = mulTemp2;
        insertInterCode(targetAddrIR);
        return;
    }else{
        printf("Cannot translate, code contains array variable of more than 2 dimensions.\n");
        exit(0);
    }
    
}

void translate_Cond(Node* exp, Operand* label_true, Operand* label_false){

    if(exp->n_children == 2 && strcmp(exp->children[0]->name, "NOT")==0){   // Exp->NOT Exp
        translate_Cond(exp->children[1], label_false, label_true);
    }else if(exp->n_children == 3 && strcmp(exp->children[1]->name, "RELOP") == 0){   // Exp->Exp RELOP Exp
        // optimize
        Node* firstExp = exp->children[0];
        Node* secondExp = exp->children[2];
        bool firstIsID = firstExp->n_children==1 && strcmp(firstExp->children[0]->name,"ID")==0;
        bool firstIsINT = firstExp->n_children==1 && strcmp(firstExp->children[0]->name,"INT")==0;
        bool secondIsID = secondExp->n_children==1 && strcmp(secondExp->children[0]->name,"ID")==0;
        bool secondIsINT = secondExp->n_children==1 && strcmp(secondExp->children[0]->name, "INT")==0;
        
        Operand *first, *second;
        // code1
        if(firstIsID){
            first = createVar(firstExp->children[0]->propertyValue);
        }else if(firstIsINT){
            first = createNumber(atoi(firstExp->children[0]->propertyValue));
        }else{
            first = createTemp();
            translate_Exp(exp->children[0], first);
        }

        // code2
        if(secondIsID){
            second = createVar(secondExp->children[0]->propertyValue);
        }else if(secondIsINT){
            second = createNumber(atoi(secondExp->children[0]->propertyValue));
        }else{
            second = createTemp();
            translate_Exp(exp->children[2], second);
        }

        char* relop = exp->children[1]->propertyValue;
        // code3
        InterCode* ifGoto = (InterCode*)malloc(sizeof(InterCode));
        ifGoto->type = ConJump_IR;
        ifGoto->n_operand = 3;
        ifGoto->ops.conJump.op1 = first;
        ifGoto->ops.conJump.op2 = second;
        ifGoto->ops.conJump.relop = (char*)malloc(strlen(relop)+1);
        strcpy(ifGoto->ops.conJump.relop, relop);
        ifGoto->ops.conJump.op3 = label_true;
        insertInterCode(ifGoto);

        InterCode* finishGoto = (InterCode*)malloc(sizeof(InterCode));
        finishGoto->type = Goto_IR;
        finishGoto->n_operand = 1;
        finishGoto->ops.o1.op1 = label_false;
        insertInterCode(finishGoto);
        return;

    }else if(exp->n_children == 3 && strcmp(exp->children[1]->name, "AND") == 0){   // Exp->Exp AND Exp
        
        Operand* label1 = createLabel();

        //code1
        translate_Cond(exp->children[0], label1, label_false);

        insertLabelInterCode(label1);
        //code2
        translate_Cond(exp->children[2], label_true, label_false);
        return;

    }else if(exp->n_children == 3 && strcmp(exp->children[1]->name, "OR") == 0){    // Exp->Exp OR Exp
        
        Operand* label1 = createLabel();

        //code1
        translate_Cond(exp->children[0], label_true, label1);

        insertLabelInterCode(label1);
        //code2
        translate_Cond(exp->children[2], label_true, label_false);

    }else{      // Other cases
        Operand* temp1 = createTemp();

        //code1
        translate_Exp(exp, temp1);
        //code2
        Operand* zero = createNumber(0);

        InterCode* ifGoto = (InterCode*)malloc(sizeof(InterCode));
        ifGoto->type = ConJump_IR;
        ifGoto->n_operand = 3;
        ifGoto->ops.conJump.op1 = temp1;
        ifGoto->ops.conJump.relop = "!=";
        ifGoto->ops.conJump.op2 = zero;
        ifGoto->ops.conJump.op3 = label_true;
        insertInterCode(ifGoto);

        insertLabelInterCode(label_false);
        return;
    }
}

void translate_Stmt(Node* stmt){
    if(stmt->n_children == 1){  // Stmt -> CompSt
        translate_CompSt(stmt->children[0]);
        return; 
    }else if(stmt->n_children == 2){    // Stmt -> Exp SEMI
        Operand* temp = createTemp();
        translate_Exp(stmt->children[0], temp);
        return;
    }else if(stmt->n_children == 3){        // Stmt -> RETURN Exp SEMI
        
        Node* exp = stmt->children[1];
        Operand* temp;
        if(exp->n_children==1 && strcmp(exp->children[0]->name, "INT")==0){
            temp = createNumber(atoi(exp->children[0]->propertyValue));
        }else if(exp->n_children==1 && strcmp(exp->children[0]->name, "ID")==0){
            temp = createVar(exp->children[0]->propertyValue);
        }else{
            temp = createTemp();
            translate_Exp(stmt->children[1], temp);
        }
        InterCode* returnIR = (InterCode*)malloc(sizeof(InterCode));
        returnIR->n_operand = 1;
        returnIR->type = Return_IR;
        returnIR->ops.o1.op1 = temp;
        insertInterCode(returnIR);
        return;
    }else if(stmt->n_children == 5){
        if(strcmp(stmt->children[0]->name, "IF") == 0){ // Stmt -> IF LP Exp RP Stmt

            Operand* label1 = createLabel();
            Operand* label2 = createLabel();
            // code1
            translate_Cond(stmt->children[2], label1, label2);
            
            insertLabelInterCode(label1);
            // code2
            translate_Stmt(stmt->children[4]);

            insertLabelInterCode(label2);
            return;

        }else if(strcmp(stmt->children[0]->name, "WHILE") == 0){    // Stmt -> WHILE LP Exp RP Stmt
            Operand* label1 = createLabel();
            Operand* label2 = createLabel();
            Operand* label3 = createLabel();

            insertLabelInterCode(label1);
            // code1
            translate_Cond(stmt->children[2], label2, label3);

            insertLabelInterCode(label2);

            // code2
            translate_Stmt(stmt->children[4]);

            InterCode* ir_goto = (InterCode*)malloc(sizeof(InterCode));
            ir_goto->n_operand = 1;
            ir_goto->type = Goto_IR;
            ir_goto->ops.o1.op1 = label1;
            insertInterCode(ir_goto);

            insertLabelInterCode(label3);
            return;

        }else{
            printf("Unhandled stmt in translate_Stmt.\n");
            assert(0);
        }

    }else if(stmt->n_children == 7){        // Stmt -> IF LP Exp RP Stmt ELSE Stmt
        Operand* label1 = createLabel();
        Operand* label2 = createLabel();
        Operand* label3 = createLabel();

        // code1
        translate_Cond(stmt->children[2], label1, label2);
        insertLabelInterCode(label1);
        // code2
        Node* stmt1 = stmt->children[4];
        translate_Stmt(stmt1);

        if(strcmp(stmt1->children[0]->name,"RETURN") != 0){
            InterCode* ir_goto = (InterCode*)malloc(sizeof(InterCode));
            ir_goto->n_operand = 1;
            ir_goto->type = Goto_IR;
            ir_goto->ops.o1.op1 = label3;
            insertInterCode(ir_goto);
        }

        insertLabelInterCode(label2);

        // code3
        Node* stmt2 = stmt->children[6];
        translate_Stmt(stmt2);

        if(strcmp(stmt2->children[0]->name,"RETURN") != 0){
            insertLabelInterCode(label3);
        }
        return;
    }else{
        printf("Unhandled stmt in translate_Stmt.\n");
        assert(0);
    }
}

char* ifExpIsArrayName(Node* exp){
    if(exp->n_children == 1 && strcmp(exp->children[0]->name, "ID")==0){
        char* id = exp->children[0]->propertyValue;
        ArrayDesc* arrayDesc = getArrayDesc(id);
        if(arrayDesc != NULL) return id;
        else return NULL;
    }
    return NULL;
}

void translate_Args(Node* args){
    if(args->n_children == 1){  // Args -> Exp
        char* arrName = ifExpIsArrayName(args->children[0]);
        if(arrName != NULL){
            translate_ArrayArg(arrName);
        }else{
            Operand* t1 = createTemp();
            // code1
            translate_Exp(args->children[0], t1);
            insertIntoArgList(t1);
        }

    }else if(args->n_children == 3){    // Args -> Exp COMMA Args
        char* arrName = ifExpIsArrayName(args->children[0]);
        if(arrName != NULL){
            translate_ArrayArg(arrName);
            translate_Args(args->children[2]);
        }else{
            Operand* t1 = createTemp();
            //code1
            translate_Exp(args->children[0], t1);
            insertIntoArgList(t1);
            //code2
            translate_Args(args->children[2]);
        }
    }
}

void translate_ArrayArg(char* arrayName){
    // use array name as parameter

    Operand* arrAddr = (Operand*)malloc(sizeof(Operand));
    char* addrValue = (char*)malloc(20);
    sprintf(addrValue, "&%s", arrayName);
    arrAddr->value = (char*)malloc(strlen(addrValue)+1);
    strcpy(arrAddr->value, addrValue);
    arrAddr->type = Address;
    insertIntoArgList(arrAddr);
}


void translate_FunDec(Node* fundec){
    Operand* funcOp = (Operand*)malloc(sizeof(Operand));
    char* funcName = fundec->children[0]->propertyValue;
    funcOp->value = (char*)malloc(strlen(funcName)+1);
    strcpy(funcOp->value, funcName);
    funcOp->type = Function;

    InterCode* fundecIR = (InterCode*)malloc(sizeof(InterCode));
    fundecIR->type = Function_IR;
    fundecIR->n_operand = 1;
    fundecIR->ops.o1.op1 = funcOp;
    insertInterCode(fundecIR);
    if(fundec->n_children == 3){    // FunDec -> ID LP RP
        return;
    }else if(fundec->n_children == 4){  // FunDec -> ID LP VarList RP
        translate_VarList(fundec->children[2]);
    }else{
        assert(0);
    }

}

void translate_Dec(Node* dec, char* varName){
    if(dec->n_children == 1){   // Dec -> VarDec
        // char* vName = getFormatStr("v", varCount++);
        // insertNameMap(varName, vName);
        translate_VarDec(dec->children[0]);
        
    }else if(dec->n_children == 3){ // Dec -> VarDec ASSIGNOP Exp
        if(dec->children[2]->n_children==1 && strcmp(dec->children[2]->children[0]->name,"INT")==0){
            int intValue = atoi(dec->children[2]->children[0]->propertyValue);
            Operand* intOperand = createNumber(intValue);

            Operand* varOperand = (Operand*)malloc(sizeof(Operand));
            varOperand->value = (char*)malloc(strlen(varName)+1);
            strcpy(varOperand->value, varName);
            varOperand->type = Variable;


            InterCode* decIR = (InterCode*)malloc(sizeof(InterCode));
            decIR->type = Assign_IR;
            decIR->n_operand = 2;
            decIR->ops.o2.op1 = varOperand;
            decIR->ops.o2.op2 = intOperand;
            insertInterCode(decIR);
        }else{
        
            Operand* temp = createTemp();
            translate_Exp(dec->children[2], temp);

            //char* vName = getFormatStr("v", varCount++);
            //insertNameMap(varName, vName);
            Operand* varOperand = (Operand*)malloc(sizeof(Operand));
            varOperand->value = (char*)malloc(strlen(varName)+1);
            strcpy(varOperand->value, varName);
            varOperand->type = Variable;

            InterCode* decIR = (InterCode*)malloc(sizeof(InterCode));
            decIR->type = Assign_IR;
            decIR->n_operand = 2;
            decIR->ops.o2.op1 = varOperand;
            decIR->ops.o2.op2 = temp;
            insertInterCode(decIR);
        }
    }

}

Operand* createLabel(){
    Operand* label = (Operand*)malloc(sizeof(Operand));
    char* labelName = getFormatStr("label", labelCount++);
    label->value = (char*)malloc(strlen(labelName)+1);
    strcpy(label->value, labelName);
    label->type = Label;
    return label;
}

void insertLabelInterCode(Operand* label){
    InterCode* label_ir = (InterCode*)malloc(sizeof(InterCode));
    label_ir->n_operand = 1;
    label_ir->type = Label_IR;
    label_ir->ops.o1.op1 = label;
    insertInterCode(label_ir);
}

Operand* createTemp(){
    Operand* temp = (Operand*)malloc(sizeof(Operand));
    char* tempName = getFormatStr("t", tempCount++);
    temp->value = (char*)malloc(strlen(tempName)+1);
    strcpy(temp->value, tempName);
    temp->type = Temp;
    return temp;
}

Operand* createNumber(int n){
    char* str = (char*)malloc(10);
    sprintf(str, "#%d", n);
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->value = (char*)malloc(strlen(str)+1);
    strcpy(res->value, str);
    res->type = Constant;
    return res;
}

Operand* createVar(char* id){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->value = (char*)malloc(strlen(id)+1);
    strcpy(res->value, id);
    res->type = Variable;
    return res;
}


