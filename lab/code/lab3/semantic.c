#include "semantic.h"
// debug flag
bool debug_sema = false;
// hash table
// both variable name and type name will be hashed into this table, since each two of them can not be identical
bool hash_table[HASH_TABLE_SIZE];
// the head node of list of current defined variables
SYMBOL_NODE* symbol_list_head = NULL;
// the head node of list of current defined types
TYPE_NODE* type_list_head = NULL;

// global hash node
HASH_NODE* root_hash_node = NULL;

HASH_NODE* createHashNode(HASH_NODE* parent){
    HASH_NODE* p = (HASH_NODE*)malloc(sizeof(HASH_NODE));
    for(int i = 0; i < HASH_TABLE_SIZE; ++i) p->hash_table[i] = false;
    p->parent = parent;
    p->n_children = 0;
    p->children = NULL;
    return p;
}

// str: string to search
// node: start node of searching
// return value: found in which node, or not found (NULL)
HASH_NODE* searchHashTableInNode(char* str, HASH_NODE* node){
    unsigned int key = BKDRHash(str) % HASH_TABLE_SIZE;
    HASH_NODE* tmpNode = node;
    while(tmpNode){
        if(node->hash_table[key]){
            return node;
        }else tmpNode = tmpNode->parent;
    }
    return NULL;
}

// str: string to insert
// node: node to insert (2-d pointer)
void insertIntoHashTableInNode(char* str, HASH_NODE** node){
    unsigned int key = BKDRHash(str) % HASH_TABLE_SIZE;
    (*node)->hash_table[key] = true;
}

