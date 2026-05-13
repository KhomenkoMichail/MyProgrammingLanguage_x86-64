#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/instructionsEncoding.h"
#include "../include/backendCntxtFuncs.h"

int emitByte (backendContext_t* cntxt, uint8_t byte) {
    assert(cntxt);

    if (*cntxtErrCode(cntxt) != BACKEND_SUCCESS) return *cntxtErrCode(cntxt);

    if (*cntxtProgramBufSize(cntxt) >= *cntxtProgramBufCapacity(cntxt)) {
        uint8_t* newBuf = (uint8_t*)realloc(*cntxtProgramBuf(cntxt), sizeof(uint8_t) *
                                            *cntxtProgramBufCapacity(cntxt) * 2);

        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error program buffer realloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
        *cntxtProgramBufCapacity(cntxt) *= 2;
    }

    *cntxtProgramBufElem(cntxt, *cntxtProgramBufSize(cntxt)) = byte;
    (*cntxtProgramBufSize(cntxt))++;

    return BACKEND_SUCCESS;
}

int emitRex64 (backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg, regCode_t indexReg) {
    assert(cntxt);

    uint8_t w = 1;
    uint8_t r = (srcReg >= R8)  ? 1 : 0;
    uint8_t x = (indexReg >= R8 && indexReg != NO_REG) ? 1 : 0;
    uint8_t b = (destReg >= R8) ? 1 : 0;

    uint8_t rex = 0x40 | (w << 3) | (r << 2) | (x << 1) | b;

    return emitByte(cntxt, rex);
}

int emitModRM(backendContext_t* cntxt, uint8_t mod, uint8_t reg, uint8_t rm) {
    assert(cntxt);

    uint8_t modrm = (mod << 6) | ((reg & 7) << 3) | (rm & 7);

    return emitByte(cntxt, modrm);
}

int emitSIB (backendContext_t* cntxt, uint8_t scale, regCode_t indexReg, regCode_t baseReg) {
    assert(cntxt);

    uint8_t index = (uint8_t)indexReg;
    uint8_t base  = (uint8_t)baseReg;

    uint8_t SIB = (scale << 6) | (index << 3) | base;

    return emitByte(cntxt, SIB);
}

int emit_32CurPos (backendContext_t* cntxt, int32_t value) {
    assert(cntxt);

    if (*cntxtErrCode(cntxt) != BACKEND_SUCCESS) return *cntxtErrCode(cntxt);

    if (*cntxtProgramBufSize(cntxt) + sizeof(int32_t) >= *cntxtProgramBufCapacity(cntxt)) {
        uint8_t* newBuf = (uint8_t*)realloc(*cntxtProgramBuf(cntxt), sizeof(uint8_t) *
                                            *cntxtProgramBufCapacity(cntxt) * 2);

        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error program buffer realloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
        *cntxtProgramBufCapacity(cntxt) *= 2;
    }

    *(int32_t*)(*cntxtProgramBuf(cntxt) + *cntxtProgramBufSize(cntxt)) = value;
    *cntxtProgramBufSize(cntxt) += sizeof(int32_t);

    return BACKEND_SUCCESS;
}

int emit_64CurPos (backendContext_t* cntxt, int32_t value) {
    assert(cntxt);

    if (*cntxtErrCode(cntxt) != BACKEND_SUCCESS) return *cntxtErrCode(cntxt);

    if (*cntxtProgramBufSize(cntxt) + sizeof(int64_t) >= *cntxtProgramBufCapacity(cntxt)) {
        uint8_t* newBuf = (uint8_t*)realloc(*cntxtProgramBuf(cntxt), sizeof(uint8_t) *
                                            *cntxtProgramBufCapacity(cntxt) * 2);

        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error program buffer realloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
        *cntxtProgramBufCapacity(cntxt) *= 2;
    }

    *(int64_t*)(*cntxtProgramBuf(cntxt) + *cntxtProgramBufSize(cntxt)) = value;
    *cntxtProgramBufSize(cntxt) += sizeof(int64_t);

    return BACKEND_SUCCESS;
}

