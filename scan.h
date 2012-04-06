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
