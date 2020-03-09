%{
    #include <stdio.h>
    #include "lex.yy.c"
    void printDebug2(char* str, int lineno){
        printf("%s (%d)\n",str, lineno);
    }
%}

/* Tokens */
%token INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token AND OR DOT NOT TYPE LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

/* Associative property and priority */
/* https://zh.cppreference.com/w/c/language/operator_precedence */
%left COMMA
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%

/* High-level Definitions */
Program : ExtDefList        { printDebug2("Program -> ExtDefList", @$.first_line);   }
    ;

ExtDefList : ExtDef ExtDefList  { printDebug2("ExtDefList -> ExtDef ExtDefList", @$.first_line);    }
    |   /*  empty   */          { printDebug2("ExtDefList -> Empty", @$.first_line);    }
    ;

ExtDef : Specifier ExtDecList SEMI  { printDebug2("ExtDef -> Specifier ExtDecList SEMI", @$.first_line);  }
    | Specifier SEMI                { printDebug2("ExtDef -> Specifier SEMI", @$.first_line);  }
    | Specifier FunDec CompSt       { printDebug2("ExtDef -> Specifier FuncDec CompSt", @$.first_line);  }
    ;

ExtDecList : VarDec     { printDebug2("ExtDecList -> VarDec", @$.first_line);  }
    | VarDec COMMA ExtDecList   { printDebug2("ExtDecList -> VarDec COMMA ExtDecList", @$.first_line); }
    ;

/* Specifiers */
Specifier : TYPE    { printDebug2("Specifier -> TYPE", @$.first_line); }
    | StructSpecifier   { printDebug2("Specifier -> StructSpecifier", @$.first_line);  }
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { printDebug2("StructSpecifier -> STRUCT OptTag LC DefList RC", @$.first_line);    }
    | STRUCT Tag    { printDebug2("StructSpecifier -> STRUCT Tag", @$.first_line); }
    ;

OptTag : ID     { printDebug2("OptTag -> ID", @$.first_line);  }
    |   /*  empty  */   { printDebug2("OptTag -> Empty", @$.first_line);   }
    ;

Tag : ID    { printDebug2("Tag -> ID", @$.first_line); }
    ;

/* Declarations */
VarDec : ID    { printDebug2("VarDec -> ID", @$.first_line);   }
    | VarDec LB INT RB  { printDebug2("VarDec -> VarDec LB INT RB", @$.first_line);    }
    ;

FunDec : ID LP VarList RP   { printDebug2("FunDec -> ID LP VarList RP", @$.first_line);    }
    | ID LP RP  { printDebug2("FunDec -> ID LP RP", @$.first_line);    }
    ;

VarList : ParamDec COMMA VarList    { printDebug2("VarList -> ParamDec COMMA VarList", @$.first_line); }
    | ParamDec  { printDebug2("VarList -> ParamDec", @$.first_line);   }
    ;

ParamDec : Specifier VarDec     { printDebug2("ParamDec -> Specifier VarDec", @$.first_line);  }
    ;

/* Statements */
CompSt : LC DefList StmtList RC     { printDebug2("CompSt -> LC DefList StmtList RC", @$.first_line);  }
    ;

StmtList : Stmt StmtList    { printDebug2("StmtList -> Stmt StmtList", @$.first_line); }
    |   /*  empty   */      { printDebug2("StmtList -> Empty", @$.first_line); }
    ;

Stmt : Exp SEMI     { printDebug2("Stmt -> Exp SEMI", @$.first_line);  }
    | CompSt        { printDebug2("Stmt -> CompSt", @$.first_line);    }
    | RETURN Exp SEMI   { printDebug2("Stmt -> RETURN Exp SEMI", @$.first_line);   }
    | IF LP Exp RP Stmt     { printDebug2("Stmt -> IF LP Exp RP Stmt", @$.first_line); }
    | IF LP Exp RP Stmt ELSE Stmt   { printDebug2("Stmt -> IF LP Exp RP Stmt ELSE Stmt", @$.first_line);   }
    | WHILE LP Exp RP Stmt      { printDebug2("Stmt -> WHILE LP Exp RP Stmt", @$.first_line);  }
    ;

/* Local Definitions */
DefList : Def DefList       { printDebug2("DefList -> Def DefList", @$.first_line);    }
    |   /* empty */         { printDebug2("DefList -> Empty", @$.first_line);  }
    ;

Def : Specifier DecList SEMI        { printDebug2("Def -> Specifier DecList SEMI", @$.first_line); }
    ;

DecList : Dec       { printDebug2("DecList -> Dec", @$.first_line);    }
    | Dec COMMA DecList     { printDebug2("DecList -> Dec COMMA DecList", @$.first_line);  }
    ;

Dec : VarDec        { printDebug2("Dec -> VarDec", @$.first_line); }
    | VarDec ASSIGNOP Exp       { printDebug2("Dec -> VarDec ASSIGNOP Exp", @$.first_line);    }
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp      { printDebug2("Exp -> Exp ASSIGNOP Exp", @$.first_line);   }
    | Exp AND Exp           { printDebug2("Exp -> Exp AND Exp", @$.first_line);        }
    | Exp OR Exp            { printDebug2("Exp -> Exp OR Exp", @$.first_line);         }
    | Exp RELOP Exp         { printDebug2("Exp -> Exp RELOP Exp", @$.first_line);      }
    | Exp PLUS Exp          { printDebug2("Exp -> Exp PLUS Exp", @$.first_line);       }
    | Exp MINUS Exp         { printDebug2("Exp -> Exp MINUS Exp", @$.first_line);      }
    | Exp STAR Exp          { printDebug2("Exp -> Exp STAR Exp", @$.first_line);       }
    | Exp DIV Exp           { printDebug2("Exp -> Exp DIV Exp", @$.first_line);        }
    | LP Exp RP             { printDebug2("Exp -> LP Exp RP", @$.first_line);          }
    | MINUS Exp             { printDebug2("Exp -> MINUS Exp", @$.first_line);          }
    | NOT Exp               { printDebug2("Exp -> NOT Exp", @$.first_line);            }
    | ID LP Args RP         { printDebug2("Exp -> ID LP Args RP", @$.first_line);      }
    | ID LP RP              { printDebug2("Exp -> ID LP RP", @$.first_line);           }
    | Exp LB Exp RB         { printDebug2("Exp -> Exp LB Exp RB", @$.first_line);      }
    | Exp DOT ID            { printDebug2("Exp -> Exp DOT ID", @$.first_line);         }
    | ID                    { printDebug2("Exp -> ID", @$.first_line);                 }
    | INT                   { printDebug2("Exp -> INT", @$.first_line);                }
    | FLOAT                 { printDebug2("Exp -> FLOAT", @$.first_line);              }
    ;

Args : Exp COMMA Args       { printDebug2("Args -> Exp COMMA Args", @$.first_line);    }
    | Exp                   { printDebug2("Args -> Exp", @$.first_line);               }
    ;









