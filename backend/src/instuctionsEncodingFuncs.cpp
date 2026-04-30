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

int emitRex64 (backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg, regCode_t indexReg) {
    assert(cntxt);

    uint8_t rex = 0x40;

    uint8_t w = 1;
    uint8_t r = (srcReg > RDI) ? 1 : 0;
    uint8_t x = (indexReg > RDI) ? 1 : 0;
    uint8_t b = (destReg > RDI) ? 1 : 0;

    rex = rex | (w << 3) | (r << 2) | (x << 1) | b;

    emitByte(cntxt, rex);
}

int emitModRM(backendContext_t* cntxt, uint8_t mod, uint8_t reg, uint8_t rm) {
    assert(cntxt);

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;
    emit_byte(buf, modrm);
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

static inline int emit_u64(backendContext_t* cntxt, uint64_t value) {
    assert(cntxt);

    *(uint64_t*)(buf->code + buf->size) = value;
    buf->size += 8;
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

int emitIdiv (backendContext_t* cntxt, regCode_t srcReg) {
    assert(cntxt);

    emitRex64(cntxt, NO_REG, srcReg, NO_REG);
    emitByte(cntxt, 0xF7);
    emitModRM(cntxt, REG_REG, 0x07, srcReg);
}

int emitMov (backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, uint32_t disp) {
    assert(cntxt);

    bool direction = (modARGS == MEM_REG) || (modARGS == REG_REG);

    uint8_t movOpCode = (direction) ? 0x8B: 0x89;
    movOpCode = (modARGS == REG_CONST) ? movOpCode + destReg: movOpCode;

    if (modARGS != REG_REG && modARGS != REG_CONST) {
        modARGS = (disp < 0xFF) ? MEM_32_OFFSET : MEM_8_OFFSET;
        modARGS = (disp) ? modARGS : MEM_NO_OFFSET;
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

int emitJMP (backendContext_t* cntxt, opCode_t opCode, uint32_t offset) {
    assert(cntxt);

    emitByte(cntxt, opCode);
    emit_u32(cntxt, offset);
}

int emitSETcc (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg) {
    assert(cntxt);

    emitByte(cntxt, 0x0F);
    emitByte(cntxt, opCode);

    emitModRM(cntxt, REG_REG, 0x00, destReg);
}
