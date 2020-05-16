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


// intercode var name count
int labelCount = 0;     // labelx
int tempCount = 0;      // tx
int varCount = 0;       // vx
int funcCount = 0;      // fx
int paramCount = 0;     // px

Operand* copyOperand(Operand* src){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->value = (char*)malloc(strlen(src->value)+1);
    strcpy(res->value, src->value);
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

void writeInterCode(char* fileName){
    FILE* fp;
    if(strcmp(fileName, "stdout") == 0){
        fp = stdout;
    }else {
        fp = fopen(fileName, "w");
    }
    assert(fp);
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

}
void translate_OptTag(Node* opttag){

}
void translate_VarDec(Node* vardec){

}
void translate_VarList(Node* varlist){

}

void translate_ParamDec(Node* paramdec){

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

void translate_Exp(Node* exp, Operand* placeOperand){
    switch(exp->n_children){
        case 1:{
            if(strcmp(exp->children[0]->name, "ID") == 0){  // Exp -> ID

                // intercode
                char* id = exp->children[0]->propertyValue;

                // id should be added into sym_table in variable declaration
                /*char* interCodeVarName = getInterCodeVarName(id);
                if(interCodeVarName == NULL){
                    char* varName = getFormatStr("v", varCount++);
                    interCodeVarName = (char*)malloc(strlen(varName)+1);
                    strcpy(interCodeVarName, varName);
                }*/
                
                Operand* varOperand = (Operand*)malloc(sizeof(Operand));
                varOperand->value = (char*)malloc(strlen(id)+1);
                //strcpy(varOperand->value, interCodeVarName);
                strcpy(varOperand->value, id);

                InterCode* expID = (InterCode*)malloc(sizeof(InterCode));
                expID->n_operand = 2;
                expID->type = Assign_IR;
                expID->ops.o2.op1 = placeOperand;
                expID->ops.o2.op2 = varOperand;
                insertInterCode(expID);
                return;
            }else if(strcmp(exp->children[0]->name, "INT") == 0){  // Exp -> INT
                
                // intercode 
                Operand* intOperand = (Operand*)malloc(sizeof(Operand));
                // Operand->type = Constant;
                int intValue = atoi(exp->children[0]->propertyValue);
                char* intName = getFormatStr("#", intValue);
                intOperand->value = (char*)malloc(strlen(intName)+1);
                strcpy(intOperand->value, intName);

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
                Operand* zero = (Operand*)malloc(sizeof(Operand));
                zero->value = "#0";

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
                 Operand* zero = (Operand*)malloc(sizeof(Operand));
                 zero->value = "#0";

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

                 Operand* one = (Operand*)malloc(sizeof(Operand));
                 one->value = "#1";
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
                // TODO: struct and array
                char* id = exp->children[0]->children[0]->propertyValue;
                //char* interCodeVarName = getInterCodeVarName(id);

                Operand* temp = createTemp();

                // code1
                translate_Exp(exp->children[2], temp);

                // code2
                Operand* varOperand = (Operand*)malloc(sizeof(Operand));
                varOperand->value = (char*)malloc(strlen(id)+1);
                strcpy(varOperand->value, id);

                InterCode* ic1 = (InterCode*)malloc(sizeof(InterCode));
                ic1->n_operand = 2;
                ic1->type = Assign_IR;
                ic1->ops.o2.op1 = varOperand;
                ic1->ops.o2.op2 = temp;
                insertInterCode(ic1);

                //InterCode* ic2 = (InterCode*)malloc(sizeof(InterCode));
                //ic2->n_operand = 2;
                //ic2->type = Assign_IR;
                //ic2->ops.o2.op1 = placeOperand;
                //ic2->ops.o2.op2 = varOperand;
                //insertInterCode(ic2);
                return;

            }else if(strcmp(exp->children[1]->name, "AND") == 0 ||      // Exp -> Exp AND Exp
                     strcmp(exp->children[1]->name, "OR") == 0 ||        // Exp -> Exp OR Exp
                     strcmp(exp->children[1]->name, "RELOP") == 0){          // Exp -> Exp RELOP Exp
                     
                 Operand* label1 = createLabel();
                 Operand* label2 = createLabel();

                 // code0
                 Operand* zero = (Operand*)malloc(sizeof(Operand));
                 zero->value = "#0";

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

                 Operand* one = (Operand*)malloc(sizeof(Operand));
                 one->value = "#1";
                 InterCode* code2_2 = (InterCode*)malloc(sizeof(InterCode));
                 code2_2->type = Assign_IR;
                 code2_2->n_operand = 2;
                 code2_2->ops.o2.op1 = placeOperand;
                 code2_2->ops.o2.op2 = one;
                 insertInterCode(code2_2);

                 insertLabelInterCode(label2);
                 return;

            }else if(strcmp(exp->children[1]->name, "PLUS") == 0){  // Exp -> Exp PLUS Exp
                Operand* t1 = createTemp();
                Operand* t2 = createTemp();

                // code1
                translate_Exp(exp->children[0], t1);
                // code2
                translate_Exp(exp->children[2], t2);

                InterCode* plusIR = (InterCode*)malloc(sizeof(InterCode));
                plusIR->n_operand = 3;
                plusIR->type = Plus_IR;
                plusIR->ops.o3.op1 = placeOperand;
                plusIR->ops.o3.op2 = t1;
                plusIR->ops.o3.op3 = t2;
                insertInterCode(plusIR);
                return;

            }else if(strcmp(exp->children[1]->name, "MINUS") == 0){ // Exp -> Exp MINUS Exp
                Operand* t1 = createTemp();
                Operand* t2 = createTemp();

                // code1
                translate_Exp(exp->children[0], t1);
                // code2
                translate_Exp(exp->children[2], t2);

                InterCode* minusIR = (InterCode*)malloc(sizeof(InterCode));
                minusIR->n_operand = 3;
                minusIR->type = Minus_IR;
                minusIR->ops.o3.op1 = placeOperand;
                minusIR->ops.o3.op2 = t1;
                minusIR->ops.o3.op3 = t2;
                insertInterCode(minusIR);
                return;

            }else if(strcmp(exp->children[1]->name, "STAR") == 0){  // Exp -> Exp STAR Exp
                Operand* t1 = createTemp();
                Operand* t2 = createTemp();

                // code1
                translate_Exp(exp->children[0], t1);
                // code2
                translate_Exp(exp->children[2], t2);

                InterCode* multiplyIR = (InterCode*)malloc(sizeof(InterCode));
                multiplyIR->n_operand = 3;
                multiplyIR->type = Multiply_IR;
                multiplyIR->ops.o3.op1 = placeOperand;
                multiplyIR->ops.o3.op2 = t1;
                multiplyIR->ops.o3.op3 = t2;
                insertInterCode(multiplyIR);
                return;

            }else if(strcmp(exp->children[1]->name, "DIV") == 0){   // Exp -> Exp DIV Exp
                Operand* t1 = createTemp();
                Operand* t2 = createTemp();

                // code1
                translate_Exp(exp->children[0], t1);
                // code2
                translate_Exp(exp->children[2], t2);

                InterCode* divideIR = (InterCode*)malloc(sizeof(InterCode));
                divideIR->n_operand = 3;
                divideIR->type = Divide_IR;
                divideIR->ops.o3.op1 = placeOperand;
                divideIR->ops.o3.op2 = t1;
                divideIR->ops.o3.op3 = t2;
                insertInterCode(divideIR);
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

                    InterCode* callIR = (InterCode*)malloc(sizeof(InterCode));
                    callIR->n_operand = 2;
                    callIR->type = Call_IR;
                    callIR->ops.o2.op1 = placeOperand;
                    callIR->ops.o2.op2 = funcOp;
                    insertInterCode(callIR);
                    return;
                }

            }else if(strcmp(exp->children[0]->name, "Exp") == 0){   // Exp -> Exp LB Exp RB
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

void translate_Cond(Node* exp, Operand* label_true, Operand* label_false){

    if(exp->n_children == 2 && strcmp(exp->children[0]->name, "NOT")==0){   // Exp->NOT Exp
        translate_Cond(exp->children[1], label_false, label_true);
    }else if(exp->n_children == 3 && strcmp(exp->children[1]->name, "RELOP") == 0){   // Exp->Exp RELOP Exp
        
        Operand* temp1 = createTemp();
        Operand* temp2 = createTemp();

        // code1
        translate_Exp(exp->children[0], temp1);
        // code2
        translate_Exp(exp->children[2], temp2);

        char* relop = exp->children[1]->propertyValue;
        // code3
        InterCode* ifGoto = (InterCode*)malloc(sizeof(InterCode));
        ifGoto->type = ConJump_IR;
        ifGoto->n_operand = 3;
        ifGoto->ops.conJump.op1 = temp1;
        ifGoto->ops.conJump.op2 = temp2;
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
        Operand* zero = (Operand*)malloc(sizeof(Operand));
        zero->value = "#0";

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
        
        Operand* temp = createTemp();
        translate_Exp(stmt->children[1], temp);

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
            translate_Cond(stmt->children[2], label1, label2);

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
        translate_Stmt(stmt->children[4]);

        InterCode* ir_goto = (InterCode*)malloc(sizeof(InterCode));
        ir_goto->n_operand = 1;
        ir_goto->type = Goto_IR;
        ir_goto->ops.o1.op1 = label3;
        insertInterCode(ir_goto);

        insertLabelInterCode(label2);

        // code3
        translate_Stmt(stmt->children[6]);

        insertLabelInterCode(label3);
        return;
    }else{
        printf("Unhandled stmt in translate_Stmt.\n");
        assert(0);
    }
}

void translate_Args(Node* args){
    if(args->n_children == 1){  // Args -> Exp
        Operand* t1 = createTemp();
        // code1
        translate_Exp(args->children[0], t1);
        insertIntoArgList(t1);
    }else if(args->n_children == 3){    // Args -> Exp COMMA Args
        Operand* t1 = createTemp();
        //code1
        translate_Exp(args->children[0], t1);
        insertIntoArgList(t1);
        //code2
        translate_Args(args->children[2]);
    }
}

void translate_FunDec(Node* fundec){
    if(fundec->n_children == 3){    // FunDec -> ID LP RP
        Operand* funcOp = (Operand*)malloc(sizeof(Operand));
        char* funcName = fundec->children[0]->propertyValue;
        funcOp->value = (char*)malloc(strlen(funcName)+1);
        strcpy(funcOp->value, funcName);

        InterCode* fundecIR = (InterCode*)malloc(sizeof(InterCode));
        fundecIR->type = Function_IR;
        fundecIR->n_operand = 1;
        fundecIR->ops.o1.op1 = funcOp;
        insertInterCode(fundecIR);

    }else if(fundec->n_children == 4){  // FunDec -> ID LP VarList RP
        // TODO
    }else{
        assert(0);
    }

}

void translate_FunDecParam(char* paramName){
    Operand* paramOperand = (Operand*)malloc(sizeof(Operand));
    char* pName = getFormatStr("p", paramCount++);
    paramOperand->value = (char*)malloc(strlen(pName)+1);
    strcpy(paramOperand->value, pName);

    InterCode* paramIR = (InterCode*)malloc(sizeof(InterCode));
    paramIR->n_operand = 1;
    paramIR->type = Param_IR;
    paramIR->ops.o1.op1 = paramOperand;
    insertInterCode(paramIR);
}

void translate_Dec(Node* dec, char* varName){
    if(dec->n_children == 1){   // Dec -> VarDec
        char* vName = getFormatStr("v", varCount++);
        insertNameMap(varName, vName);
        
    }else if(dec->n_children == 3){ // Dec -> VarDec ASSIGNOP Exp
        Operand* temp = createTemp();
        translate_Exp(dec->children[2], temp);

        char* vName = getFormatStr("v", varCount++);
        insertNameMap(varName, vName);
        Operand* varOperand = (Operand*)malloc(sizeof(Operand));
        varOperand->value = (char*)malloc(strlen(vName)+1);
        strcpy(varOperand->value, vName);

        InterCode* decIR = (InterCode*)malloc(sizeof(InterCode));
        decIR->type = Assign_IR;
        decIR->n_operand = 2;
        decIR->ops.o2.op1 = temp;
        decIR->ops.o2.op2 = varOperand;
        insertInterCode(decIR);
    }

}

Operand* createLabel(){
    Operand* label = (Operand*)malloc(sizeof(Operand));
    char* labelName = getFormatStr("label", labelCount++);
    label->value = (char*)malloc(strlen(labelName)+1);
    strcpy(label->value, labelName);
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
    return temp;
}