int emit_32givenPos (backendContext_t* cntxt, size_t givenBufPos, uint32_t value) {
    assert(cntxt);

    if (*cntxtErrCode(cntxt) != BACKEND_SUCCESS) return *cntxtErrCode(cntxt);

    if (givenBufPos + sizeof(int32_t) >= *cntxtProgramBufCapacity(cntxt)) {
        uint8_t* newBuf = (uint8_t*)realloc(*cntxtProgramBuf(cntxt), sizeof(uint8_t) *
                                            *cntxtProgramBufCapacity(cntxt) * 2);

        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error program buffer realloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
        *cntxtProgramBufCapacity(cntxt) *= 2;
    }

    *(int32_t*)(*cntxtProgramBuf(cntxt) + givenBufPos) = value;
    *cntxtProgramBufSize(cntxt) = (givenBufPos + sizeof(int32_t) > *cntxtProgramBufSize(cntxt))?
                                   givenBufPos + sizeof(int32_t) :  *cntxtProgramBufSize(cntxt);

    return BACKEND_SUCCESS;
}


int emitRet (backendContext_t* cntxt) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "ret\n");
    return emitByte(cntxt, opCodeRET);
}

int emitCqo (backendContext_t* cntxt) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "cqo\n");

    emitByte(cntxt, REX_W_BYTE);
    emitByte(cntxt, opCodeCQO);

    return *cntxtErrCode(cntxt);
}

int emitPushReg (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    if (regCode >= R8)
        emitByte(cntxt, REX_B_BYTE);

    fprintf(*cntxtAsmFile(cntxt), "push %s\n", regName(cntxt, regCode));

    return emitByte(cntxt, (uint8_t)(opCodePUSH + (regCode & 7)));
}

int emitPopReg (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);
    if (regCode >= R8)
        emitByte(cntxt, REX_B_BYTE);

    fprintf(*cntxtAsmFile(cntxt), "pop %s\n", regName(cntxt, regCode));

    return emitByte(cntxt, (uint8_t)(opCodePOP + (regCode & 7)));
}

int emitOpRegReg (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    switch (opCode) {
        case opCodeADD: fprintf(*cntxtAsmFile(cntxt), "add ");   break;
        case opCodeSUB: fprintf(*cntxtAsmFile(cntxt), "sub ");   break;
        case opCodeIMUL: fprintf(*cntxtAsmFile(cntxt), "imul "); break;
        case opCodeTEST: fprintf(*cntxtAsmFile(cntxt), "test "); break;
        case opCodeCMP: fprintf(*cntxtAsmFile(cntxt), "cmp ");   break;
        default: break;
    }
    fprintf(*cntxtAsmFile(cntxt), "%s, %s\n", regName(cntxt, destReg), regName(cntxt, srcReg));

    if (opCode == opCodeIMUL) {
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

    return *cntxtErrCode(cntxt);;
}

int emitIdiv (backendContext_t* cntxt, regCode_t srcReg) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "idiv %s\n", regName(cntxt, srcReg));

    emitRex64(cntxt, NO_REG, srcReg, NO_REG);
    emitByte(cntxt, opCodeIDIV);
    emitModRM(cntxt, REG_REG, IDIV_EXTRA_OPCODE, srcReg);

    return *cntxtErrCode(cntxt);
}

