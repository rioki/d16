
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
	A_VALUE = 0x0, 
	B_VALUE = 0x1,  
	C_VALUE = 0x2, 
	X_VALUE = 0x3, 
	Y_VALUE = 0x4, 
	Z_VALUE = 0x5, 
	I_VALUE = 0x6,
	J_VALUE = 0x7,
	/*	Whatever that means:
		0x08-0x0f: [register]
		0x10-0x17: [next word + register]
	*/
	POP_VALUE = 0x18,	
    PEEK_VALUE = 0x19,
    PUSH_VALUE = 0x1a,
    SP_VALUE = 0x1b,
    PC_VALUE = 0x1c,
    O_VALUE = 0x1d
	/*
		Whatever that means:
		0x1e: [next word]
        0x1f: next word (literal)
	 */
	/* 0x20-0x3f literal values (0x00-0x1f) */
} value_t;

const char* get_opcode_name(int opcode);

#endif
