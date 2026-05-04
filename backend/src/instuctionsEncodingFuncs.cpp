#include <stdio.h>
#include <assert.h>

#include "../include/backendConsts.h"
#include "../include/instructionsEncoding.h"

int emitByte(programBuf, uint8_t byte) {
    assert(programBuf);

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

static inline int emit_u32(backendContext_t* cntxt, uint32_t value) {
    assert(cntxt);

    *(uint32_t*)(buf->code + buf->size) = value;
    buf->size += 4;
}

int emitRet (backendContext_t* cntxt) {
    assert(cntxt);
    emit_byte(buf, 0xC3);
}

int emitCqo(backendContext_t* cntxt) {
    assert(backendContext_t* cntxt)

    emitByte(cntxt, 0x48);
    emitByte(cntxt, 0x99);
}

int emitPushReg(backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    emitByte(cntxt, 0x50 + regCode);
}

int emitPopReg(backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    emitByte(cntxt, 0x58 + regCode);
}

int emitOpRegReg (backendContext_t* cntxt, opRegRegCode_t opCode, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    emitRex64(cntxt, destReg, srcReg, NO_REG);

    if (opCode == IMUL) {
        emitByte(cntxt, 0x0F);
        emitModRM(cntxt, REG_REG, destReg, srcReg);
    }
    else {
        emitByte(cntxt, opCode);
        emitModRM(cntxt, REG_REG, srcReg, destReg);
    }
}

int emitOpRegReg(backendContext_t* cntxt, opRegRegCode_t opCode, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    if (opCode == IMUL) {
        emitRex64(cntxt, destReg, srcReg, NO_REG);
        emitByte(cntxt, 0x0F);
        emitByte(cntxt, 0xAF);
        emitModRM(cntxt, REG_REG, destReg, srcReg);
    } else {
        emitRex64(cntxt, srcReg, destReg, NO_REG);
        emitByte(cntxt, (uint8_t)opCode);
        emitModRM(cntxt, REG_REG, srcReg, destReg);
    }
    return 0;
}

int emitIdiv (backendContext_t* cntxt, regCode_t srcReg) {
    assert(cntxt);

    emitRex64(cntxt, NO_REG, srcReg, NO_REG);
    emitByte(cntxt, 0xF7);
    emitModRM(cntxt, REG_REG, 0x07, srcReg);
}

int emitMov (backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, int32_t disp) {
    assert(cntxt);

    bool direction = (modARGS == MEM_REG) || (modARGS == REG_REG);

    uint8_t movOpCode = (direction) ? 0x8B: 0x89;
    movOpCode = (modARGS == REG_CONST) ? movOpCode + destReg: movOpCode;

    if (modARGS != REG_REG && modARGS != REG_CONST) {
        modARGS = (disp || (baseReg & 7) == RBP) ? modARGS : MEM_NO_OFFSET;
        modARGS = (disp >= -128 && disp <= 127) ? MEM_32_OFFSET : MEM_8_OFFSET;
    }

    emitRex64(cntxt, destReg, srcReg, NO_REG);

    if (modARGS != REG_CONST) {
        uint8_t reg = (direction) ? destReg : srcReg;
        uint8_t rm = (modARGS == REG_REG) ? srcReg : RSP;

        emitModRM(cntxt, modARGS, reg, rm);
    }


    if (modARGS != REG_REG && modARGS != REG_CONST) {

        regCode_t baseReg = (direction) ? destReg, srcReg;

        emitSIB(cntxt, 0x00, NO_REG, baseReg);
        if (modARGS == MEM_32_OFFSET)
            emit_u32(cntxt, disp);
        if (modARGS == MEM_8_OFFSET)
            emitByte(cntxt, (uint8_t)disp);
    }
}

int emitMov(backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, int64_t disp) {
    assert(cntxt);

    bool direction = (modARGS == REG_MEM);

    uint8_t opCode = direction ? 0x8B : 0x89;
    opCode = (modARGS == REG_CONST) ? 0xB8 + (destReg & 7): opCode;

    regCode_t valueReg = direction ? destReg : srcReg;
    regCode_t baseReg  = direction ? srcReg  : destReg;

    emitRex64(cntxt, baseReg, valueReg, NO_REG);
    emitByte(cntxt, opCode);

    if (modARGS == REG_CONST) {
        emit_u64(cntxt, (uint64_t)disp);
        return 0;
    }

    if (modARGS == REG_REG) {
        emitModRM(cntxt, REG_REG, valueReg, baseReg);
        return 0;
    }

    uint8_t mod = (!disp && (baseReg & 7) != RBP) ? MEM_NO_OFFSET: MEM_8_OFFSET;
    mod = (disp >= -128 && disp <= 127) ? mod: MEM_32_OFFSET;

    emitModRM(cntxt, mod, valueReg, baseReg);

    if ((baseReg & 7) == RSP)
        emitSIB(cntxt, 0x00, 0x04, RSP);

    if (mod == 0x01)
        emitByte(cntxt, (uint8_t)disp);
    else if (mod == 0x02)
        emit_u32(cntxt, (uint32_t)disp);

    return 0;
}

int emitJMPorCALL (backendContext_t* cntxt, bool isCnd, opCode_t opCode, uint64_t labelAddr) {
    assert(cntxt);
    int32_t offset = (int32_t)(labelAddr - nextInstAddr);

    if (isCnd)
        emit(cntxt, 0x0F);

    emitByte(cntxt, opCode);
    emit_u32(cntxt, offset);
}

int emitSETcc (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg) {
    assert(cntxt);

    emitByte(cntxt, 0x0F);
    emitByte(cntxt, opCode);

    emitModRM(cntxt, REG_REG, 0x00, destReg);
}
