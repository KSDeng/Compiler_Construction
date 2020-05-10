#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include "parseTree.h"
#define HASH_TABLE_SIZE 24593

// Hash node to implement local scope
struct HASH_TABLE_NODE {
    bool hash_table[HASH_TABLE_SIZE];
    struct HASH_TABLE_NODE* parent;
    struct HASH_TABLE_NODE** children;
    int n_children;
};
typedef struct HASH_TABLE_NODE HASH_NODE;

HASH_NODE* createHashNode(HASH_NODE* parent);
HASH_NODE* searchHashTableInNode(char* str, HASH_NODE* node);
void insertIntoHashTableInNode(char* str, HASH_NODE** node);

// debug flag
// bool debug_sema = true;

// array info
struct ARRAY_INFO {    
    int size;               // size of array
    // type info of array element
    // to get detailed type info, use getTypeInfo(eleTypeName)
    char* eleTypeName;      // type name of element
};

// struct info
struct STRUCT_INFO {
    int n_fields;   // num of fields
    struct VAR_INFO** fields;         // fields of struct (var type and var name)
};

// func info
struct FUNC_INFO {
    int n_params;       // num of parameters
    struct VAR_INFO** params;     // parameters of function (var type and var name)
    char* returnTypeName;   // type name of return value
};


// Legal type names are as follows:
// "int"
// "float"
// self-defined type name (struct ID / function ID / array ID)
// typeName + "_pointer" (only 1-demisional pointer)

// A variable should be added to SYMBOL_LIST after it has been defined
// A type should be added to TYPE_LIST after it has been defined

struct VAR_INFO {
    char* varType;  // int->"int", float->"float", array->element type name, function->"function", stru->"struct"
    char* varName;
    bool ifArray;   // array->true, non-array->false
};
// information of certain type
// for basic type, the typeDetail pointer is NULL
// allow 2-D array (enumeration in code when defining)
// don'e allow nested structure/function
struct TYPE_INFO {
    char* typeName;
    char* typeCategory;     // "array", "function" or "struct"
    union TYPE_DETAIL* typeDetail;
};
// detail of complex type
union TYPE_DETAIL {
    struct ARRAY_INFO* array_info;
    struct STRUCT_INFO* struct_info;
    struct FUNC_INFO* func_info;
};

// symbol list node
// constants won't be added into this list, but they have their types
// each node represents a VARIABLE that has already been defined
struct SYMBOL_LIST_NODE {
    struct VAR_INFO* info;
    struct SYMBOL_LIST_NODE* next;
};
// type list node
// each node represents a TYPE that has already been defined
struct TYPE_LIST_NODE {
    struct TYPE_INFO* info;
    struct TYPE_LIST_NODE* next;
};

// typedef
typedef struct ARRAY_INFO ARRAY_INFO;
typedef struct STRUCT_INFO STRUCT_INFO;
typedef struct FUNC_INFO FUNC_INFO;

typedef struct VAR_INFO VAR_INFO;
typedef struct TYPE_INFO TYPE_INFO;
typedef union TYPE_DETAIL TYPE_DETAIL;
typedef struct SYMBOL_LIST_NODE SYMBOL_NODE;
typedef struct TYPE_LIST_NODE TYPE_NODE;

// hash table element
struct HASH_ELE {
    char* str;
    bool status;
};
typedef struct HASH_ELE HASH_ELE;

// hashing functions
unsigned int BKDRHash(char* str);
void initHashTable();                     // init all hash table elements to be false
void insertIntoHashTable(char* str);      // turn the flag in hash table, solve conflicts at the same time
bool searchHashTable(char* str);          // search if a name has already existed

// list functions
void insertSymbol(VAR_INFO* varInfo);      // insert a new symbol into symbol list
VAR_INFO* getSymbolInfo(char* varName);        // search for symbol info by its name
void insertType(TYPE_INFO* typeInfo, const char* typeCategory);       // insert a new type into type list
TYPE_INFO* getTypeInfo(char* typeName);     // search for type info by its name

// semantic analysis
// return value: return to parent level
// parameter: pass to sub level

// High-level Definitions
void Program(Node* program);
void ExtDefList(Node* extdeflist);
void ExtDef(Node* extdef);
void ExtDecList(Node* extdeclist, char* typeName);
// Specifiers
char* Specifier(Node* specifier);       // return a string representing TYPE name, then you can get the detail of this type by calling getTypeInfo(char* typeName)
char* StructSpecifier(Node* structspecifier);   // return structure name
char* OptTag(Node* opttag);         // return ID
// char* Tag(Node* tag);               // return ID
// Declarations
char* VarDec(Node* vardec, char* typeName);
void FunDec(Node* fundec, char* returnTypeName);      // insert symbol: function name
// VAR_INFO** VarList(Node* varlist);
// VAR_INFO* ParamDec(Node* paramdec);  // insert symbol: int/float/struct/array (function params)
// Statements
void CompSt(Node* compst, char* returnType);         // returnType: function return type
void StmtList(Node* stmtlist, char* returnType);     // returnType: function return type
void Stmt(Node* stmt, char* returnType);             // returnType: function return type
// Local Definitions
void DefList(Node* deflist);
void Def(Node* def);            // insert symbol or type: int/float/struct/array
void DecList(Node* declist, char* typeName);        // insert symbol into symbol list
void Dec(Node* dec, char* typeName);
// Expressions
VAR_INFO* Exp(Node* exp);           // return a string representing TYPE name
char* Args(Node* args, char* funcName);     // return function return type name

// utils
VAR_INFO* copyVarInfo(VAR_INFO* src);
TYPE_INFO* copyTypeInfo(TYPE_INFO* src, const char* typeCategory);
ARRAY_INFO* copyArrayInfo(ARRAY_INFO* src);
STRUCT_INFO* copyStructInfo(STRUCT_INFO* src);
FUNC_INFO* copyFuncInfo(FUNC_INFO* src);
void showSymbolList();
void showTypeList();

#endif
