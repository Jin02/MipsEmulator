//#pragma once
//
//#include "Instruction.h"
//
//class MoveFromHi : public Instruction
//{
//private:
//    unsigned int _rd, _hiData;
//    
//public:
//    MoveFromHi(unsigned int rd);
//    virtual ~MoveFromHi(void);
//    
//public:
//    virtual void Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst);
//    virtual void Memory(const Instruction* prev2stepInst, const Instruction* prev1stepInst){}
//    virtual void WriteBack();
//
//    virtual void DependencyCheckWithGetTargetData(bool& hasDependency, uint& outTargetData, uint compareRegiIdx) const;
//};
//
//
//class MoveToHi : public Instruction
//{
//private:
//    unsigned int _rs, _rsData;
//    
//public:
//    MoveToHi(unsigned int rs);
//    virtual ~MoveToHi(void);
//    
//public:
//    virtual void Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst);
//    virtual void Memory(const Instruction* prev2stepInst, const Instruction* prev1stepInst){}
//    virtual void WriteBack();
//
//    virtual void DependencyCheckWithGetTargetData(bool& hasDependency, uint& outTargetData, uint compareRegiIdx) const;
//};
//
//
//class MoveFromLo : public Instruction
//{
//private:
//    unsigned int _rd, _loData;
//    
//public:
//    MoveFromLo(unsigned int rd);
//    virtual ~MoveFromLo(void);
//    
//public:
//    virtual void Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst);
//    virtual void Memory(const Instruction* prev2stepInst, const Instruction* prev1stepInst){}
//    virtual void WriteBack();
//
//    virtual void DependencyCheckWithGetTargetData(bool& hasDependency, uint& outTargetData, uint compareRegiIdx) const;
//};
//
//class MoveToLo : public Instruction
//{
//private:
//    unsigned int _rs, _rsData;
//    
//public:
//    MoveToLo(unsigned int rs);
//    virtual ~MoveToLo(void);
//    
//public:
//    virtual void Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst);
//    virtual void Memory(const Instruction* prev2stepInst, const Instruction* prev1stepInst){}    
//    virtual void WriteBack();
//
//    virtual void DependencyCheckWithGetTargetData(bool& hasDependency, uint& outTargetData, uint compareRegiIdx) const;
//};
