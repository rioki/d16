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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

const char* usage = 
    "Usage: \n" 
    "  dcpu16 <program.d16> [options]\n" 
    "\n"
    "Options:\n"
    "  no options yet";

int read_program(unsigned short** program, unsigned int* size, const char* file)
{
    assert(program != NULL);
    assert(size != NULL);
    assert(file != NULL);

    FILE* fp = NULL;
    size_t fsize = 0;
    unsigned char* buff = NULL;
    size_t read = 0;
    
    fp = fopen(file, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s for reading.", file);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);    
    fseek(fp, 0, SEEK_SET);
    
    buff = (unsigned char*)malloc(fsize);
    
    read = fread(buff, fsize, 1, fp);        
    
    if (read != 1)
    {
        fprintf(stderr, "Failed to read file %s.", file);
        fclose(fp);
        return -1;
    }
    
    fclose(fp);
    
    
    *size = fsize / 2;
    *program = (unsigned short*)buff;
    
    return 0;
}
    
int main(int argc, char* argv[])
{
    unsigned short* program = NULL;
    unsigned int size = 0;
    
    if (argc < 2)
    {
        fputs(usage, stderr);
        return 0;
    }
    
    int r = read_program(&program, &size, argv[1]);
    if (r < 0)
    {
        return r;
    }
    
    return execute(program, size);
}
