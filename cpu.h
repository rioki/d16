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

#ifndef _D16_CPU_H_
#define _D16_CPU_H_

/**
 * Operation Code
 **/
typedef enum 
{
    // basic opcodes	
    SET_OPCODE = 0x1,
    ADD_OPCODE = 0x2,
    SUB_OPCODE = 0x3,
    MUL_OPCODE = 0x4,
    DIV_OPCODE = 0x5,
    MOD_OPCODE = 0x6,
    SHL_OPCODE = 0x7,
    SHR_OPCODE = 0x8,
    AND_OPCODE = 0x9,
    BOR_OPCODE = 0xA,
    XOR_OPCODE = 0xB,
    IFE_OPCODE = 0xC,
    IFN_OPCODE = 0xD,
    IFG_OPCODE = 0xE,
    IFB_OPCODE = 0xF,
    
    // extended opcodes
    JSR_OPCODE = 0x10
} opcode_t;

/**
 * Register
 **/
typedef enum
{	
    A_REGISTER = 0x0, 
	B_REGISTER = 0x1,  
	C_REGISTER = 0x2, 
	X_REGISTER = 0x3, 
	Y_REGISTER = 0x4, 
	Z_REGISTER = 0x5, 
	I_REGISTER = 0x6,
	J_REGISTER = 0x7,
	VALUE_A = 0x8, 
	VALUE_B = 0x9,  
	VALUE_C = 0xA, 
	VALUE_X = 0xB, 
	VALUE_Y = 0xC, 
	VALUE_Z = 0xD, 
	VALUE_I = 0xE,
	VALUE_J = 0xF,
    NEXT_VALUE_A = 0x10,
    NEXT_VALUE_B = 0x11,
    NEXT_VALUE_C = 0x12,
    NEXT_VALUE_X = 0x13,
    NEXT_VALUE_Y = 0x14,
    NEXT_VALUE_Z = 0x15,
    NEXT_VALUE_I = 0x16,
    NEXT_VALUE_J = 0x17,    
	POP_VALUE = 0x18,	
    PEEK_VALUE = 0x19,
    PUSH_VALUE = 0x1a,
    SP_REGISTER = 0x1b,
    PC_REGISTER = 0x1c,
    O_REGISTER = 0x1d,
	NEXT_VALUE = 0x1e,
    NEXT_LITERAL = 0x1f,
	/* 0x20-0x3f literal values (0x00-0x1f) */
} value_t;

int execute(const unsigned short* program, unsigned int size);

const char* get_opcode_name(int opcode);

#endif