int emitMov (backendContext_t* cntxt, modARGS_t modARGS, regCode_t destReg, regCode_t srcReg, int32_t disp) {
    assert(cntxt);

    switch (modARGS) {
        case REG_REG:
            fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", regName(cntxt, destReg), regName(cntxt, srcReg));
            break;
        case REG_CONST:
            fprintf(*cntxtAsmFile(cntxt), "mov %s, %d\n", regName(cntxt, destReg), disp);
            break;
        case REG_MEM:
            fprintf(*cntxtAsmFile(cntxt), "mov %s, [%s + %d]\n", regName(cntxt, destReg), regName(cntxt, srcReg), disp);
            break;
        case MEM_REG:
            fprintf(*cntxtAsmFile(cntxt), "mov [%s + %d], %s\n", regName(cntxt, destReg), disp, regName(cntxt, srcReg));
            break;
        default:
            break;
    }

    bool direction = (modARGS == REG_MEM);

    uint8_t opCode = direction ? opCodeMOVsetDir : opCodeMOVcleanDir;
    opCode = (modARGS == REG_CONST) ? opCodeMOVregConst + (destReg & 7): opCode;

    regCode_t valueReg = direction ? destReg : srcReg;
    regCode_t baseReg  = direction ? srcReg  : destReg;

    emitRex64(cntxt, baseReg, valueReg, NO_REG);
    emitByte(cntxt, opCode);

    if (modARGS == REG_CONST) {
        emit_64CurPos(cntxt, (uint64_t)disp);
        return *cntxtErrCode(cntxt);
    }

    if (modARGS == REG_REG) {
        emitModRM(cntxt, REG_REG, valueReg, baseReg);
        return *cntxtErrCode(cntxt);
    }

    uint8_t mod = (!disp && (baseReg & 7) != RBP) ? MEM_NO_OFFSET: MEM_8_OFFSET;
    if (disp < -128 || disp > 127)
        mod = MEM_32_OFFSET;

    emitModRM(cntxt, mod, valueReg, baseReg);

    if ((baseReg & 7) == RSP)
        emitSIB(cntxt, 0x00, (regCode_t)NO_INDEX_REG, RSP);

    if (mod == MEM_8_OFFSET)
        emitByte(cntxt, (uint8_t)disp);
    else if (mod == MEM_32_OFFSET)
        emit_32CurPos(cntxt, disp);

    return *cntxtErrCode(cntxt);
}

int emitJMP (backendContext_t* cntxt, bool isCnd, opCode_t opCode, int32_t offset) {
    assert(cntxt);

    if (isCnd)
        emitByte(cntxt, ESCAPE_PREFIX);

    emitByte(cntxt, opCode);

    emit_32CurPos(cntxt, offset);
    return *cntxtErrCode(cntxt);
}

int emitPatchCALLorJMP (backendContext_t* cntxt, opCode_t opCode, bool isCnd, const char* patchLabelName) {
    assert(cntxt);
    assert(patchLabelName);

    switch (opCode) {
        case opCodeCALL: fprintf(*cntxtAsmFile(cntxt), "call %s\n", patchLabelName); break;
        case opCodeJMP:  fprintf(*cntxtAsmFile(cntxt), "jmp %s\n", patchLabelName);  break;
        case opCodeJZ:   fprintf(*cntxtAsmFile(cntxt), "jz %s\n", patchLabelName);  break;
        default: break;
    }

    if (isCnd)
        emitByte(cntxt, ESCAPE_PREFIX);

    emitByte(cntxt, opCode);
    patchCurLabel(cntxt, patchLabelName);

    return *cntxtErrCode(cntxt);
}

int emitSETcc (backendContext_t* cntxt, opCode_t opCode, regCode_t destReg) {
    assert(cntxt);

    switch (opCode) {
        case opCodeSETe:  fprintf(*cntxtAsmFile(cntxt), "sete %s\n", regLowByteName(cntxt, destReg));         break;
        case opCodeSETne: fprintf(*cntxtAsmFile(cntxt), "setne %s\n", regLowByteName(cntxt, destReg)); break;
        case opCodeSETl:  fprintf(*cntxtAsmFile(cntxt), "setl %s\n", regLowByteName(cntxt, destReg));  break;
        case opCodeSETg:  fprintf(*cntxtAsmFile(cntxt), "setg %s\n", regLowByteName(cntxt, destReg));  break;
        case opCodeSETle: fprintf(*cntxtAsmFile(cntxt), "setle %s\n", regLowByteName(cntxt, destReg)); break;
        case opCodeSETge: fprintf(*cntxtAsmFile(cntxt), "setge %s\n", regLowByteName(cntxt, destReg)); break;
        default:
            break;
    }

    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCode);

    emitModRM(cntxt, REG_REG, 0x00, destReg);

    return *cntxtErrCode(cntxt);
}

