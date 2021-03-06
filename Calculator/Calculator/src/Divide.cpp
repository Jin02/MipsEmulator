#include "Divide.h"
#include "System.h"
#include "DumpLogManager.h"

/** Divide **/

Divide::Divide(unsigned int rs, unsigned int rt, unsigned int rd)
    : RFormatInstruction(rs, rt, rd), _executionHiResult(0), _executionLoResult(0)
{
	GlobalDumpManagerAddLogClassName(Divide);
}

Divide::~Divide(void)
{

}

unsigned int Divide::Instruct(unsigned int rsData, unsigned int rtData)
{
	return 0;
}

void Divide::Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst)
{
	Forwarding(prev2stepInst, prev1stepInst, _rsData, _rs);
	Forwarding(prev2stepInst, prev1stepInst, _rtData, _rt);

	int rsData = _rsData;
	int rtData = _rtData;

	_executionLoResult = (rsData / rtData);
	_executionHiResult = (rsData % rtData);
	{
		char buff[256] = {0, };	
		sprintf(buff, "ExecutionHiData = 0x%x / ExecutionLoData = 0x%x", _executionHiResult, _executionLoResult);
		GlobalDumpLogManager->AddLog(buff, true);
	}
}

void Divide::WriteBack()
{
	System* system = System::GetInstance();

	system->SetHiRegister( _executionHiResult );
	system->SetLoRegister( _executionLoResult );
	{
		char buff[256] = {0, };	
		sprintf(buff, "Hi = 0x%x / Lo = 0x%x", _executionHiResult, _executionLoResult);
		GlobalDumpLogManager->AddLog(buff, true);
	}
}

/** Divide Unsigned **/

DivideUnsigned::DivideUnsigned(unsigned int rs, unsigned int rt, unsigned int rd)
    : RFormatInstruction(rs, rt, rd), _executionHiResult(0), _executionLoResult(0)
{
	GlobalDumpManagerAddLogClassName(DivideUnsigned);
}

DivideUnsigned::~DivideUnsigned(void)
{

}

unsigned int DivideUnsigned::Instruct(unsigned int rsData, unsigned int rtData)
{
    return 0;
}

void DivideUnsigned::Execution(const Instruction* prev2stepInst, const Instruction* prev1stepInst)
{
	Forwarding(prev2stepInst, prev1stepInst, _rsData, _rs);
	Forwarding(prev2stepInst, prev1stepInst, _rtData, _rt);

	_executionLoResult = (_rsData / _rtData);
	_executionHiResult = (_rsData % _rtData);
	{
		char buff[256] = {0, };	
		sprintf(buff, "ExecutionHiData = 0x%x / ExecutionLoData = 0x%x", _executionHiResult, _executionLoResult);
		GlobalDumpLogManager->AddLog(buff, true);
	}
}

void DivideUnsigned::WriteBack()
{
	System* system = System::GetInstance();

	system->SetHiRegister( _executionHiResult );
	system->SetLoRegister( _executionLoResult );
	{
		char buff[256] = {0, };	
		sprintf(buff, "Hi = 0x%x / Lo = 0x%x", _executionHiResult, _executionLoResult);
		GlobalDumpLogManager->AddLog(buff, true);
	}
}