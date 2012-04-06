
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "scan.h"
#include "cpu.h"

static int token = 0;
static int next_token = 0;
static variant_t value;
static variant_t next_value;

static int result = 0;
static program_t* prog;

void read_token()
{
    clear_variant(&value);
    
    token = next_token;
    value = next_value;
    
    init_variant(&next_value);
    next_token = scan_token(&next_value);
}

int add_instruction(program_t* prog, instruction_t instr)
{
    assert(prog != NULL);
    
    assert(prog->code_lines <= prog->code_size);
    
    if (prog->code_lines == prog->code_size)
    {
        if (prog->code_size == 0)
        {
            prog->code = (instruction_t*)malloc(32 * sizeof(instruction_t));
            prog->code_size = 32;
        }
        else
        {
            unsigned int new_size = prog->code_size * 2;
            prog->code = (instruction_t*)realloc(prog->code, new_size * sizeof(instruction_t));
            prog->code_size = new_size;
        }
    }
    
    assert(prog->code_lines < prog->code_size);
    
    prog->code[prog->code_lines] = instr;
    prog->code_lines++;
    
    return 0;
}

void read_label(char** label)
{
    assert(label != NULL);

    read_token();
    assert(token == LABEL_TOKEN);
    assert(value.type == STRING_TYPE);
    
    *label = strdup(value.str);
}

int is_opcode(int token)
{
    switch (token)
    {
        case SET_TOKEN:
        case ADD_TOKEN:
        case SUB_TOKEN:
        case MUL_TOKEN:
        case DIV_TOKEN:
        case MOD_TOKEN:
        case SHL_TOKEN:
        case SHR_TOKEN:
        case AND_TOKEN:
        case BOR_TOKEN:
        case XOR_TOKEN:
        case IFE_TOKEN:
        case IFN_TOKEN:
        case IFG_TOKEN:
        case IFB_TOKEN:
        case JSR_TOKEN:
            return 1;
        default:
            return 0;
    }
}

int map_opcode(int token)
{
    switch (token)
    {
        case SET_TOKEN:
            return SET_OPCODE;
        case ADD_TOKEN:
            return ADD_OPCODE;
        case SUB_TOKEN:
            return SUB_OPCODE;
        case MUL_TOKEN:
            return MUL_OPCODE;
        case DIV_TOKEN:
            return DIV_OPCODE;
        case MOD_TOKEN:
            return MOD_OPCODE;
        case SHL_TOKEN:
            return SHL_OPCODE;
        case SHR_TOKEN:
            return SHR_OPCODE;
        case AND_TOKEN:
            return AND_OPCODE;
        case BOR_TOKEN:
            return BOR_OPCODE;
        case XOR_TOKEN:
            return XOR_OPCODE;
        case IFE_TOKEN:
            return IFE_OPCODE;
        case IFN_TOKEN:
            return IFN_OPCODE;
        case IFG_TOKEN:
            return IFG_OPCODE;
        case IFB_TOKEN:
            return IFB_OPCODE; 
        case JSR_TOKEN:
            return JSR_OPCODE;
        default:
            assert(0 && "Should never happen.");
            return 0;
    }
}

void read_opcode(int* opcode)
{
    assert(opcode != NULL);
    
    read_token();
    if (is_opcode(token))
    {
        *opcode = map_opcode(token);
    }
    else
    {
        fprintf(stderr, "%s:%d: error: Expected opcode but got %s.\n", get_scan_file(), get_scan_line(), get_token_name(token));
        result = -1;
    }
}

int is_value_token(unsigned int token)
{
    switch (token)
    {
        case A_TOKEN:
        case B_TOKEN:
        case C_TOKEN:
        case X_TOKEN:
        case Y_TOKEN:
        case Z_TOKEN: 
        case I_TOKEN:
        case J_TOKEN:
        case POP_TOKEN:
        case PEEK_TOKEN:
        case PUSH_TOKEN:
        case SP_TOKEN:
        case PC_TOKEN:
        case O_TOKEN:
        case IDENTIFIER_TOKEN:
        case INTEGER_TOKEN:
            return 1;
        default:
            return 0;
    }
}