int emitMovzxRR8(backendContext_t* cntxt, regCode_t destReg, regCode_t srcReg) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "movzx %s, %s\n", regName(cntxt, destReg), regLowByteName(cntxt, srcReg));

    emitRex64(cntxt, destReg, srcReg, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);

    emitByte(cntxt, opCodeMOVZXrr8);

    emitModRM(cntxt, REG_REG, destReg, srcReg);

    return *cntxtErrCode(cntxt);
}

int emitAluRegConst(backendContext_t* cntxt, regCode_t regCode, int32_t imm, aluOpCode_t aluOpCode) {
    assert(cntxt);

    if (aluOpCode == aluSUB)
        fprintf(*cntxtAsmFile(cntxt), "sub %s, %d\n", regName(cntxt, regCode), imm);
    if (aluOpCode == aluADD)
        fprintf(*cntxtAsmFile(cntxt), "add %s, %d\n", regName(cntxt, regCode), imm);

    emitRex64(cntxt, NO_REG, regCode, NO_REG);

    if (imm >= -128 && imm <= 127) {
        emitByte(cntxt, opCodeALU_R_IMM8);
        emitModRM(cntxt, REG_REG, (uint8_t)aluOpCode, regCode);
        emitByte(cntxt, (uint8_t)imm);
    }
    else {
        emitByte(cntxt, opCodeALU_R_IMM32);
        emitModRM(cntxt, REG_REG, (uint8_t)aluOpCode, regCode);
        emit_32CurPos(cntxt, (int32_t)imm);
    }

    return *cntxtErrCode(cntxt);
}

int emitBreakpoint (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    if (*nodeType(node) == typeOperator && nodeValue(node)->opCode == opSEPARATOR)
        return *cntxtErrCode(cntxt);

    node_t* parentNode = *nodeParent(node);

    if (parentNode)
        if (!(*nodeType(parentNode) == typeOperator && nodeValue(parentNode)->opCode == opSEPARATOR))
            if (*nodeLine(node) == *nodeLine(parentNode))
                return *cntxtErrCode(cntxt);

    return emitByte(cntxt, opCodeBREAKPOINT);
}

int emitCVTSI2SD(backendContext_t* cntxt, xmmRegCode_t dstXmmRegCode, regCode_t srcRegCode) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "cvtsi2sd %s, %s\n", xmmArr[dstXmmRegCode].name,
                                                       regName(cntxt, srcRegCode));

    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitRex64(cntxt, NO_REG, srcRegCode, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeCVTSI2SD);
    emitModRM(cntxt, REG_REG, dstXmmRegCode, srcRegCode);

    return *cntxtErrCode(cntxt);
}

int emitSQRTSD(backendContext_t* cntxt, xmmRegCode_t dstXmmRegCode, xmmRegCode_t srcXmmRegCode) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "sqrtsd %s, %s\n", xmmArr[dstXmmRegCode].name,
                                                     xmmArr[srcXmmRegCode].name);

    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeSQRTSD);
    emitModRM(cntxt, REG_REG, dstXmmRegCode, srcXmmRegCode);

    return *cntxtErrCode(cntxt);
}

int emitCVTTSD2SI(backendContext_t* cntxt, regCode_t dstRegCode, xmmRegCode_t srcXmmRegCode) {
    assert(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "cvttsd2si %s, %s\n", regName(cntxt, dstRegCode),
                                                        xmmArr[srcXmmRegCode].name);

    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitRex64(cntxt, dstRegCode, NO_REG, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeCVTTSD2SI);
    emitModRM(cntxt, REG_REG, dstRegCode, srcXmmRegCode);

    return *cntxtErrCode(cntxt);
}
