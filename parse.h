
#ifndef _D16_PARSE_H_
#define _D16_PARSE_H_

#include "variant.h"

/**
 * Argument
 **/
typedef struct
{
    unsigned char value;
    variant_t extra;
} arg_t;
 
 
/**
 * Instruction
 **/
typedef struct 
{
    unsigned int line;
    char* label;
    int opcode;
    arg_t arg1;    
    arg_t arg2;    
} instruction_t;

/**
 * Program
 **/
typedef struct 
{
    char* file;
    instruction_t* code;
    unsigned int code_lines;
    unsigned int code_size;
} program_t;

/**
 * Parse a d16 file.
 *
 * @param file the file to parse, may be NULL, then STDIN will be used.
 **/
int parse(const char* file, program_t* prog);

/**
 * Initialize a program.
 **/
int init_program(program_t* prog);

/**
 * Clear program memory.
 **/
int clear_program(program_t* prog);

#endif
