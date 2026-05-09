#include <stdio.h>
#include <assert.h>

#include "../include/backendConsts.h"
#include "../include/instructionsEncoding.h"

int emitByte(programBuf, uint8_t byte) {
    assert(programBuf);
                                                        //FIXME
    if (buf->size < buf->capacity) {
        buf->code[buf->size++] = byte;
    } else {
    }
}

void emitRex64(backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg, regCode_t indexReg) {
    assert(cntxt);

    uint8_t w = 1;
    uint8_t r = (srcReg >= R8)  ? 1 : 0;
    uint8_t x = (indexReg >= R8 && indexReg != NO_REG) ? 1 : 0;
    uint8_t b = (destReg >= R8) ? 1 : 0;

    uint8_t rex = 0x40 | (w << 3) | (r << 2) | (x << 1) | b;
    emitByte(cntxt, rex);
}

void emitModRM(backendContext_t* cntxt, uint8_t mod, uint8_t reg, uint8_t rm) {
    assert(cntxt);

    uint8_t modrm = (mod << 6) | ((reg & 7) << 3) | (rm & 7);
    emitByte(cntxt, modrm);
}

int emitSIB (backendContext_t* cntxt, uint8_t scale, regCode_t indexReg, regCode_t baseReg) {
    assert(cntxt);

    uint8_t index = (uint8_t)indexReg;
    uint8_t base  = (uint8_t)baseReg;

    uint8_t SIB = (scale << 6) | (index << 3) | base;
    emit_byte(buf, SIB);
}

int emit_32CurPos (backendContext_t* cntxt, int32_t value) {
    assert(cntxt);
                                                                    //FIXME
    *(int32_t*)(*cntxtProgramBuf(cntxt) + *cntxtProgramBufSize(cntxt)) = value;
    *cntxtProgramBufSize(cntxt) += sizeof(int32_t);
}

int emit_32givenPos (backendContext_t* cntxt, size_t givenBufPos, uint32_t value) {
    assert(cntxt);
                                                                    //FIXME
    *(int32_t*)(*cntxtProgramBuf(cntxt) + givenBufPos) = value;
    *cntxtProgramBufSize(cntxt) = (givenBufPos + sizeof(int32_t) > *cntxtProgramBufSize(cntxt))?
                                   givenBufPos + sizeof(int32_t) :  *cntxtProgramBufSize(cntxt);
}


int emitRet (backendContext_t* cntxt) {
    assert(cntxt);
    emit_byte(buf, opCodeRET);
}

int emitCqo(backendContext_t* cntxt) {
    assert(backendContext_t* cntxt)

    emitByte(cntxt, REX_W_BYTE);
    emitByte(cntxt, opCodeCQO);
}

int emitPushReg(backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    emitByte(cntxt, opCodePUSH + regCode);
}

int emitPopReg(backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    emitByte(cntxt, opCodePOP + regCode);
}

int emitOpRegReg(backendContext_t* cntxt, opCode_t opCode, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    if (opCode == IMUL) {
        emitRex64(cntxt, destReg, srcReg, NO_REG);
        emitByte(cntxt, ESCAPE_PREFIX);
        emitByte(cntxt, opCodeIMUL);
        emitModRM(cntxt, REG_REG, destReg, srcReg);
    }
    else {
        emitRex64(cntxt, srcReg, destReg, NO_REG);
        emitByte(cntxt, (uint8_t)opCode);
        emitModRM(cntxt, REG_REG, srcReg, destReg);
    }
    return BACKEND_SUCCESS;
}

int emitIdiv (backendContext_t* cntxt, regCode_t srcReg) {
    assert(cntxt);

    emitRex64(cntxt, NO_REG, srcReg, NO_REG);
    emitByte(cntxt, opCodeIDIV);
    emitModRM(cntxt, REG_REG, IDIV_EXTRA_OPCODE, srcReg);
}

int emitMov (backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, int64_t disp) {
    assert(cntxt);

    bool direction = (modARGS == REG_MEM);

    uint8_t opCode = direction ? opCodeMOVsetDir : opCodeMOVcleanDir;
    opCode = (modARGS == REG_CONST) ? opCodeMOVregConst + (destReg & 7): opCode;

    regCode_t valueReg = direction ? destReg : srcReg;
    regCode_t baseReg  = direction ? srcReg  : destReg;

    emitRex64(cntxt, baseReg, valueReg, NO_REG);
    emitByte(cntxt, opCode);

    if (modARGS == REG_CONST) {
        emit_u64(cntxt, (uint64_t)disp);
        return BACKEND_SUCCESS;
    }

    if (modARGS == REG_REG) {
        emitModRM(cntxt, REG_REG, valueReg, baseReg);
        return BACKEND_SUCCESS;
    }

    uint8_t mod = (!disp && (baseReg & 7) != RBP) ? MEM_NO_OFFSET: MEM_8_OFFSET;
    mod = (disp >= -128 && disp <= 127) ? mod: MEM_32_OFFSET;

    emitModRM(cntxt, mod, valueReg, baseReg);

    if ((baseReg & 7) == RSP)
        emitSIB(cntxt, 0x00, NO_INDEX_REG, RSP);

    if (mod == MEM_8_OFFSET)
        emitByte(cntxt, (uint8_t)disp);
    else if (mod == MEM_32_OFFSET)
        emit_32(cntxt, (int32_t)disp);

    return BACKEND_SUCCESS;
}

int emitJMPorCALL (backendContext_t* cntxt, bool isCnd, opCode_t opCode, uint64_t labelAddr) {
    assert(cntxt);

    if (isCnd)
        emit(cntxt, ESCAPE_PREFIX);

    emitByte(cntxt, opCode);
    int32_t offset = (int32_t)(labelAddr - *cntxtBufSize(cntxt));

    emit_32(cntxt, offset);
}

int emitSETcc (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg) {
    assert(cntxt);

    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCode);

    emitModRM(cntxt, REG_REG, 0x00, destReg);
}

int emitMovzxRR8(backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    emitRex64(cntxt, destReg, srcReg, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);

    emitByte(cntxt, opCodeMOVZXrr8);

    emitModRM(cntxt, REG_REG, destReg, srcReg);

    return BACKEND_SUCCESS;
}

int emitAluRegConst(backendContext_t* cntxt, regCode_t regCode, int32_t imm, aluOpCode_t aluOpCode) {
    assert(cntxt);

    emitRex64(cntxt, NO_REG, regCode, NO_REG);

    if (imm >= -128 && imm <= 127) {
        emitByte(cntxt, 0x83);
        emitModRM(cntxt, REG_REG, (uint8_t)aluOpCode, regCode);
        emitByte(cntxt, (uint8_t)imm);
    }
    else {
        emitByte(cntxt, 0x81);
        emitModRM(cntxt, REG_REG, (uint8_t)aluOpCode, regCode);
        emit_32CurPos(cntxt, (int32_t)imm);
    }

    return BACKEND_SUCCESS;
}
