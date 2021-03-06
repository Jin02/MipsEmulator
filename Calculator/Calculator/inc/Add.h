#pragma once

#include "RFormatInstruction.h"

class AddUnsigned : public RFormatInstruction
{
private:
    
public:
    AddUnsigned(unsigned int rs, unsigned int rt, unsigned int rd);
    virtual ~AddUnsigned(void);
    
public:
    virtual unsigned int Instruct(unsigned int rsData, unsigned int rtData);
};

class Add : public AddUnsigned
{
public:
	Add(unsigned int rs, unsigned int rt, unsigned int rd);
	virtual ~Add();
};