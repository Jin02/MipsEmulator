#include "And.h"
#include "System.h"

/** Add Unsigned **/

And::And(unsigned int rs, unsigned int rt, unsigned int rd)
    : RFormatInstruction(rs, rt, rd)
{
    
}

And::~And(void)
{
    
}

unsigned int And::Instruct(unsigned int rs, unsigned int rt)
{
    return rs & rt;
}