int is_symbolic(unsigned int token)
{
    switch (token)
    {
        case A_TOKEN:
        case B_TOKEN:
        case C_TOKEN:
        case X_TOKEN:
        case Y_TOKEN:
        case Z_TOKEN: 
        case I_TOKEN:
        case J_TOKEN:
        case POP_TOKEN:
        case PEEK_TOKEN:
        case PUSH_TOKEN:
        case SP_TOKEN:
        case PC_TOKEN:
        case O_TOKEN:
            return 1;
        default:
            return 0;
    }
}

void read_argument(arg_t* arg)
{
    assert(arg != NULL);
    
    int as_value = 0;
    unsigned int t1 = 0;
    variant_t v1;
    unsigned int t2 = 0;
    
    init_variant(&v1);
    
    read_token();
    
    if (token == LEFT_BRAKET_TOKEN)
    {
        as_value = 1;
        read_token();
    }
    else
    {
        as_value = 0;
    }
    
    if (is_value_token(token))
    {
        t1 = token;
        copy_variant(&v1, &value);
    }
    else
    {
        fprintf(stderr, "%s:%d: error: Expected identifier or integer but got %s.\n", get_scan_file(), get_scan_line(), get_token_name(token));
        result = -1;
        return;
    }
    
    if (next_token == PLUS_TOKEN)
    {
        read_token();
        assert(token == PLUS_TOKEN);
        
        read_token();
        
        if (is_value_token(token))
        {
            t2 = token;
        }
        else
        {
            fprintf(stderr, "%s:%d: error: Expected identifier or integer but got %s.\n", get_scan_file(), get_scan_line(), get_token_name(token));
            result = -1;
            return;
        }
    }
        
    if (as_value == 1)
    {
        read_token();
        if (token != RIGHT_BRAKET_TOKEN)
        {
            fprintf(stderr, "%s:%d: error: Missmatched braket.\n", get_scan_file(), get_scan_line());
            result = -1;
            return;
        }
    }
    /*
    0x00-0x07: register (A, B, C, X, Y, Z, I or J, in that order)
    0x08-0x0f: [register]
    0x10-0x17: [next word + register]
         0x18: POP / [SP++]
         0x19: PEEK / [SP]
         0x1a: PUSH / [--SP]
         0x1b: SP
         0x1c: PC
         0x1d: O
         0x1e: [next word]
         0x1f: next word (literal)
    0x20-0x3f: literal value 0x00-0x1f (literal)
    */
    
    if (is_symbolic(t1))
    {
        switch (t1)
        {
            case A_TOKEN:
                if (as_value)
                    arg->value = 0x08;
                else
                    arg->value = 0x00;
                break;
            case B_TOKEN:
                if (as_value)
                    arg->value = 0x09;
                else
                    arg->value = 0x01;
                break;
            case C_TOKEN:
                if (as_value)
                    arg->value = 0x0A;
                else
                    arg->value = 0x02;
                break;
            case X_TOKEN:
                if (as_value)
                    arg->value = 0x0B;
                else
                    arg->value = 0x03;
                break;
            case Y_TOKEN:
                if (as_value)
                    arg->value = 0x0C;
                else
                    arg->value = 0x04;
                break;
            case Z_TOKEN:
                if (as_value)
                    arg->value = 0x0D;
                else
                    arg->value = 0x05;
                break;
            case I_TOKEN:
                if (as_value)
                    arg->value = 0x0E;
                else
                    arg->value = 0x06;
                break;    
            case J_TOKEN:
                if (as_value)
                    arg->value = 0x0F;
                else
                    arg->value = 0x07;
                break;
            case POP_TOKEN:
                arg->value = 0x18;
                break;
            case PEEK_TOKEN:
                arg->value = 0x19;
                break;
            case PUSH_TOKEN:
                arg->value = 0x1A;
                break;
            case SP_TOKEN:
                arg->value = 0x1B;
                break;
            case PC_TOKEN:
                arg->value = 0x1C;
                break;
            case O_TOKEN:
                arg->value = 0x1D;
                break;
        }
        /* TODO validate that there is no offset */
    }
    else if (t1 == IDENTIFIER_TOKEN)
    {
        // for jumps we encode alwas next word
        // this is since we can't resolve the target at this point
        arg->value = 0x1F;
        copy_variant(&arg->extra, &v1);
        
        /* TODO validate that there is no offset */
    }
    else
    {
        assert(t1 == INTEGER_TOKEN);
        assert(v1.type == UINT_TYPE);
        
        if (as_value && t2 == 0)
        {
            arg->value = 0x1E;
            copy_variant(&arg->extra, &v1);
        }
        if (!as_value && t2 == 0)
        {
            if (v1.ui <= 0x1F)
            {
                arg->value = 0x20 + v1.ui;
            }
            else
            {
                arg->value =  0x1F;
                copy_variant(&arg->extra, &v1);
            }            
        }
        if (t2 != 0)
        {
            assert(as_value == 1);
            switch (t2)
            {
                case A_TOKEN:
                    arg->value = 0x10;
                    break;
                case B_TOKEN:
                    arg->value = 0x11;
                    break;
                case C_TOKEN:
                    arg->value = 0x12;
                    break;
                case X_TOKEN:
                    arg->value = 0x13;
                    break;
                case Y_TOKEN:
                    arg->value = 0x14;
                    break;
                case Z_TOKEN:
                    arg->value = 0x15;
                    break;
                case I_TOKEN:
                    arg->value = 0x16;
                    break;
                case J_TOKEN:
                    arg->value = 0x17;
                    break;      
                default:
                    assert(0 && "should never happen");
                    break;
            }
            copy_variant(&arg->extra, &v1);
        }
    }    
}

