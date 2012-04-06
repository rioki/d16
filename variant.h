
#ifndef _D16_VARIANT_H_
#define _D16_VARIANT_H_

typedef enum 
{
    NO_TYPE,
    INT_TYPE,
    UINT_TYPE,
    STRING_TYPE
} type_t;

typedef struct 
{
    type_t type;
    union
    {
        int i;
        unsigned int ui;
        char* str;
    };    
} variant_t;

/**
 * Initialize a variant_t.
 **/
void init_variant(variant_t* v);

/**
 * Clear a variant and any assiciated data.
 **/
void clear_variant(variant_t* v);

/**
 * Create a deep copy of the variant.
 **/
int copy_variant(variant_t* dest, const variant_t* src);

#endif