unsigned int BKDRHash(char* str){
    unsigned int seed = 131;
    unsigned int hash = 0;

    while(*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

void initHashTable(){
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        hash_table[i] = false;
    }
}

void insertIntoHashTable(char* str){
    unsigned int key = (unsigned int)(BKDRHash(str) % HASH_TABLE_SIZE);
    unsigned int tmp = key;
    while(hash_table[key]){
        key = (key + 1) % HASH_TABLE_SIZE;
        if(key == tmp){
            printf("Insert into hashtable failed, \"%s\"\n", str);
            return;
        }
    }
    hash_table[key] = true;
    if(debug_sema) printf("Insert \"%s\" into [%d]\n", str, key);
}

// parameter: keyword
// has some probility to get false result
bool searchHashTable(char* str){
    unsigned int key = (unsigned int)(BKDRHash(str) % HASH_TABLE_SIZE);
    if(debug_sema && hash_table[key]){
        if(hash_table[key]) printf("Symbol %s exists at [%d]\n", str, key);
        else printf("Symbol %s does not exist\n", str);
    }
    return hash_table[key];
}

// Insert a new symbol into symbol list
void insertSymbol(VAR_INFO* var_info){
    SYMBOL_NODE* p = (SYMBOL_NODE*)malloc(sizeof(SYMBOL_NODE));
    p->info = copyVarInfo(var_info);
    p->next = NULL;
    
    if(!symbol_list_head){
        symbol_list_head = p;
    }else{
        SYMBOL_NODE* q = symbol_list_head;
        while(q->next) q = q->next;
        q->next = p;
    }      
    if(debug_sema) showSymbolList();
}    
// search for varialbe info in symbol list according to variable name
VAR_INFO* getSymbolInfo(char* varName){
    if(!symbol_list_head){
        if(debug_sema) printf("Symbol %s does not exist!\n", varName);
        return NULL;
    }
    SYMBOL_NODE* p = symbol_list_head;
    while(p){
        if(strcmp(p->info->varName, varName) == 0) return p->info;
        p = p->next;
    }
    return NULL;
}

TYPE_INFO* getTypeInfo(char* typeName){
    if(!type_list_head){
        if(debug_sema) printf("Type %s does not exist!\n", typeName);
        return NULL;
    }
    TYPE_NODE* p = type_list_head;
    while(p){
        if(strcmp(p->info->typeName, typeName) == 0) return p->info;
        p = p->next;
    }
    return NULL;
}

VAR_INFO* copyVarInfo(VAR_INFO* src){
    if(!src){
        printf("Error in copyVarInfo, src is NULL\n");
        return NULL;;
    }
    VAR_INFO* p = (VAR_INFO*)malloc(sizeof(VAR_INFO));
    p->varType = (char*)malloc(strlen(src->varType)+1);
    p->varName = (char*)malloc(strlen(src->varName)+1);
    strcpy(p->varType, src->varType);
    strcpy(p->varName, src->varName);
    p->ifArray = src->ifArray;
    return p;
}
// Semantic analysis
void Program(Node* program){
    root_hash_node = createHashNode(NULL);
    if(debug_sema) printf("Program()\n");
    if(!program) {
        if(debug_sema) printf("Program(), program node NULL\n");
        return;
    }
    ExtDefList(program->children[0]);       // Program -> ExtDefList
}
void ExtDefList(Node* extdeflist){
    if(debug_sema) printf("ExtDefList()\n");
    if(!extdeflist){
        if(debug_sema) printf("ExtDefList(), extdeflist node NULL\n");
        return;
    }
    if(extdeflist->n_children == 2){    // ExtDefList -> ExtDef ExtDefList
        ExtDef(extdeflist->children[0]);
        //if(debug_sema) showSymbolList();
        //if(debug_sema) showTypeList();
        ExtDefList(extdeflist->children[1]);
    }else if(extdeflist->n_children == 0){  // ExtDefList -> empty
        return;
    }else{
        printf("ExtDefList(), error with unknown production\n");
        return;
    }
}
void ExtDef(Node* extdef){
    if(debug_sema) printf("ExtDef()\n");
    if(!extdef){
        if(debug_sema) printf("ExtDef(), extdef node NULL\n");
        return;
    }
    if(strcmp(extdef->children[1]->name, "ExtDecList") == 0){   // ExtDef -> Specifier ExtDecList SEMI
        char* typeName = Specifier(extdef->children[0]);
        ExtDecList(extdef->children[1], typeName);
    }else if(strcmp(extdef->children[1]->name, "SEMI") == 0){   // ExtDef -> Specifier SEMI
        Specifier(extdef->children[0]);
    }else if(strcmp(extdef->children[1]->name, "FunDec") == 0){ // ExtDef -> Specifier FunDec CompSt
        char* returnTypeName = Specifier(extdef->children[0]);
        FunDec(extdef->children[1], returnTypeName);
        CompSt(extdef->children[2], returnTypeName);

    }else{
        printf("ExtDef(), error with unknown production\n");
        return;
    }
}

void insertType(TYPE_INFO* typeInfo, const char* typeCategory){
    TYPE_NODE* p = (TYPE_NODE*)malloc(sizeof(TYPE_NODE));
    p->info = copyTypeInfo(typeInfo, typeCategory);
    p->next = NULL;
    if(!type_list_head){
        type_list_head = p;
    }else{
        TYPE_NODE* q = type_list_head;
        while(q->next) q = q->next;
        q->next = p;
    }
    if(debug_sema) showTypeList();
}
FUNC_INFO* copyFuncInfo(FUNC_INFO* src){
    FUNC_INFO* p = (FUNC_INFO*)malloc(sizeof(FUNC_INFO));
    p->n_params = src->n_params;
    p->returnTypeName = (char*)malloc(strlen(src->returnTypeName)+1);
    strcpy(p->returnTypeName, src->returnTypeName);
    p->params = (VAR_INFO**)malloc(sizeof(VAR_INFO*) * src->n_params);
    for(int i = 0; i < src->n_params; ++i){
        p->params[i] = (VAR_INFO*)malloc(sizeof(VAR_INFO));
        p->params[i]->varName = (char*)malloc(strlen(src->params[i]->varName)+1);
        strcpy(p->params[i]->varName, src->params[i]->varName);
        p->params[i]->varType = (char*)malloc(strlen(src->params[i]->varType)+1);
        strcpy(p->params[i]->varType, src->params[i]->varType);
        p->params[i]->ifArray = src->params[i]->ifArray;
    }
    return p;
}
STRUCT_INFO* copyStructInfo(STRUCT_INFO* src){
    STRUCT_INFO* p = (STRUCT_INFO*)malloc(sizeof(STRUCT_INFO));
    p->n_fields = src->n_fields;
    p->fields = (VAR_INFO**)malloc(sizeof(VAR_INFO*) * src->n_fields);
    for(int i = 0; i < src->n_fields; ++i){
        p->fields[i] = (VAR_INFO*)malloc(sizeof(VAR_INFO));
        p->fields[i]->varName = (char*)malloc(strlen(src->fields[i]->varName)+1);
        strcpy(p->fields[i]->varName, src->fields[i]->varName);
        p->fields[i]->varType = (char*)malloc(strlen(src->fields[i]->varType)+1);
        strcpy(p->fields[i]->varType, src->fields[i]->varType);
        p->fields[i]->ifArray = src->fields[i]->ifArray;
    }
    return p;
}
ARRAY_INFO* copyArrayInfo(ARRAY_INFO* src){
    ARRAY_INFO* p = (ARRAY_INFO*)malloc(sizeof(ARRAY_INFO));
    p->size = src->size;
    p->eleTypeName = (char*)malloc(strlen(src->eleTypeName)+1);
    strcpy(p->eleTypeName, src->eleTypeName);
    return p;
}
// typeCategory: "array", "function" or "struct"
TYPE_INFO* copyTypeInfo(TYPE_INFO* src, const char* typeCategory){
    TYPE_INFO* p = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
    p->typeCategory = (char*)malloc(strlen(typeCategory)+1);
    strcpy(p->typeCategory, typeCategory);
    p->typeName = (char*)malloc(strlen(src->typeName)+1);
    strcpy(p->typeName, src->typeName);
    if(strcmp(typeCategory, "int") == 0 || strcmp(typeCategory, "float") == 0){
        printf("Illegal type category \"%s\"\n", typeCategory);
        return NULL;
    }else{
        p->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
        if(strcmp(typeCategory, "function") == 0){
            p->typeDetail->func_info = copyFuncInfo(src->typeDetail->func_info);
        }else if(strcmp(typeCategory, "array") == 0){
            p->typeDetail->array_info = copyArrayInfo(src->typeDetail->array_info);
        }else if(strcmp(typeCategory, "struct") == 0){
            p->typeDetail->struct_info = copyStructInfo(src->typeDetail->struct_info);
        }else{
            printf("Error, unknown type category \"%s\"\n", typeCategory);
            return NULL;
        }
    }
    return p;
}

void ExtDecList(Node* extdeclist, char* typeName){
    if(debug_sema) printf("ExtDecList()\n");
    if(!extdeclist){
        if(debug_sema) printf("ExtDecList(), extdeclist node NULL\n");
        return;
    }
    if(extdeclist->n_children == 1){        // ExtDecList -> VarDec
        VarDec(extdeclist->children[0], typeName);
    }else if(extdeclist->n_children == 3){  // ExtDecList -> VarDec COMMA ExtDecList
        VarDec(extdeclist->children[0], typeName);
        ExtDecList(extdeclist->children[2], typeName);
    }else{
        printf("ExtDecList(), error with unknown production\n");
        return;
    }
}

char* Specifier(Node* specifier){
    if(debug_sema) printf("Specifier()\n");
    if(!specifier){
        if(debug_sema) printf("Specifier(), specifier node NULL\n");
        return "";
    }

    if(strcmp(specifier->children[0]->name, "TYPE") == 0){  // Specifier -> TYPE
        Node* node_type = specifier->children[0];
        return node_type->propertyValue;        // "int" or "float"
    }else if(strcmp(specifier->children[0]->name, "StructSpecifier") == 0){ // Specifier -> StructSpecifier
        // return struct name as type id
        char* structName = StructSpecifier(specifier->children[0]);
        // for structures, the type insert work should be done here

        return structName;
    }else{
        printf("Specifier(), error with unknown production\n");
        return "";
    }
}
char* StructSpecifier(Node* structspecifier){
    if(debug_sema) printf("StructSpecifier()\n");
    if(!structspecifier){
        if(debug_sema) printf("StructSpecifier node NULL\n");
        return "";
    }

    if(structspecifier->n_children == 5){   // StructSpecifier -> STRUCT OptTag LC DefList RC
        char* structName = OptTag(structspecifier->children[1]);
        // DefList(structspecifier->children[3]);
        
        Node* deflist = structspecifier->children[3];
        // count number of fields
        Node* tmpDeflist = deflist;
        int n_def = 0, n_fields = 0;
        while(tmpDeflist){     // DefList -> Def DefList
            n_def++;
            Node* def = tmpDeflist->children[0];
            int n_fieldsDeclist = 1;
            Node* declist = def->children[1];   // Def -> Specifier DecList SEMI
            Node* tmpDeclist = declist;
            while(tmpDeclist->n_children == 3){
                n_fieldsDeclist++;
                tmpDeclist = tmpDeclist->children[2];
            }
            n_fields += n_fieldsDeclist;
            tmpDeflist = tmpDeflist -> children[1];
        }
        if(debug_sema) printf("number of fields: %d\n", n_fields);

        // set type info of struct
        TYPE_INFO* structTypeInfo = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
        structTypeInfo->typeCategory = "struct";
        structTypeInfo->typeName = (char*)malloc(strlen(structName)+1);
        strcpy(structTypeInfo->typeName, structName);

        structTypeInfo->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
        // create struct info
        STRUCT_INFO* structInfo = (STRUCT_INFO*)malloc(sizeof(STRUCT_INFO));
        structInfo->n_fields = n_fields;
        structInfo->fields = (VAR_INFO**)malloc(sizeof(VAR_INFO*) * n_fields);
        for(int i = 0; i < n_fields; ++i){
            structInfo->fields[i] = (VAR_INFO*)malloc(sizeof(VAR_INFO));
        }
        // set fields info
        Node* tmpDefList2 = deflist;
        int i = 0;
        while(tmpDefList2){         // DefList -> Def DefList
            Node* def = tmpDefList2->children[0];
            Node* declist = def->children[1];       // Def -> Specifier DecList SEMI
            char* currentType = Specifier(def->children[0]);
            Node* tmpDeclist = declist;
            while(tmpDeclist->n_children > 0){
                Node* dec = tmpDeclist->children[0];
                if(dec->n_children == 3){       // Dec -> VarDec ASSIGNOP Exp
                    int line = dec->first_line;
                    printf("Error type 15 at Line %d: Initialize field when define a structure.\n", line);
                    return "";
                }
                Node* vardec = dec->children[0];    // Dec -> VarDec
                char* varName = VarDec(vardec, currentType);
                if(strcmp(varName, "") == 0) return "";     // error occurs in vardec
                VAR_INFO* fieldVarInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                fieldVarInfo->varName = (char*)malloc(strlen(varName)+1);
                strcpy(fieldVarInfo->varName, varName);
                if(vardec->n_children == 1){    // VarDec -> ID
                    fieldVarInfo->ifArray = false;
                }else if(vardec->n_children == 4){  // VarDec -> VarDec LB INT RB
                    fieldVarInfo->ifArray = true;
                }
                fieldVarInfo->varType = (char*)malloc(strlen(currentType)+1);
                strcpy(fieldVarInfo->varType, currentType);

                structInfo->fields[i] = copyVarInfo(fieldVarInfo);
                
                ++i;
                if(tmpDeclist->n_children == 3){        // DecList -> Dec COMMA DecList
                    tmpDeclist = tmpDeclist->children[2];
                }else if(tmpDeclist->n_children == 1){  // DecList -> Dec
                    break;
                }else{
                    printf("Error, unknown production for DecList. StructSpecifier().\n");
                    return "";
                }   
            }
            if(tmpDefList2->n_children == 2){
                tmpDefList2 = tmpDefList2 -> children[1];
            }else if(tmpDefList2->n_children == 0){
                break;
            }else{
                printf("Error, unknown production for DefList. StructSpecifier().\n");
                return "";
            }
        }
        if(debug_sema){
            printf("struct %s fields:\n", structName);
            int n_fields = structInfo->n_fields;
            for(int i = 0; i < structInfo->n_fields; ++i){
                VAR_INFO* v = structInfo->fields[i];
                printf("(varName: %s, varType: %s, ifArray: %d)\n", v->varName, v->varType, v->ifArray);
            }
        }
        structTypeInfo->typeDetail->struct_info = copyStructInfo(structInfo);
        insertType(structTypeInfo, "struct");
        // if(debug_sema) showTypeList();
        return structName;

    }else if(structspecifier->n_children == 2){ // StructSpecifier -> STRUCT Tag
        Node* idNode = structspecifier->children[1]->children[0];       // Tag -> ID
        return idNode->propertyValue;
    }else{
        printf("StructSpecifier(), error with unknown production\n");
        return "";
    }
}
char* OptTag(Node* opttag){
    if(debug_sema) printf("OptTag()\n");
    if(!opttag){
        if(debug_sema) printf("OptTag node NULL\n");
        return "";
    }
    if(opttag->n_children == 1){            // OptTag -> ID
        // used when define a structure
        char* id = opttag->children[0]->propertyValue;
        bool searchRes = searchHashTable(id);
        if(searchRes){  // ID redefined
            int line = opttag->first_line;
            printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", line, id);
            return "";
        }
        insertIntoHashTable(id);
        if(debug_sema) printf("OptTag(), create ID \"%s\" succeeds\n", id);
        return id;
    }else if(opttag->n_children == 0){      // OptTag -> empty
        return "";
    }else{
        printf("OptTag(), error with unknown production\n");
        return "";
    }
}
// return variable name(ID), can be dropped if not need to use
// return value used in FunDec and StructSpecifier
char* VarDec(Node* vardec, char* typeName){
    if(debug_sema) printf("VarDec()\n");
    if(!vardec){
        if(debug_sema) printf("VarDec node NULL\n");
        return "";
    }
    if(vardec->n_children == 1){        // VarDec -> ID
        char* id = vardec->children[0]->propertyValue;
        // Undefined complex type
        if(strcmp(typeName, "int") != 0 && strcmp(typeName, "float") != 0 && !searchHashTable(typeName)){
            int line = vardec->children[0]->first_line;
            printf("Error type 17 at Line %d: Undefined type \"%s\".\n", line, typeName);
            return "";
        }
        // ID redefined
        if(searchHashTable(id)){
            int line = vardec->children[0]->first_line;
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, id);
            return "";
        }
        insertIntoHashTable(id);
        // create and insert symbol
        VAR_INFO* p = (VAR_INFO*)malloc(sizeof(VAR_INFO));
        p->varType = (char*)malloc(strlen(typeName)+1);
        strcpy(p->varType, typeName);
        p->varName = (char*)malloc(strlen(id)+1);
        strcpy(p->varName, id);
        p->ifArray = false;
        insertSymbol(p);

        // insert type
        // basic types don't need to insert type node
        // function type is handled in FunDec
        // struct type is handled in StructSpecifier

        return id;

    }else if(vardec->n_children == 4){  // VarDec -> VarDec LB INT RB (array)
        int nDimension = 0;
        Node* tmp = vardec;
        while(tmp->n_children > 1){
            ++nDimension;
            tmp = tmp->children[0];
        }
        if(nDimension == 1){        // 1-dimensional array
            // get variable id
            char* id = vardec->children[0]->children[0]->propertyValue;
            if(searchHashTable(id)){
                printf("Error in 1-dimensional array, \"%s\" redefined\n", id);
                return "";
            }
            insertIntoHashTable(id);
            // insert symbol into symbol list
            VAR_INFO* p = (VAR_INFO*)malloc(sizeof(VAR_INFO));
            // p->varType = "array";
            p->varType = (char*)malloc(strlen(typeName)+1);
            strcpy(p->varType, typeName);
            p->varName = (char*)malloc(strlen(id)+1);
            strcpy(p->varName, id);
            p->ifArray = true;
            insertSymbol(p);

            // array info
            int arraySize = atoi(vardec->children[2]->propertyValue);
            ARRAY_INFO* arrInfo = (ARRAY_INFO*)malloc(sizeof(ARRAY_INFO));
            arrInfo->size = arraySize;
            arrInfo->eleTypeName = (char*)malloc(strlen(typeName)+1);
            strcpy(arrInfo->eleTypeName, typeName);

            // type info
            TYPE_INFO* q = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
            q->typeCategory = "array";
            q->typeName = (char*)malloc(strlen(id)+1);  // use id as unique type name of complex type
            strcpy(q->typeName, id);
            q->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
            q->typeDetail->array_info = copyArrayInfo(arrInfo);
            // insert type info into type info list
            insertType(q, "array");

            return id;
        }else if(nDimension == 2){  // 2-dimensional array
            // VarDec -> VarDec1 LB INT RB
            // VarDec1 -> VarDec2 LB INT RB
            // VarDec2 -> ID
            Node* vardec1 = vardec->children[0];
            Node* vardec2 = vardec1->children[0];
            char* id = vardec2->children[0]->propertyValue;
            int dim1 = atoi(vardec1->children[2]->propertyValue);
            int dim2 = atoi(vardec->children[2]->propertyValue);

            // insert into symbol list
            if(searchHashTable(id)){
                printf("Error in 2-dimensional array, \"%s\" redefined\n", id);
                return "";
            }
            insertIntoHashTable(id);
            VAR_INFO* array2DVarInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
            array2DVarInfo->varName = (char*)malloc(strlen(id)+1);
            strcpy(array2DVarInfo->varName, id);
            // array2DVarInfo->varType = "array";
            array2DVarInfo->varType = (char*)malloc(strlen(typeName)+1);
            strcpy(array2DVarInfo->varType, typeName);
            array2DVarInfo->ifArray = true;
            insertSymbol(array2DVarInfo);
            
            // typeName of internal array: "typeName" + "_array", eleType: "typeName"
            // typeName of external array: "id", eleType: "typeName" + "_array"

            // "typeName" + "_array"
            char* arrayConst = "_array";
            char* arrayTypeName = (char*)malloc(strlen(typeName)+strlen(arrayConst)+1);
            strcat(arrayTypeName, typeName);
            strcat(arrayTypeName, arrayConst);

            // internal array info
            ARRAY_INFO* arrInfo2 = (ARRAY_INFO*)malloc(sizeof(ARRAY_INFO));
            arrInfo2->size = dim2;
            arrInfo2->eleTypeName = (char*)malloc(strlen(typeName)+1);
            strcpy(arrInfo2->eleTypeName, typeName);

            // external array info
            ARRAY_INFO* arrInfo1 = (ARRAY_INFO*)malloc(sizeof(ARRAY_INFO));
            arrInfo1->size = dim1;
            arrInfo1->eleTypeName = (char*)malloc(strlen(arrayTypeName)+1);
            strcpy(arrInfo1->eleTypeName, arrayTypeName);

            // internal type info
            TYPE_INFO* typeInfo2 = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
            typeInfo2->typeCategory = "array";
            typeInfo2->typeName = (char*)malloc(strlen(arrayTypeName)+1);
            strcpy(typeInfo2->typeName, arrayTypeName);
            typeInfo2->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
            typeInfo2->typeDetail->array_info = copyArrayInfo(arrInfo2);

            // external type info
            TYPE_INFO* typeInfo1 = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
            typeInfo1->typeCategory = "array"; 
            typeInfo1->typeName = (char*)malloc(strlen(id)+1);
            strcpy(typeInfo1->typeName, id);
            typeInfo1->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
            typeInfo1->typeDetail->array_info = copyArrayInfo(arrInfo1);

            // insert types into type list
            insertType(typeInfo1, "array");
            insertType(typeInfo2, "array");

            return id;
        }else{
            printf("Error, array with more than 2 dimension is not supported.\n");
            return "";
        }
    }else{
        printf("Unknown produection, Vardec()\n");
        return "";
    }
}

