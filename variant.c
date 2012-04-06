
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "variant.h"

void init_variant(variant_t* v)
{
    assert(v != NULL);
    v->type = NO_TYPE;
}

void clear_variant(variant_t* v)
{
    assert(v != NULL);
    if (v->type == STRING_TYPE)
    {
        free(v->str);
    }
	v->type = NO_TYPE;
}

int copy_variant(variant_t* dest, const variant_t* src)
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(dest != src);
    
    *dest = *src;
    if (src->type == STRING_TYPE)
    {
        dest->str = strdup(src->str);
    }
    return 0;
}
