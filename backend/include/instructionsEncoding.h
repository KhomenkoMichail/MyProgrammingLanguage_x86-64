#ifndef INSTRUCTIONS_ENCODING_H
#define INSTRUCTIONS_ENCODING_H

int emitByte (backendContext_t* cntxt, uint8_t byte);

int emitRex64 (backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg, regCode_t indexReg);

int emitModRM(backendContext_t* cntxt, uint8_t mod, uint8_t reg, uint8_t rm);

int emitSIB (backendContext_t* cntxt, uint8_t scale, regCode_t indexReg, regCode_t baseReg);

int emit_32CurPos (backendContext_t* cntxt, int32_t value);

int emit_32givenPos (backendContext_t* cntxt, size_t givenBufPos, uint32_t value);

int emitRet (backendContext_t* cntxt);

int emitCqo(backendContext_t* cntxt);

int emitPushReg(backendContext_t* cntxt, regCode_t regCode);

int emitPopReg(backendContext_t* cntxt, regCode_t regCode);

int emitOpRegReg(backendContext_t* cntxt, opCode_t opCode, regCode_t destReg, regCode_t srcReg);

int emitIdiv (backendContext_t* cntxt, regCode_t srcReg);

int emitMov (backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, int32_t disp);

int emitJMPorCALL (backendContext_t* cntxt, bool isCnd, opCode_t opCode, uint64_t labelAddr);

int emitSETcc (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg);

int emitMovzxRR8(backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg);

int emitAluRegConst(backendContext_t* cntxt, regCode_t regCode, int32_t imm, aluOpCode_t aluOpCode);

int emitBreakpoint (backendContext_t* cntxt, node_t* node);

int emit_64CurPos (backendContext_t* cntxt, int32_t value);

#endif