void FunDec(Node* fundec, char* returnTypeName){
    if(debug_sema) printf("FunDec()\n");
    if(!fundec){
        if(debug_sema) printf("FunDec node NULL\n");
        return;
    }
    FUNC_INFO* funcInfo = (FUNC_INFO*)malloc(sizeof(FUNC_INFO));
    char* id = fundec->children[0]->propertyValue;

    if(fundec->n_children == 4){        // FunDec -> ID LP VarList RP
        // count number of parameters
        int n_params = 1;
        Node* paramNodePointer = fundec->children[2];
        while(paramNodePointer->n_children > 2){
            n_params++;
            paramNodePointer = paramNodePointer->children[2];
        }
        if(debug_sema) printf("Function %s has %d parameters\n", id, n_params);
        funcInfo->n_params = n_params;
        // analyse parameters of function declaration
        funcInfo->params = (VAR_INFO**)malloc(sizeof(VAR_INFO*) * n_params);
        for(int i = 0; i < n_params; ++i){
            funcInfo->params[i] = (VAR_INFO*)malloc(sizeof(VAR_INFO));
        }
        Node* paramNodePointer2 = fundec->children[2];  // VarList
        int i = 0;
        // processing VarList
        while(paramNodePointer2->n_children > 0){
            if(debug_sema) printf("%s\n", paramNodePointer2->name);
            Node* paramdec = paramNodePointer2->children[0];    // ParamDec
            char* paramType = Specifier(paramdec->children[0]);
            char* paramName = VarDec(paramdec->children[1], paramType);
            // set params info of function
            funcInfo->params[i]->varType = (char*)malloc(strlen(paramType)+1);
            strcpy(funcInfo->params[i]->varType, paramType);
            funcInfo->params[i]->varName = (char*)malloc(strlen(paramName)+1);
            strcpy(funcInfo->params[i]->varName, paramName);
            if(paramNodePointer2->n_children == 3){   // VarList->ParamDec COMMA VarList
                // processing next parameter
                paramNodePointer2 = paramNodePointer2->children[2];
                ++i;
            }else if(paramNodePointer2->n_children == 1){   // VarList -> ParamDec
                break;
            }else{
                printf("Error in FunDec(), unknown production for VarList\n");
                return;
            }
        }
    }else if(fundec->n_children == 3){  // FunDec -> ID LP RP
        funcInfo->n_params = 0;
        funcInfo->params = NULL;

    }else{
        printf("FunDec(), error with unknown production\n");
        return;
    }
    funcInfo->returnTypeName = (char*)malloc(strlen(returnTypeName)+1);
    strcpy(funcInfo->returnTypeName, returnTypeName);
    // create function id
    bool searchRes = searchHashTable(id);
    if(searchRes){
        int line = fundec->first_line;
        printf("Error type 4 at Line %d: Redefined function \"%s\".\n", line, id);
        return;
    }
    insertIntoHashTable(id);
    // insert symbol
    VAR_INFO* funcSymbolInfo = (VAR_INFO*)malloc(sizeof(FUNC_INFO));
    funcSymbolInfo->varType = "function";
    funcSymbolInfo->varName = (char*)malloc(strlen(id)+1);
    strcpy(funcSymbolInfo->varName, id);
    // funcSymbolInfo->ifArray = false;
    insertSymbol(funcSymbolInfo);

    // insert type
    TYPE_INFO* funcTypeInfo = (TYPE_INFO*)malloc(sizeof(TYPE_INFO));
    funcTypeInfo->typeName = (char*)malloc(strlen(id)+1);
    strcpy(funcTypeInfo->typeName, id);
    funcTypeInfo->typeCategory = "function";
    funcTypeInfo->typeDetail = (TYPE_DETAIL*)malloc(sizeof(TYPE_DETAIL));
    funcTypeInfo->typeDetail->func_info = copyFuncInfo(funcInfo);
    insertType(funcTypeInfo, "function");
    // if(debug_sema) showTypeList();
}
void CompSt(Node* compst, char* returnType){
    if(debug_sema) printf("CompSt()\n");
    if(!compst){
        if(debug_sema) printf("Compst node NULL\n");
        return;
    }
    if(compst->n_children == 4){        // CompSt -> LC DefList StmtList RC
        DefList(compst->children[1]);
        // if(debug_sema) showSymbolList();
        StmtList(compst->children[2], returnType);
        return;
    }else{
        printf("CompSt(), error with unknown production\n");
        return;
    }
}
void StmtList(Node* stmtlist, char* returnType){
    if(debug_sema) printf("StmtList()\n");
    if(!stmtlist){
        if(debug_sema) printf("StmtList node NULL\n");
        return;
    }
    if(stmtlist->n_children == 2){      // StmtList -> Stmt StmtList
        Stmt(stmtlist->children[0], returnType);
        StmtList(stmtlist->children[1], returnType);
    }else if(stmtlist->n_children == 0){    // StmtList -> empty
        return;
    }else{
        printf("StmtList(), error with unknown production\n");
        return;
    }
}
void Stmt(Node* stmt, char* returnType){
    if(debug_sema) printf("Stmt()\n");
    if(!stmt){
        if(debug_sema) printf("Stmt node NULL\n");
        return;
    }
    switch(stmt->n_children){
        case 2:{        // Stmt -> Exp SEMI
            Exp(stmt->children[0]);
            break;
        }
        case 1:{        // Stmt -> CompSt
            CompSt(stmt->children[0], returnType);
            break;
        }
        case 3:{        // Stmt -> RETURN Exp SEMI
            VAR_INFO* expInfo = Exp(stmt->children[1]);
            char* expType = expInfo->varType;
            if(strcmp(returnType, expType) != 0){
                int line = stmt->first_line;
                printf("Error type 8 at Line %d: Type mismatched for return.\n", line);
            }
            break;
        }
        case 5:{        
            if(strcmp(stmt->children[0]->name, "IF") == 0){ // Stmt -> IF LP Exp RP Stmt
                Exp(stmt->children[2]);
                Stmt(stmt->children[4], returnType);
            }else if(strcmp(stmt->children[0]->name, "WHILE") == 0){ // Stmt -> WHILE LP Exp RP Stmt
                Exp(stmt->children[2]);
                Stmt(stmt->children[4], returnType);
            }else{
                printf("Stmt(), error with unknown production\n");
                return;
            }
            break;
        }
        case 7:{    // Stmt -> IF LP Exp RP Stmt ELSE Stmt
            Exp(stmt->children[2]);
            Stmt(stmt->children[4], returnType);
            Stmt(stmt->children[6], returnType);
            break;
        }
        default:{
            printf("Stmt(), error with unknown production\n");
            return;
        }
    }
}
void DefList(Node* deflist){
    if(debug_sema) printf("DefList()\n");
    if(!deflist){
        if(debug_sema) printf("DefList node NULL\n");
        return;
    }
    if(deflist->n_children == 2){       // DefList -> Def DefList
        Def(deflist->children[0]);
        DefList(deflist->children[1]);
    }else if(deflist->n_children == 0){ // DefList -> empty
        return;
    }else{
        printf("DefList(), error with unknown production\n");
        return;
    }
}
void Def(Node* def){
    if(debug_sema) printf("Def()\n");
    if(!def){
        if(debug_sema) printf("Def node NULL\n");
        return;
    }
    if(def->n_children == 3){       // Def -> Specifier DecList SEMI
        char* typeName = Specifier(def->children[0]);
        DecList(def->children[1], typeName);
    }else{
        printf("Def(), error with unknown production\n");
        return;
    }
}
void DecList(Node* declist, char* typeName){
    if(debug_sema) printf("DecList()\n");
    if(!declist){
        if(debug_sema) printf("DecList node NULL\n");
        return;
    }
    if(declist->n_children == 1){       // DecList -> Dec
        Dec(declist->children[0], typeName);
    }else if(declist->n_children == 3){ // DecList -> Dec COMMA DecList
        Dec(declist->children[0], typeName);
        DecList(declist->children[2], typeName);
    }else{
        printf("DecList(), error with unknwon production\n");
        return;
    }
}

