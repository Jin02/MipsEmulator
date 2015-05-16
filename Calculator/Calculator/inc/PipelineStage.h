#pragma once

#include "Common.h"
#include "Instruction.h"

class PipelineStage
{
public:
    enum class State : uint
    {
        Fetch, Decode, Execution, Memory, WriteBack, Stall, Num = Stall
    };
    
private:
    State           _state;
    uint            _pc;

    Instruction*    _instruction;    
    uint            _instructionValue;

	PipelineStage*	_prev1StepPip;
	PipelineStage*	_prev2StepPip;

	bool			_isDummyStall;

public:
    PipelineStage(bool dummyStall = false);
    ~PipelineStage();
    
public:
    void NextState();
    void Cancel();
    void Clear();
    
private:
    uint            Fetch();
    void            Decode(uint instValue);
    void            Execution(const PipelineStage* prev2step, const PipelineStage* prev1step);
    void            Memory();
    void            WriteBack();
    
public:
    void            RunStage();
    
public:
    GET_ACCESSOR(State, State, _state);
    GET_ACCESSOR(Instruction, Instruction*, _instruction);
	GET_ACCESSOR(ProgramCounter,  uint, _pc);

	SET_ACCESSOR(Prev2StepPip, PipelineStage*, _prev2StepPip);
	SET_ACCESSOR(Prev1StepPip, PipelineStage*, _prev1StepPip);

	GET_ACCESSOR(IsDummyStall, bool, _isDummyStall);
};