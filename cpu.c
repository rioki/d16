
#include <assert.h>

#include "cpu.h"

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
    assert(opcode < 0x11);
    return opcode_names[opcode];
}