void Dec(Node* dec, char* typeName){
    if(debug_sema) printf("Dec()\n");
    if(!dec){
        if(debug_sema) printf("Dec node NULL\n");
        return;
    }
    if(dec->n_children == 1){           // Dec -> VarDec
        VarDec(dec->children[0], typeName);
    }else if(dec->n_children == 3){     // Dec -> VarDec ASSIGNOP Exp
        VAR_INFO* expInfo = Exp(dec->children[2]);
        if(expInfo){
            char* expType = expInfo->varType;
            if(strcmp(typeName, "") != 0 && strcmp(expType, "") != 0 && strcmp(typeName, expType) != 0){
                int line = dec->first_line;
                printf("Error type 5 at Line %d: Type mismatched for assignment.\n", line);
                return;
            }
        }
        VarDec(dec->children[0], typeName);
    }
    else{
        printf("Dec(), error with unknwon production\n");
        return;
    }
}
VAR_INFO* Exp(Node* exp){
    if(debug_sema) printf("Exp()\n");
    if(!exp){
        if(debug_sema) printf("Exp node NULL\n");
        return NULL;
    }
    switch(exp->n_children){
        case 1:{
            if(strcmp(exp->children[0]->name, "ID") == 0){  // Exp -> ID
                char* id = exp->children[0]->propertyValue;
                bool searchRes = searchHashTable(id);
                if(!searchRes){
                    int line = exp->first_line;
                    printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", line, id);
                    return NULL;
                }
                VAR_INFO* varInfo = getSymbolInfo(id);
                return varInfo;
            }else if(strcmp(exp->children[0]->name, "INT") == 0){  // Exp -> INT
                VAR_INFO* varInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                varInfo->varType = "int";
                varInfo->varName = "";      // varName == "" means constant or right-side value
                varInfo->ifArray = false;
                return varInfo;
            }else if(strcmp(exp->children[0]->name, "FLOAT") == 0){ // Exp -> FLOAT
                VAR_INFO* varInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                varInfo->varType = "float";
                varInfo->varName = "";
                varInfo->ifArray = false;
                return varInfo;    
            }else{
                printf("Exp() case 1, error with unknown production\n");
                return NULL;
            }
            break;
        }
        case 2:{
            if(strcmp(exp->children[0]->name, "MINUS") == 0){   // Exp -> MINUS Exp
                VAR_INFO* expInfo = Exp(exp->children[1]);
                return expInfo;
            }else if(strcmp(exp->children[0]->name, "NOT") == 0){   // Exp -> NOT Exp
                VAR_INFO* expInfo = Exp(exp->children[1]);
                return expInfo;
            }else{
                printf("Exp() case 2, error with unknown production\n");
                return NULL;
            }
            break;
        }
        case 3:{
            if(strcmp(exp->children[1]->name, "ASSIGNOP") == 0){    // Exp -> Exp ASSIGNOP Exp
                // Assignment left-side check
                VAR_INFO* leftExpInfo = Exp(exp->children[0]);
                if(!leftExpInfo) return NULL;       // some error occurs
                int line = exp->first_line;
                if(strcmp(leftExpInfo->varName, "") == 0){
                    printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", line);
                    return NULL;
                }

                VAR_INFO* rightExpInfo = Exp(exp->children[2]);
                if(!rightExpInfo) return NULL;
                // type check
                if(strcmp(leftExpInfo->varType, rightExpInfo->varType) != 0){
                    printf("Error type 5 at Line %d: Type mismatched for assignment.\n", line);
                    return NULL;
                }

                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = "assignment";
                returnInfo->ifArray = false;
                return returnInfo;

            }else if(strcmp(exp->children[1]->name, "AND") == 0){   // Exp -> Exp AND Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = "bool";
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[1]->name, "OR") == 0){    // Exp -> Exp OR Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = "bool";
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[1]->name, "RELOP") == 0){ // Exp -> Exp RELOP Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                if((strcmp(type1, "int") != 0 && strcmp(type1, "float") != 0) ||
                    (strcmp(type2, "int") != 0 && strcmp(type2, "float") != 0) ||
                    strcmp(type1, type2) != 0){
                    int line = exp->first_line;
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = "bool";
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[1]->name, "PLUS") == 0){  // Exp -> Exp PLUS Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                int line = exp->first_line;
                if((strcmp(type1, "int")!=0 && strcmp(type1, "float")!=0) || (strcmp(type2, "int")!=0 && strcmp(type2, "float")!=0)){
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                if(strcmp(type1, type2) != 0){
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(type1)+1);
                strcpy(returnInfo->varType, type1);
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[1]->name, "MINUS") == 0){ // Exp -> Exp MINUS Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                if(strcmp(type1, type2) != 0){
                    int line = exp->first_line;
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(type1)+1);
                strcpy(returnInfo->varType, type1);
                returnInfo->ifArray = false;
                return returnInfo;

            }else if(strcmp(exp->children[1]->name, "STAR") == 0){  // Exp -> Exp STAR Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                if(strcmp(type1, type2) != 0){
                    int line = exp->first_line;
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(type1)+1);
                strcpy(returnInfo->varType, type1);
                returnInfo->ifArray = false;
                return returnInfo;

            }else if(strcmp(exp->children[1]->name, "DIV") == 0){   // Exp -> Exp DIV Exp
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                if(strcmp(type1, type2) != 0){
                    int line = exp->first_line;
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(type1)+1);
                strcpy(returnInfo->varType, type1);
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[1]->name, "Exp") == 0){   // Exp -> LP Exp RP
                VAR_INFO* expInfo = Exp(exp->children[1]);
                return expInfo;
            }else if(strcmp(exp->children[1]->name, "LP") == 0){    // Exp -> ID LP RP
                // call function
                char* id = exp->children[0]->propertyValue;
                int line = exp->first_line;
                VAR_INFO* getVarInfo = getSymbolInfo(id);
                // not a function
                if(strcmp(getVarInfo->varType, "function") != 0){
                    printf("Error type 11 at Line %d: \"%s\" is not a function.\n", line, id);
                    return NULL;
                }
                // function not defined
                if(!searchHashTable(id)){
                    printf("Error type 2 at Line %d: undefined function \"%s\".\n", line, id);
                    return NULL;
                }
                TYPE_INFO* funcTypeInfo = getTypeInfo(id);
                FUNC_INFO* funcInfo = funcTypeInfo->typeDetail->func_info;
                // function call and argument don't match
                if(funcInfo->n_params != 0){
                    printf("Error type 9 at Line %d: Function call is not applicable for arguments.\n", line);
                    return NULL;
                }
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(funcInfo->returnTypeName)+1);
                strcpy(returnInfo->varType, funcInfo->returnTypeName);
                returnInfo->ifArray = false;
                return returnInfo;

            }else if(strcmp(exp->children[1]->name, "DOT") == 0){   // Exp -> Exp DOT ID
                VAR_INFO* expInfo = Exp(exp->children[0]);
                // some error occurs in left-hand expression
                if(!expInfo) return NULL;
                char* expType = expInfo->varType;
                char* id = exp->children[2]->propertyValue;
                int line = exp->first_line;
                // int or float
                if(strcmp(expType, "int") == 0 || strcmp(expType, "float") == 0){
                    printf("Error type 13 at Line %d: Illegal use of \".\".\n", line);
                    return NULL;
                }
                TYPE_INFO* typeInfo = getTypeInfo(expType);
                // other complex types except struct
                if(strcmp(typeInfo->typeCategory, "struct") != 0){
                    printf("Error type 13 at Line %d: Illegal use of \".\".\n", line);
                    return NULL;
                }
                STRUCT_INFO* structDetail = typeInfo->typeDetail->struct_info;
                for(int i = 0; i < structDetail->n_fields; ++i){
                    VAR_INFO* p = structDetail->fields[i];
                    if(strcmp(id, p->varName) == 0){
                        VAR_INFO* returnInfo = copyVarInfo(p);
                        return returnInfo;
                    }
                }
                printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", line, id);
                return NULL;
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
                bool searchRes = searchHashTable(id);
                if(!searchRes) {
                    int line = exp->children[0]->first_line;
                    printf("Error type 2 at Line %d: Undefined function \"%s\".\n", line, id);
                    return NULL;
                }
                VAR_INFO* idInfo = getSymbolInfo(id);
                if(strcmp(idInfo->varType, "function") != 0){
                    int line = exp->first_line;
                    printf("Error type 11 at Line %d: \"%s\" is not a function.\n", line, id);
                    return NULL;
                }
                char* returnTypeName = Args(exp->children[2], id);
                
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                returnInfo->varName = "";
                returnInfo->varType = (char*)malloc(strlen(returnTypeName)+1);
                strcpy(returnInfo->varType, returnTypeName);
                returnInfo->ifArray = false;
                return returnInfo;
            }else if(strcmp(exp->children[0]->name, "Exp") == 0){   // Exp -> Exp LB Exp RB
                VAR_INFO* expInfo1 = Exp(exp->children[0]);
                VAR_INFO* expInfo2 = Exp(exp->children[2]);
                // some error occurs in sub-expression
                if((!expInfo1) || (!expInfo2)) return NULL;
                char *type1 = expInfo1->varType, *type2 = expInfo2->varType;
                int line = exp->first_line;
                if(strcmp(type2, "int") != 0){
                    printf("Error type 12 at Line %d: subscript is not an integer.\n", line);
                    return NULL;
                }
                
                // basic type
                /*
                if(strcmp(type1,"int") == 0 || strcmp(type1, "float") == 0){
                    printf("Error type 10 at Line %d: illegal subscript for a non-array variable.\n", line);
                    return NULL;
                }
                */
                if(!expInfo1->ifArray){
                    printf("Error type 10 at Line %d: illegal subscript with a non-array variable.\n", line);
                    return NULL;
                }
                    
                TYPE_INFO* arrayInfo = getTypeInfo(expInfo1->varName);
                char* eleType = arrayInfo->typeDetail->array_info->eleTypeName;
                VAR_INFO* returnInfo = (VAR_INFO*)malloc(sizeof(VAR_INFO));
                if(strcmp(eleType, "int") == 0 || strcmp(eleType, "float") == 0){
                    returnInfo->ifArray = false;
                }else{
                    TYPE_INFO* tInfo = getTypeInfo(eleType);
                    if(strcmp(tInfo->typeCategory, "array") == 0) returnInfo->ifArray = true;
                    else returnInfo->ifArray = false;
                }
                // returnInfo->ifArray = false;
                returnInfo->varName = "arrayElem";      // non-empty
                returnInfo->varType = (char*)malloc(strlen(type1)+1);
                strcpy(returnInfo->varType, type1);
                return returnInfo;
            }else{
                printf("Exp() case 4, error with unknown production\n");
                return NULL;
            }
            break;
        }
        default:{
            printf("Exp(), error with unknown production\n");
            return NULL;
        }
    }
}

