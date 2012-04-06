/*
 * Copyright (C) 2012 Sean "rioki" Farrell
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

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
