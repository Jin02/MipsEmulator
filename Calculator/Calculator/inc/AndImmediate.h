#pragma once

#include "IFormatInstruction.h"

class AndImmediate : public IFormatInstruction
{
private:
    
public:
    AndImmediate(unsigned int rs, unsigned int rt, unsigned int immediate);
    virtual ~AndImmediate(void);
    
public:
    virtual void Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst);
};