char* Args(Node* args, char* funcName){
    if(debug_sema) printf("Args()\n");
    if(!args){
        if(debug_sema) printf("Args node NULL\n");
        return "";
    }
    TYPE_INFO* funcTypeInfo = getTypeInfo(funcName);
    if(strcmp(funcTypeInfo->typeCategory, "function") != 0){
        printf("Error, %s is not a function\n", funcName);
        return "";
    }
    FUNC_INFO* funcInfo = funcTypeInfo->typeDetail->func_info;
    int nFuncParam = funcInfo->n_params;
    VAR_INFO** params = funcInfo->params;

    int line = args->first_line;

    if(args->n_children == 3){      // Args -> Exp COMMA Args
        int n_params = 1;
        Node* moreArgs = args->children[2];
        while(moreArgs->n_children > 0){
            n_params++;
            if(moreArgs->n_children == 3) moreArgs = moreArgs->children[2];
            else if(moreArgs->n_children == 1) break;
            else{
                printf("Error, unknwon production in Args()\n");
                return "";
            }
        }
        if(nFuncParam != n_params){
            printf("Error type 9 at Line %d: Function call is not applicable with arguments.\n", line);
            return "";
        }

        // traverse all parameters
        int i = 0;
        Node* tmpArg = args;
        while(i < n_params){
            VAR_INFO* paramInfo = Exp(tmpArg->children[0]);
            char* paramType = paramInfo->varType;
            char* argType = params[i]->varType;
            if(strcmp(paramType, argType) != 0){
                printf("Error type 9 at Line %d: Function call is not applicable with arguments.\n", line);
                return "";
            }
            if(tmpArg->n_children == 3) {
                tmpArg = tmpArg->children[2];
                ++i;
            }
            else if(tmpArg->n_children == 1) break;
            else{
                printf("Error unknown production in Args()...\n");
                return "";
            }
        }

        return funcInfo->returnTypeName;
    }else if(args->n_children == 1){    // Args -> Exp
        if(nFuncParam != 1){
            printf("Error type 9 at Line %d: Function call is not applicable with arguments.\n", line);
            return "";
        }
        VAR_INFO* paramInfo = Exp(args->children[0]);
        char* paramType = paramInfo->varType;
        if(strcmp(paramType, "") == 0) return "";
        char* argType = params[0]->varType;
        if(strcmp(paramType, argType) != 0){
            printf("Error type 9 at Line %d: Function call is not applicable with arguments.\n", line);
            return "";
        }
        return funcInfo->returnTypeName;

    }else{
        printf("Args(), error with unknown production\n");
        return "";
    }
}

void showSymbolList(){
    if(!symbol_list_head){
        printf("Symbol list empty.\n");
        return;
    }
    SYMBOL_NODE* p = symbol_list_head;
    while(p){
        VAR_INFO* info = p->info;
        printf("(varName: %s, varType: %s, ifArray: %d)\n", info->varName, info->varType, info->ifArray);
        p = p->next;
    }
}

void showTypeList(){
    if(!type_list_head){
        printf("Type list empty.\n");
        return;
    }
    TYPE_NODE* p = type_list_head;
    while(p){
        TYPE_INFO* info = p->info;
        printf("(typeCategory:%s, typeName:%s)\n", info->typeCategory, info->typeName);
        p = p->next;
    }
}

