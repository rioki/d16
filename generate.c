
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generate.h"
#include "cpu.h"

static program_t* prog = NULL;

#define JUMP_TABLE_MAX 1024
static struct
{
	const char* name;
	unsigned int pos;
} jump_table[JUMP_TABLE_MAX];
static unsigned int jump_table_size = 0;

int generate_jump_table()
{
	assert(prog != NULL);
	unsigned int i;
    unsigned int code_point = 0;
	
	// TODO validate uniqueness of labels
	
	for (i = 0; i < prog->code_lines; i++)
	{
		if (prog->code[i].label != NULL)
		{
			assert(jump_table_size < JUMP_TABLE_MAX);
			jump_table[jump_table_size].name = prog->code[i].label;
			jump_table[jump_table_size].pos = code_point;
			
			jump_table_size++;
		}
        
        code_point += 1;
        
        if (prog->code[i].arg1.extra.type != NO_TYPE)
        {
            code_point += 1;
        }
        if (prog->code[i].arg2.extra.type != NO_TYPE)
        {
            code_point += 1;
        }
	}	

	return 0;
}

int resolve_label(unsigned int* pos, const char* label)
{
    assert(pos != NULL);
    unsigned int i;    
    for (i = 0; i < JUMP_TABLE_MAX; i++)
	{
		if (strcmp(jump_table[i].name, label) == 0)
        {
            *pos = jump_table[i].pos;
            return 0;
        }
	}
    return -1;
}

int is_simple_opcode(int opcode)
{
	switch (opcode)
	{
        case SET_OPCODE:
        case ADD_OPCODE:
        case SUB_OPCODE:
        case MUL_OPCODE:
        case DIV_OPCODE:
        case MOD_OPCODE:
        case SHL_OPCODE:
        case SHR_OPCODE:
        case AND_OPCODE:
        case BOR_OPCODE:
        case XOR_OPCODE:
        case IFE_OPCODE:
        case IFN_OPCODE:
        case IFG_OPCODE:
        case IFB_OPCODE:
            return 1;
        default:
            return 0;    
	}
}

unsigned short swap_eniness(unsigned short x) 
{
	return ((x<<8)|(x>>8));
}

int generate_executable(FILE* fp)
{
	int result = 0;
	unsigned int i;
	
	assert(fp != NULL);
	
	for (i = 0; i < prog->code_lines; i++)
	{
		// instructions can be only 3 words (3 x 2 byte) long
		unsigned short buff[3] = {0, 0, 0};
		unsigned int size = 1;		
        arg_t arg1 = prog->code[i].arg1;
        arg_t arg2 = prog->code[i].arg2;
		
		buff[0] |= prog->code[i].opcode;
		
		/* todo as value */
		if (is_simple_opcode(prog->code[i].opcode))
		{
            assert(arg1.value <= 0x3f);
            assert(arg2.value <= 0x3f);
            
            buff[0] |= arg1.value << 4;
			buff[0] |= arg2.value << 10;
            
            if (arg1.extra.type == UINT_TYPE)
            {    
                buff[size] = arg1.extra.ui;
                size++;
            }
            if (arg1.extra.type == STRING_TYPE)
            {    
                unsigned int pos = 0;
                resolve_label(&pos, arg1.extra.str);
                buff[size] = pos;
                size++;
            }
            
            if (arg2.extra.type == UINT_TYPE)
            {
                buff[size] = arg2.extra.ui;
                size++;
            }
            if (arg2.extra.type == STRING_TYPE)
            {    
                unsigned int pos = 0;
                resolve_label(&pos, arg2.extra.str);
                buff[size] = pos;
                size++;
            }			
		}
		else
		{
            assert(arg1.value <= 0x3f);
            
			buff[0] |= arg1.value << 10;
            
            if (arg1.extra.type == UINT_TYPE)
            {
                /* label */                
                buff[size] = arg1.extra.ui;
                size++;
            }
            if (arg1.extra.type == STRING_TYPE)
            {    
                unsigned int pos = 0;
                resolve_label(&pos, arg1.extra.str);
                buff[size] = pos;
                size++;
            }
		}
		
		buff[0] = swap_eniness(buff[0]);
		buff[1] = swap_eniness(buff[1]);
		buff[2] = swap_eniness(buff[2]);
		
		fwrite(&buff, 2, size, fp);
	}
	
	return result;
}

int generate(const char* file, program_t* p)
{
    int result = 0;
	FILE* fp = NULL;
	
	assert(p != NULL);
	
	prog = p;
	
	result = generate_jump_table();	
	if (result < 0)
	{
		return result;
	}
	
    if (file != NULL)
    {        
        fp = fopen(file, "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Failed to open %s\n", file);
            return -1;
        }   
    }
    else
    {
        fp = stdout;
    }
	
	result = generate_executable(fp);
	
    return result;
}
