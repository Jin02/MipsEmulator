#pragma once

#include "IFormatInstruction.h"

/** LoadByteUnsigned **/
class LoadByteUnsigned : public IFormatInstruction
{
private:
    
public:
    LoadByteUnsigned(unsigned int rs, unsigned int rt, unsigned int immediate);
    virtual ~LoadByteUnsigned(void);
    
public:
    virtual void Execution();
};

/** LoadHalfwordUnsigned **/
class LoadHalfwordUnsigned : public IFormatInstruction
{
private:
    
public:
    LoadHalfwordUnsigned(unsigned int rs, unsigned int rt, unsigned int immediate);
    virtual ~LoadHalfwordUnsigned(void);
    
public:
    virtual void Execution();
};

/** LoadLinked **/
//class LoadLinked : public IFormatInstruction
//{
//private:
//    
//public:
//    LoadLinked(unsigned int rs, unsigned int rt, unsigned int immediate);
//    virtual ~LoadLinked(void);
//    
//public:
//    virtual void Execution();
//};

/** LoadUpperImmediate **/
class LoadUpperImmediate : public IFormatInstruction
{
private:
    
public:
    LoadUpperImmediate(unsigned int rs, unsigned int rt, unsigned int immediate);
    virtual ~LoadUpperImmediate(void);
    
public:
    virtual void Execution();
};


/** LoadWord **/
class LoadWord : public IFormatInstruction
{
private:
    
public:
    LoadWord(unsigned int rs, unsigned int rt, unsigned int immediate);
    virtual ~LoadWord(void);
    
public:
    virtual void Execution();
};

/** LoadImmediate **/
class LoadImmediate : public Instruction
{
private:
    unsigned int _rd, _immediate;
    
public:
    LoadImmediate(unsigned int rd, unsigned int immediate);
    virtual ~LoadImmediate(void);
    
public:
    virtual void Execution();
};
