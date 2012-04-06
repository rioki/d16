
#ifndef _D16_GENERATE_H_
#define _D16_GENERATE_H_

#include "parse.h"
#include <stdio.h>

/**
 * Generate D16 code.
 *
 * @param file the file to writ to, if NULL stdout will be used
 * @param prog the program to generate
 **/
int generate(const char* file, program_t* prog);

#endif
