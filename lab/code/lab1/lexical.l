%{
    #include "parseTree.h"
    #include "syntax.tab.h"
    int np = 0, nb = 0, nc = 0;
    int line_p = 0, line_b = 0, line_c = 0;
    int n_error = 0;            // number of error
    extern bool debug;
    // Bison location information
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    #define NEW_NODE(token_name) \
        yylval.type_node = createNode(#token_name, yytext, yylloc.first_line);   \
        (yylval.type_node) -> if_leaf = true

    #define INFO(STR) if(debug) printf("[%d] %s: %s\n", yylineno, #STR, yytext)

%}
%option yylineno

DIGIT       [0-9]
INT         {DIGIT}+
FLOAT       {INT}\.{DIGIT}+
LETTER_     [a-zA-Z_]
ID          {LETTER_}({LETTER_}|{DIGIT})*
SEMI        ";"
COMMA       ","
ASSIGNOP    "="
RELOP       ">"|"<"|">="|"<="|"=="|"!="
PLUS        "+"
MINUS       "-"
STAR        "*"
DIV         "/"
AND         "&&"
OR          "||"
DOT         "."
NOT         "!"
TYPE        int|float
LP          "("
RP          ")"
LB          "["
RB          "]"
LC          "{"
RC          "}"
STRUCT      "struct"
RETURN      "return"
IF          "if"
ELSE        "else"
WHILE       "while"
MAIN        "main"
WHITESPACE  [ \t\r\n]+
INVALID_ID  ({DIGIT})+{ID}

%%



\n              {   yycolumn = 1; }

{INT}           {   
                    INFO(INT);
                    NEW_NODE(INT);
                    return INT;         }
{FLOAT}         {   INFO(FLOAT);
                    NEW_NODE(FLOAT);
                    return FLOAT;       }
{SEMI}          {   INFO(SEMI);
                    NEW_NODE(SEMI);
                    return SEMI;        }
{COMMA}         {   INFO(COMMA);
                    NEW_NODE(COMMA);
                    return COMMA;       }
{ASSIGNOP}      {   INFO(ASSIGNOP);
                    NEW_NODE(ASSIGNOP);
                    return ASSIGNOP;    }
{RELOP}         {   INFO(RELOP);
                    NEW_NODE(RELOP);
                    return RELOP;       }
{PLUS}          {   INFO(PLUS);
                    NEW_NODE(PLUS);
                    return PLUS;        }
{MINUS}         {   INFO(MINUS);
                    NEW_NODE(MINUS);
                    return MINUS;       }
{STAR}          {   INFO(STAR);
                    NEW_NODE(STAR);
                    return STAR;        }
{DIV}           {   INFO(DIV);
                    NEW_NODE(DIV);
                    return DIV;         }
{AND}           {   INFO(AND);
                    NEW_NODE(AND);
                    return AND;         }
{OR}            {   INFO(OR);
                    NEW_NODE(OR);
                    return OR;          }
{DOT}           {   INFO(DOT);
                    NEW_NODE(DOT);
                    return DOT;         }
{NOT}           {   INFO(NOT);
                    NEW_NODE(NOT);
                    return NOT;         }
{TYPE}          {   INFO(TYPE);
                    NEW_NODE(TYPE);
                    return TYPE;        }
{LP}            {   INFO(LP); np++; line_p = yylineno;
                    NEW_NODE(LP);
                    return LP;          }
{RP}            {   INFO(RP); np--; line_p = yylineno;
                    if(np < 0) {
                        printErrorTypeB("Parentheses not match, redundant \")\"", yylineno);
                        np = 0;
                    }
                    NEW_NODE(RP);
                    return RP;          }
{LB}            {   INFO(LB); nb++; line_b = yylineno;
                    NEW_NODE(LB);
                    return LB;          }
{RB}            {   INFO(RB); nb--; line_b = yylineno;
                    if(nb < 0) {
                        printErrorTypeB("Brackets not match, redundant \"]\"", yylineno);
                        nb = 0;
                    }
                    NEW_NODE(RB);
                    return RB;          }
{LC}            {   INFO(LC); nc++; line_c = yylineno;
                    NEW_NODE(LC);
                    return LC;          }
{RC}            {   INFO(RC); nc--; line_c = yylineno;
                    if(nc < 0) {
                        printErrorTypeB("Brackets not match, redundant \"}\"", yylineno);
                        nc = 0;
                    }
                    NEW_NODE(RC);
                    return RC;          }
{STRUCT}        {   INFO(STRUCT);
                    NEW_NODE(STRUCT);
                    return STRUCT;      }
{RETURN}        {   INFO(RETURN);
                    NEW_NODE(RETURN);
                    return RETURN;      }
{IF}            {   INFO(IF);
                    NEW_NODE(IF);
                    return IF;          }
{ELSE}          {   INFO(ELSE);
                    NEW_NODE(ELSE);
                    return ELSE;        }
{WHILE}         {   INFO(WHILE);
                    NEW_NODE(WHILE);
                    return WHILE;       }
{ID}            {   INFO(ID);
                    NEW_NODE(ID);
                    return ID;          }
{WHITESPACE}+   {   /* Do nothing */    }

{INVALID_ID}    {   INFO(INVALID_ID);
                    NEW_NODE(INVALID_ID);
                    return INVALID_ID;  }
.               {   printErrorTypeA(yytext, yylineno);  
                    n_error++;          }

%%





