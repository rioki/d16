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

/* If the CPU is to be a Harvard architechture, uncomment the following degine. */
/*#define HARVARD_ARCH*/ 
 
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <time.h>

/* for sleep */
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>    
#endif

#include "cpu.h"

#define RAM_SIZE 0x10000 
#ifdef HARVARD_ARCH
#define PROG_SIZE 0x10000
#else
#define PROG_SIZE RAM_SIZE
#endif

/* reigsters */
static unsigned short A, B, C, X, Y, Z, I, J;
/* special registers */
static unsigned short PC, SP, O;
/* ram */
static unsigned short RAM[RAM_SIZE];
/* program code */
#ifdef HARVARD_ARCH
static unsigned short PROG[PROG_SIZE];
#else
static unsigned short* PROG = RAM;
#endif


/* the time to simulate one "cylcle" */
static unsigned int cycle_time = 100; 

unsigned short swap_eniness(unsigned short x) 
{
    return ((x<<8)|(x>>8));
}

void sleep(unsigned ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

unsigned short* get_ram_pointer(unsigned short adr)
{
    if (adr > 0xFFFF)
    {
        fprintf(stderr, "CPU TRAP: Access to RAM 0x%04X is out of bounds at instruction 0x%04X.", adr, PC);
        return NULL;
    }
    return &RAM[adr];
}

unsigned short get_next_word()
{
    unsigned short value = PROG[PC];
    PC++;
    PC = PC % PROG_SIZE;
    return swap_eniness(value);
    // return value;
}

unsigned short peek_next_word()
{
    unsigned short value = PROG[PC];
    return swap_eniness(value);
}

unsigned short* decode_arg(unsigned short arg, unsigned short* literal)
{
    unsigned short value = 0;
    switch (arg)
    {
        case A_REGISTER:
            return &A;
        case B_REGISTER:
            return &B;
        case C_REGISTER:
            return &C;
        case X_REGISTER:
            return &X;
        case Y_REGISTER:
            return &Y;
        case Z_REGISTER:
            return &Z;
        case I_REGISTER:
            return &I;
        case J_REGISTER:
            return &Z;    
        case VALUE_A:
            return get_ram_pointer(A);
        case VALUE_B:
            return get_ram_pointer(B);
        case VALUE_C:
            return get_ram_pointer(C);
        case VALUE_X:
            return get_ram_pointer(X);
        case VALUE_Y:
            return get_ram_pointer(Y);
        case VALUE_Z:
            return get_ram_pointer(Z);
        case VALUE_I:
            return get_ram_pointer(I);
        case VALUE_J:
            return get_ram_pointer(J);
        case NEXT_VALUE_A:       
            value = get_next_word();
            return get_ram_pointer(value + A);
        case NEXT_VALUE_B:
            value = get_next_word();
            return get_ram_pointer(value + B);
        case NEXT_VALUE_C:
            value = get_next_word();
            return get_ram_pointer(value + C);
        case NEXT_VALUE_X:
            value = get_next_word();
            return get_ram_pointer(value + X);
        case NEXT_VALUE_Y:
            value = get_next_word();
            return get_ram_pointer(value + Y);
        case NEXT_VALUE_Z:
            value = get_next_word();
            return get_ram_pointer(value + Z);
        case NEXT_VALUE_I:
            value = get_next_word();
            return get_ram_pointer(value + I);
        case NEXT_VALUE_J:
            value = get_next_word();
            return get_ram_pointer(value + J);
        case POP_VALUE:
            return get_ram_pointer(SP++);
        case PEEK_VALUE:
            return get_ram_pointer(SP);
        case PUSH_VALUE:
            return get_ram_pointer(--SP);
        case SP_REGISTER:
            return &SP;
        case PC_REGISTER:
            return &PC;
        case O_REGISTER:
            return &O;
        case NEXT_VALUE:
            value = get_next_word();
            return get_ram_pointer(value);
        case NEXT_LITERAL:
            if (literal != NULL)
            {
                *literal = get_next_word();
                return literal;
            }
            else
            {
                fprintf(stderr, "CPU TRAP: Literal value where none should be @0x%04X.", PC);
                return NULL;
            }
        default:
            if (literal != NULL)
            {
                *literal = arg - 0x20;
                return literal;
            }
            else
            {
                fprintf(stderr, "CPU TRAP: Literal value where none should be @0x%04X.", PC);
                return NULL;
            }
    }
}

void skip_next()
{
    unsigned short word = get_next_word();
    unsigned short opcode = word & 0xF;
    unsigned short arg1 = (word & 0x3F0) >> 4;
    unsigned short arg2 = (word & 0xFC00) >> 10;    
    
    if (opcode != 0x0)
    {
        unsigned short a_lit = 0;
        unsigned short b_lit = 0;
        
        decode_arg(arg1, &a_lit);
        decode_arg(arg2, &b_lit);
    }
    else
    {
        unsigned short a_lit = 0;
               
        decode_arg(arg2, &a_lit);
    }
}

int op_set(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest = *value;
    return 0;
}

int op_add(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest += *value;
    return 0;
}

int op_sub(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest -= *value;
    return 0;
}

int op_mul(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest *= *value;
    return 0;
}

int op_div(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest /= *value;
    return 0;
}

int op_mod(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest %= *value;
    return 0;
}      

int op_shl(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest <<= *value;
    return 0;
}      

int op_shr(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest >>= *value;
    return 0;
}

int op_and(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest &= *value;
    return 0;
}

int op_bor(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest |= *value;
    return 0;
}

int op_xor(unsigned short arg1, unsigned short arg2)
{
    unsigned short* dest = NULL;
    unsigned short* value = NULL;
    unsigned short value_lit = 0;
    
    dest = decode_arg(arg1, NULL);
    value = decode_arg(arg2, &value_lit);
    
    if (dest == NULL || value == NULL)
    {
        return -1;
    }
    
    *dest ^= *value;
    return 0;
}

int op_ife(unsigned short arg1, unsigned short arg2)
{
    unsigned short* a = NULL;
    unsigned short* b = NULL;
    unsigned short a_lit = 0;
    unsigned short b_lit = 0;
    
    a = decode_arg(arg1, &a_lit);
    b = decode_arg(arg2, &b_lit);
    
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    
    if (*a == *b)
    {
        return 0;
    }
    else
    {
        skip_next();
        return 0;
    }    
}

int op_ifn(unsigned short arg1, unsigned short arg2)
{
    unsigned short* a = NULL;
    unsigned short* b = NULL;
    unsigned short a_lit = 0;
    unsigned short b_lit = 0;
    
    a = decode_arg(arg1, &a_lit);
    b = decode_arg(arg2, &b_lit);
    
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    
    if (*a != *b)
    {
        return 0;
    }
    else
    {
        skip_next();
        return 0;
    }
}

int op_ifg(unsigned short arg1, unsigned short arg2)
{
    unsigned short* a = NULL;
    unsigned short* b = NULL;
    unsigned short a_lit = 0;
    unsigned short b_lit = 0;
    
    a = decode_arg(arg1, &a_lit);
    b = decode_arg(arg2, &b_lit);
    
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    
    if (*a > *b)
    {
        return 0;
    }
    else
    {
        skip_next();
        return 0;
    }
}

int op_ifb(unsigned short arg1, unsigned short arg2)
{
    unsigned short* a = NULL;
    unsigned short* b = NULL;
    unsigned short a_lit = 0;
    unsigned short b_lit = 0;
    
    a = decode_arg(arg1, &a_lit);
    b = decode_arg(arg2, &b_lit);
    
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    
    if ((*a & *b) == 0)
    {
        return 0;
    }
    else
    {
        skip_next();
        return 0;
    }
}

int op_jsr(unsigned short arg1)
{
    unsigned short* a = NULL;
    unsigned short a_lit = 0;
    unsigned short* s = NULL;
    
    a = decode_arg(arg1, &a_lit);
    s = get_ram_pointer(--SP);
    
    if (a == NULL || a == NULL)
    {
        return -1;
    }
    
    *s = PC;
    PC = *a;
    return 0;
}

int step_cpu()
{
    int state = 0;
    unsigned short word = get_next_word();
    unsigned short opcode = word & 0xF;
    unsigned short arg1 = (word & 0x3F0) >> 4;
    unsigned short arg2 = (word & 0xFC00) >> 10;
    
    switch (opcode)
    {
        case SET_OPCODE:
            state = op_set(arg1, arg2);
            break;
        case ADD_OPCODE:
            state = op_add(arg1, arg2);
            break;
        case SUB_OPCODE:
            state = op_sub(arg1, arg2);
            break;
        case MUL_OPCODE:
            state = op_mul(arg1, arg2);
            break;
        case DIV_OPCODE:
            state = op_div(arg1, arg2);
            break;
        case MOD_OPCODE:
            state = op_mod(arg1, arg2);
            break;
        case SHL_OPCODE:
            state = op_shl(arg1, arg2);
            break;
        case SHR_OPCODE:
            state = op_shr(arg1, arg2);
            break;
        case AND_OPCODE:
            state = op_and(arg1, arg2);
            break;
        case BOR_OPCODE:
            state = op_bor(arg1, arg2);
            break;
        case XOR_OPCODE:
            state = op_xor(arg1, arg2);
            break;
        case IFE_OPCODE:
            state = op_ife(arg1, arg2);
            break;
        case IFN_OPCODE:
            state = op_ifn(arg1, arg2);
            break;
        case IFG_OPCODE:
            state = op_ifg(arg1, arg2);
            break;
        case IFB_OPCODE:
            state = op_ifb(arg1, arg2);
            break;
        case 0x0:
        {
            opcode = word & 0x3FF;
            arg1 = (word & 0xFC00) >> 10;
            switch (opcode)
            {
                case 0x0:
                    /* ok this is not defined, but we will handle it as NOOP */
                    break;
                case JSR_OPCODE:
                    state = op_jsr(arg1);
                    break;
                default:
                    state = -1;
                    fprintf(stderr, "CPU TRAP: Unknown instruction @0x%04X\n", word);
                    break;
            }
            break;               
        }
        default:
            assert(0 && "never happens");
            break;
    }
    
    return state;
}

void print_cpu_state()
{
    printf(" A: %04X  B: %04X C: %04X\n"
           " X: %04X  Y: %04X Z: %04X\n"
           " I: %04X  I: %04X\n"
           "PC: %04X SP: %04X O: %04X\n\n", A, B, C, X, Y, Z, I, J, PC, SP, O);
    
    unsigned short word = peek_next_word();
    unsigned short opcode = word & 0xF;
    unsigned short arg1 = (word & 0x3F0) >> 4;
    unsigned short arg2 = (word & 0xFC00) >> 10;
    
    if (opcode != 0x0)
    {
        printf("Next: %s %04X, %04X (%04X)\n\n", get_opcode_name(opcode), arg1, arg2, word);
    }
    else
    {
        opcode = word & 0x3FF;
        arg1 = (word & 0xFC00) >> 10;
        
        printf("Next: %s %04X (%04X)\n\n", get_opcode_name(opcode), arg1, word);
    }    
}

int execute(const unsigned short* program, unsigned int size)
{
    A = 0;
    B = 0;
    C = 0;
    X = 0;
    Y = 0;
    Z = 0;
    I = 0;
    J = 0;
    PC = 0;
    SP = 0xFFFF;
    O = 0;
    memset(RAM, 0, RAM_SIZE*2);
    memset(PROG, 0, PROG_SIZE*2);
    
    /* NOTE: size is in Bytes, PROG_SIZE is in words. */
    assert(program != NULL);
    if (size > PROG_SIZE)
    {
        fprintf(stderr, "Program exedes program memory.");
        return -1;
    }    
    memcpy(PROG, program, size * 2);
    
    int state = 0;
    while (state == 0)
    {
        print_cpu_state();
        state = step_cpu();        
        sleep(cycle_time);
    }    
    return state;
}

static const char* opcode_names[] = {
    "", // reseved 0x0
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
    
    "JSR"
};
    
const char* get_opcode_name(int opcode)
{
    if (opcode < 0x11)
    {
        return opcode_names[opcode];
    }
    else
    {
        return opcode_names[0];
    }
}
