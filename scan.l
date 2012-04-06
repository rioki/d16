%{
#include <assert.h>
#include <stdio.h>

#include "scan.h"
%}
 
comment     ;[^\r\n]*
ws          [ \t]+ 
nl          (\r)|(\n)|(\r\n)
ident       [a-zA-Z_][0-9a-zA-Z_]*
label       :[a-zA-Z_][0-9a-zA-Z_]*
oct         0[0-7]+
dec         [1-9][0-9]*
hex         0x[0-9A-Fa-f]+

    static const char* file;
    static int line = 1;
     
    static variant_t* value;
    
%%

{comment}   /* skip comments */

{ws}        /* skip white spaces */

{nl}        {
                line++;
                return EOL_TOKEN;
            }

,           return COMMA_TOKEN;
\[          return LEFT_BRAKET_TOKEN;
\]          return RIGHT_BRAKET_TOKEN;            
\+          return PLUS_TOKEN;            

SET         return SET_TOKEN;
ADD         return ADD_TOKEN;
SUB         return SUB_TOKEN;
MUL         return MUL_TOKEN;
DIV         return DIV_TOKEN;
MOD         return MOD_TOKEN;
SHL         return SHL_TOKEN;
SHR         return SHR_TOKEN;
AND         return AND_TOKEN;
BOR         return BOR_TOKEN;
XOR         return XOR_TOKEN;
IFE         return IFE_TOKEN;
IFN         return IFN_TOKEN;
IFG         return IFG_TOKEN;
IFB         return IFB_TOKEN;

JSR         return JSR_TOKEN;

A           return A_TOKEN;
B           return B_TOKEN;
C           return C_TOKEN;
X           return X_TOKEN;
Y           return Y_TOKEN;
Z           return Z_TOKEN;
I           return I_TOKEN;
J           return J_TOKEN;

POP         return POP_TOKEN;
PEEK        return PEEK_TOKEN;
PUSH        return PUSH_TOKEN;
SP          return SP_TOKEN;
PC          return PC_TOKEN;
O           return O_TOKEN;

0           {
                /* Yea we need to handle zero explicitly, since all other codes require values >1 */
                value->type = UINT_TYPE;
                value->ui = 0;
                return INTEGER_TOKEN;
            }
            
{oct}       {
                /* TODO: Handle overflow of the integer. */
                value->type = UINT_TYPE;
                value->ui = strtoul(yytext + 1, NULL, 8);
                return INTEGER_TOKEN;
            }

{dec}       {
                value->type = UINT_TYPE;
                value->ui = strtoul(yytext, NULL, 10);
                return INTEGER_TOKEN;
            }

{hex}       {
                value->type = UINT_TYPE;
                value->ui = strtoul(yytext + 2, NULL, 16);
                return INTEGER_TOKEN;
            }

                        
{ident}     {
                value->type = STRING_TYPE;
                value->str = strdup(yytext);
                return IDENTIFIER_TOKEN;
            }
            

{label}     {
                value->type = STRING_TYPE;
                value->str = strdup(yytext + 1);
                return LABEL_TOKEN;
            }

<<EOF>>     {
                return EOF_TOKEN;
            }
            
.           {
                fprintf(stderr, "%s:%d: error: Unknown token %s.\n", file, line, yytext);
                return ERROR_TOKEN;
            }
            
%%
 
int yywrap(void)    
{
    return 1;
}

int start_scan(FILE* fp, const char* f)
{
    assert(fp != NULL);
    assert(f != NULL);

    yyin = fp;
    file = f;
    
    return 0;
}

int scan_token(variant_t* v)
{
    assert(v != NULL);    
    value = v;
    return yylex();
}

const char* get_scan_file()
{
    return file;
}

unsigned int get_scan_line()
{
    return line;
}

static const char* token_names[] = {
    "ERROR",
    ",",
    "[",
    "]",
    "+",
    "SET",
    "ADD",
    "SUB",
    "MUL", 
    "DIV",
    "MOD", 
    "SHL", 
    "SHR", 
    "AND", 
    "BOR", 
    "XOR", 
    "IFE", 
    "IFN", 
    "IFG", 
    "IFB",
    "A",
    "B",
    "C",
    "X",
    "Y",
    "Z",
    "I",
    "J",
    "identifier",
    "label",
    "integer",
    "end of line",
    "end of file"
};

const char* get_token_name(int token)
{
    assert(token < sizeof(token_names));
    return token_names[token];
}
