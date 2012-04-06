
#ifndef _SCAN_H_
#define _SCAN_H_

#include "variant.h"

/**
 * Tokens
 **/
enum tokens
{
    ERROR_TOKEN,
    COMMA_TOKEN,
    LEFT_BRAKET_TOKEN,
    RIGHT_BRAKET_TOKEN,
    PLUS_TOKEN,    
    SET_TOKEN,
    ADD_TOKEN,
    SUB_TOKEN,
    MUL_TOKEN, 
    DIV_TOKEN,
    MOD_TOKEN, 
    SHL_TOKEN, 
    SHR_TOKEN, 
    AND_TOKEN, 
    BOR_TOKEN, 
    XOR_TOKEN, 
    IFE_TOKEN, 
    IFN_TOKEN, 
    IFG_TOKEN, 
    IFB_TOKEN,
    JSR_TOKEN,
	A_TOKEN,
	B_TOKEN,
	C_TOKEN,
	X_TOKEN,
	Y_TOKEN,
	Z_TOKEN, 
	I_TOKEN,
	J_TOKEN,
	POP_TOKEN,
    PEEK_TOKEN,
    PUSH_TOKEN,
    SP_TOKEN,
    PC_TOKEN,
    O_TOKEN,
    IDENTIFIER_TOKEN,    
    LABEL_TOKEN,
    INTEGER_TOKEN,
    EOL_TOKEN,
    EOF_TOKEN
};

/**
 * Start the scanner.
 *
 * @param fp a file pointer to an open file
 * @param file the name of the file that is scanned
 **/
int start_scan(FILE* fp, const char* file);

/**
 * Scan a single token.
 **/
int scan_token(variant_t* value);

/**
 * Get the current file the scanner is on.
 **/
const char* get_scan_file();

/**
 * Get the current line the scanner is on.
 **/
unsigned int get_scan_line();

/**
 * Get the name of the token.
 **/
const char* get_token_name(int token);

#endif
