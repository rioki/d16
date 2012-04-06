
#include <stdio.h>

#include "parse.h"
#include "generate.h"

int main(int argc, char* argv[])
{
    int result = 0;    
    const char* infile = NULL;
    const char* outfile = NULL;
    
    /* TODO proper argument parsing */
    if (argc > 1)
    {
        infile = argv[1];
    }
    
    if (argc > 2)
    {
        outfile = argv[2];
    }
    
    program_t prog;
    init_program(&prog);
    
    result = parse(infile, &prog);

    if (result >= 0)
    {
        result = generate(outfile, &prog);
    }
	
    return result;
}