/* error recovery: read until EOL or EOF */
void recover_error()
{    
    while (token != EOL_TOKEN && token != EOF_TOKEN)
    {
        read_token();
    }
}

void read_instruction()
{
    instruction_t instr = {0};
    
    // skip empty lines
    if (next_token == EOL_TOKEN || next_token == EOF_TOKEN)
    {
        read_token();
        return;
    }
    
    if (next_token == LABEL_TOKEN)
    {
        read_label(&instr.label);
    }
    else
    {
        instr.label = 0;
    }
    
    read_opcode(&instr.opcode);
    
    if (next_token != EOL_TOKEN)
    {
        read_argument(&instr.arg1);
    }
    
    if (next_token == COMMA_TOKEN)
    {
    
        read_token();
        assert(token == COMMA_TOKEN);
        
        read_argument(&instr.arg2);    
    }
    
    read_token();
    if (token != EOL_TOKEN)
    {
        fprintf(stderr, "%s:%d: error: To much input.\n", get_scan_file(), get_scan_line());
        result = -1;  

        recover_error();
        return;
    }
    
    add_instruction(prog, instr);
}

void read_program()
{
    while (token != EOF_TOKEN)
    {
        read_instruction();
    }
}

int parse(const char* file, program_t* p)
{
    int r = 0;
    FILE* fp = NULL;
    prog = p;
    prog->file = strdup(file);
    
    assert(prog != NULL);
    
    if (file != NULL)
    {        
        fp = fopen(file, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Failed to open %s\n", file);
            return -1;
        }   
    }
    else
    {
        fp = stdin;
        file = "<stdin>";
    }
    
    r = start_scan(fp, file);
    if (r < 0)
    {
        return r;
    }
    
    init_variant(&value);
    init_variant(&next_value);
    next_token = scan_token(&next_value);
    
    read_program();
    
    return result;
}

int init_program(program_t* prog)
{
    assert(prog != NULL);
    
    prog->file = 0;
    prog->code = NULL;
    prog->code_lines = 0;
    prog->code_size = 0;
    
    return 0;
}

/**
 * Clear program memory.
 **/
int clear_program(program_t* prog)
{
    assert(prog != NULL);

    free(prog->file);
    prog->file = NULL;
    free(prog->code);
    prog->code = NULL;
    prog->code_lines = 0;
    prog->code_size = 0;
    
    return 0;
}
