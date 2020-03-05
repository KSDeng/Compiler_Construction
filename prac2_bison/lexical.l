%{
    #include <stdio.h>
    #include "syntax.tab.h"
%}

DIGIT       [0-9]
INT         {DIGIT}+
WHITESPACE  ([ \t\r\n])+

%%

"+"         {   return ADD;   }
"-"         {   return SUB;   }
"*"         {   return MUL;   }
"/"         {   return DIV;   }
{INT}       {   yylval = atoi(yytext); 
                return INT;   }

%%
