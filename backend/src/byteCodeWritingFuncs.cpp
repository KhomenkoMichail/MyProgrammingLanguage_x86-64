#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/stdLibHex.h"

int astToByteCode (backendContext_t* cntxt) {
    assert(cntxt);

    nodeToByteCode(cntxt, *treeRoot(*cntxtTree(cntxt)), LEFT);

    return BACKEND_SUCCESS;
}

int nodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(node);
    assert(cntxt);

    switch (*nodeType(node)) {
        case typeNumber:
            emitMov(cntxt, REG_CONST, resultReg, NO_REG, nodeValue(node)->constValue);
            return BACKEND_SUCCESS;
        case typeOperator:
            return opNodeToByteCode(cntxt, node, resultReg);
        case typeIdentifier:
            return idNodeToByteCode(cntxt, node, resultReg);
        case typeError:
        default:
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NODE_TYPE,
                        "Error of recognising a node type in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }
}

int opNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    switch ((*nodeValue(node)).opCode) {
        case opADD:
        case opSUB:
        case opMUL:
        case opDIV:
        case opSEPARATOR:
        case opCOMMA:
            return opCalcToByteCode(cntxt, node, resultReg);

        case opASSIGN: return opAssignToByteCode(cntxt, node, resultReg);
        case opWHILE: return opWhileToByteCode(cntxt, node);
        case opIF: return opIfToByteCode(cntxt, node);
        case opIN: return opInToByteCode(cntxt, node);
        case opOUT: return opOutToByteCode(cntxt, node);
        case opRET: return opRetToByteCode(cntxt, node);
        case opSQRT: return opSqrtToByteCode(cntxt, node, resultReg);

        case opHLT: emitByte(cntxt, opCodeJMP);
        return = patchCurLabel(cntxt, "stdExit");

        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return opCompareToByteCode(cntxt, node, resultReg);

        case opUNITED_ON:
        case opUNITED_OFF:
        case opQUOTES:
        case opINIT:
        case opBRACK_ON:
        case opBRACK_OFF:
        case opUNKNOWN:
        default:
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_UNEXPECTED_OPCODE,
                        "Error: unexpected opcode of node in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    return BACKEND_SUCCESS;
}

int opCalcToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(node);
    assert(cntxt);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node))
        errorCode = nodeToByteCode (cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: calc node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeRight(node)) {
        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            emitPushReg(cntxt, (regCode_t)LEFT);

        errorCode = nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;

        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            emitPopReg(cntxt, (regCode_t)LEFT);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: calc node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    switch (*nodeOpCode(node);) {
        case (opADD):
            emitOpRegReg(cntxt, opCodeADD, (regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opSUB):
            emitOpRegReg(cntxt, opCodeSUB, (regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opMUL):
            emitOpRegReg(cntxt, opCodeIMUL, (regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opDIV):
            if (*regIsUsed(cntxt, RDX))
                emitPushReg(cntxt, RDX);

            emitCqo(cntxt);
            emitIdiv(cntxt, (regCode_t)RIGHT);

            if (*regIsUsed(cntxt, RDX))
                emitPopReg(cntxt, RDX);
        default:
            break;
    }

    /*
    if (*nodeOpCode(node); == opADD)
        emitOpRegReg(cntxt, opCodeADD, (regCode_t)LEFT, (regCode_t)RIGHT);

    if (*nodeOpCode(node); == opSUB)
        emitOpRegReg(cntxt, opCodeSUB, (regCode_t)LEFT, (regCode_t)RIGHT);

    if (*nodeOpCode(node); == opMUL)
        emitOpRegReg(cntxt, opCodeIMUL, (regCode_t)LEFT, (regCode_t)RIGHT);

    if (*nodeOpCode(node); == opDIV) {
        if (*regIsUsed(cntxt, RDX))
            emitPushReg(cntxt, RDX);

        emitCqo(cntxt);
        emitIdiv(cntxt, (regCode_t)RIGHT);

        if (*regIsUsed(cntxt, RDX))
            emitPopReg(cntxt, RDX);
    }*/

    if (resultReg == RIGHT)
        emitMov(cntxt, REG_REG, (regCode_t)RIGHT, (regCode_t)LEFT, NO_DESP);

    return errorCode;
}

int opAssignToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeRight(node))
        errorCode = nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: assign node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, node);
        if (*varPosInReg(&varPos))
            emitMov(cntxt, REG_REG, *varPosRegCode(&varPos), (regCode_t)RIGHT, NO_DISP);
        else
            emitMov(cntxt, MEM_REG, RBP, (regCode_t)RIGHT, *varPosRbpOffset(&varPos));
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: assign node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    return errorCode;
}


int opWhileToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    (*cntxtWhileCounter(cntxt))++;
    int curWhileCntr = *cntxtWhileCounter(cntxt);

    size_t whileStartBufPos = *cntxtProgramBufSize(cntxt);        //while:

    if (*nodeLeft(node)) {
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    emitOpRegReg(cntxt, opCodeTEST, (regCode_t)LEFT, (regCode_t)LEFT);    //test rax rax

    emit(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeJZ);                                      //jz 0000
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt);
    emit_32CurPos(cntxt, 0x00);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);

    emitJMPorCALL(cntxt, false, opCodeJMP, whileStartBufPos);   //JMP while

    int32_t jzOffset = (int32_t)(*cntxtProgramBufSize(cntxt) - jzOffsetBufPos);
    emit_32givenPos(cntxt, jzOffsetBufPos, (uint32_t)jzOffset);

    return errorCode;
}

int opIfToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    (*cntxtIfCounter(cntxt))++;
    int curIfCntr = *cntxtIfCounter(cntxt);

    if (*nodeLeft(node)) {
        errorCode = nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    emitOpRegReg(cntxt, opCodeTEST, (regCode_t)LEFT, (regCode_t)LEFT);

    emit(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeJZ);
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt);
    emit_32CurPos(cntxt, 0x00);

    if (*nodeRight(node))
        errorCode = nodeToByteCode(cntxt, *nodeRight(node), RIGHT);

    int32_t jzOffset = (int32_t)(*cntxtProgramBufSize(cntxt) - jzOffsetBufPos);
    emit_32givenPos(cntxt, jzOffsetBufPos, (uint32_t)jzOffset);

    return errorCode;
}

int opInToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);
    //fprintf(*cntxtByteFile(cntxt), "call stdIn\n"); //FIXME
    emitByte(cntxt, opCodeJMP);
    errorCode = patchCurLabel(cntxt, "stdIn");
    popSavedRegsByteCode(cntxt, pushedRegsMask);

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, node);
        if (*varPosInReg(&varPos))
            emitMov(cntxt, REG_REG, *varPosRegCode(&varPos), RAX, NO_DISP);
        else
            emitMov(cntxt, MEM_REG, RBP, RAX, *varPosRbpOffset(&varPos));
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: in node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    return errorCode;
}


int opOutToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, node);
        if (*varPosInReg(&varPos))
            emitMov(cntxt, REG_REG, RAX, *varPosRegCode(&varPos), NO_DISP);
        else
            emitMov(cntxt, REG_MEM, RAX, RBP, *varPosRbpOffset(&varPos));
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                "Error: OUT op node does not have LEFT in func %s, %s:%d\n",
                __func__, __FILE__, __LINE__);
    }

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);
    //fprintf(*cntxtByteFile(cntxt), "call stdOut\n");                   //FIXME
    emitByte(cntxt, opCodeJMP);
    return = patchCurLabel(cntxt, "stdOut");
    popSavedRegsByteCode(cntxt, pushedRegsMask);

    return errorCode;
}


int opRetToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node))
        errorCode = nodeToByteCode(cntxt, *nodeLeft(node), LEFT);

    freeScopeRegs(cntxt);
    popSavedRegsByteCode(cntxt, *curScopePushedRegsMask(*cntxtTree(cntxt)));

    emitMov(cntxt, REG_REG, RSP, RBP, NO_DISP);
    emitPopReg(cntxt, RBP);
    emitRet(cntxt);

    return errorCode;
}

int opCompareToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeRight(node)) {
        errorCode = nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: compare node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node))
        errorCode = nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: compare node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    emitOpRegReg(cntxt, opCodeCMP, (regCode_t)LEFT, (regCode_t)RIGHT);

    switch (*nodeOpCode(node)) {
        case opEQUAL:
            emitSETcc(cntxt, opCodeSETe, regCode_t destReg)
            break;
        case opNOT_EQUAL:
            emitSETcc(cntxt, opCodeSETne, regCode_t destReg)
            break;
        case opBELOW:
            emitSETcc(cntxt, opCodeSETl, regCode_t destReg)
            break;
        case opABOVE:
            emitSETcc(cntxt, opCodeSETg, regCode_t destReg)
            break;
        case opE_BELOW:
            emitSETcc(cntxt, opCodeSETle, regCode_t destReg);
            break;
        case opE_ABOVE:
            emitSETcc(cntxt, opCodeSETge, regCode_t destReg);
        default:
            break;
    }

    /*if (*nodeOpCode(node); == opEQUAL)
        fprintf(*cntxtByteFile(cntxt), "sete al\n");        //FIXME
    if (*nodeOpCode(node); == opNOT_EQUAL)
        fprintf(*cntxtByteFile(cntxt), "setne al\n");
    if (*nodeOpCode(node); == opBELOW)
        fprintf(*cntxtByteFile(cntxt), "setl al\n");
    if (*nodeOpCode(node); == opABOVE)
        fprintf(*cntxtByteFile(cntxt), "setg al\n");
    if (*nodeOpCode(node); == opE_BELOW)
        fprintf(*cntxtByteFile(cntxt), "setle al\n");
    if (*nodeOpCode(node); == opE_ABOVE)
        fprintf(*cntxtByteFile(cntxt), "setge al\n");*/

    emitMovzxRR8(cntxt, (regCode_t)resultReg, RAX);

    return errorCode;
}

int idNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    if(*nodeRight(node))
        return FuncBodyToByteCode(cntxt, node);

    const char* idName = *nodeIdentifierName(node);

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));
    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return varNodeToByteCode(cntxt, node, resultReg);
    else
        return FuncCallNodeToByteCode(cntxt, node, resultReg);
}

int funcBodyToByteCode(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    errorCode = addLabelAddressInCntxt(cntxt, *nodeIdentifierName(node));
    if (errorCode) return errorCode;

    emitPushReg(cntxt, RBP);
    emitMov(cntxt, REG_REG, RBP, RSP, NO_DISP);

    size_t numOfStackVars = (*nodeFuncNumOfLocalVars(node) > NUM_OF_VARS_REGS) ?
                            *nodeFuncNumOfLocalVars(node) - NUM_OF_VARS_REGS : 0;

    //fprintf(*cntxtByteFile(cntxt), "sub rsp, %zu\n", numOfStackVars * 8);
    emitAluRegConst(cntxt, RSP, numOfStackVars * 8, aluSUB);

    enterNewScope(*cntxtTree(cntxt));

    *curScopePushedRegsMask(*cntxtTree(cntxt)) = pushSavedRegsByteCode(cntxt, calleeSaved);

    if (*nodeLeft(node)) {
        errorCode = fprintfGettingParamsToByteCode(cntxt, *nodeLeft(node));
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    if (*nodeRight(node))
        errorCode = NodeToByteCode(cntxt, *nodeRight(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: func body node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    exitScope(*cntxtTree(cntxt));

    return errorCode;
}

int fprintfGettingParamsToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfGettingParamsToByteCode(cntxt, *nodeLeft(node));

        if (errorCode != BACKEND_SUCCESS) return errorCode;

        errorCode = fprintfGettingParamsToByteCode(cntxt, *nodeRight(node));
    }
    else
        addIdToCurrentScope(*cntxtTree(cntxt), nodeVarName(node), idPARAM);

    return errorCode;
}

int funcCallNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;
    int numOfFuncParams = 0;

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);

    if (*nodeLeft(node))
        numOfFuncParams = fprintfPassingParams(cntxt, *nodeLeft(node));

    emitByte(cntxt, opCodeCALL);
    errorCode = patchCurLabel(cntxt, *nodeIdentifierName(node));
    if (errorCode) return errorCode;
    //fprintf(*cntxtByteFile(cntxt), "call %s\n", *nodeIdentifierName(node));

    if (*nodeLeft(node))
        emitAluRegConst(cntxt, RSP, numOfFuncParams * 8, aluADD);
        //fprintf(*cntxtByteFile(cntxt), "add rsp, %d\n", numOfFuncParams * 8);

    if (resultReg == RIGHT)
        emitMov(cntxt, REG_REG, (regCode_t)RIGHT, (regCode_t)LEFT, NO_DISP);

    popSavedRegsByteCode(cntxt, pushedRegsMask);

    return errorCode;
}

int fprintfByteCodePassingParams (backendContext_t* cntxt, node_t* node) {

    int curParamsCounter = 0;

    if(*nodeType(node) == typeOperator && (*nodeValue(node)).opCode == opCOMMA) {
        curParamsCounter += fprintfByteCodePassingParams(cntxt, *nodeRight(node));
        curParamsCounter += fprintfByteCodePassingParams(cntxt, *nodeLeft(node));
    }
    else {
        nodeToByteCode(cntxt, node, LEFT);
        emitPushReg(cntxt, (regCode_t)LEFT);
        curParamsCounter++;
    }

    return curParamsCounter;
}

varPos_t getVarPos(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*nodeType(node) != typeIdentifier) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_UNEXPECTED_NODE_TYPE,
                        "Error: node type <%d> is unexpected in func %s, %s:%d\n",
                        *nodeType(node), __func__, __FILE__, __LINE__);
    }

    varPos_t varPos = {};

    char* varName = nodeVarName(node);

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));

    identifierInfo* searchedVarId = findIdInTable(curNameTable, varName);

    if(!searchedVarId)
        searchedVarId = addIdToCurrentScope(*cntxtTree(cntxt), varName, idVAR);

    if (*varReg(searchedVarId) == NOT_IN_REG) {
        int freeRegCode = findFreeReg(cntxt);

        if (freeRegCode != NOT_IN_REG  && *infoIdType(searchedVarId) == idVAR) {
            *varReg(searchedVarId) = freeRegCode;
            *regIsUsed(cntxt, (regCode_t)freeRegCode) = true;
        }
        else {
            if (*varOffset(searchedVarId) == NOT_IN_MEMORY) {
                *numOfTableLocalVars(curNameTable) += 1;
                *varOffset(searchedVarId) = -((*numOfTableLocalVars(curNameTable) + 1) * 8);

                *varPosRbpOffset(&varPos);
                return varPos;
            }
        }
    }

    *varPosInReg(&varPos) = true;
    *varPosRegCode(&varPos) = (regCode_t)(*varReg(searchedVarId));

    return varPos;
}

int varNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    varPos_t varPos = getVarPos(cntxt, node);

    if (*varPosInReg(&varPos))
        emitMov(cntxt, REG_REG, resultReg, *varPosRegCode(&varPos), NO_DISP);
    else
        emitMov(cntxt, REG_MEM, resultReg, RBP, *varPosRbpOffset(&varPos));

    return BACKEND_SUCCESS;
}

int opSqrtToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node)) {
        errorCode = nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                           "Error: SQRT node does not have LEFT in func %s, %s:%d\n",
                           __func__, __FILE__, __LINE__);
    }

    //fprintf(*cntxtByteFile(cntxt), "cvtsi2sd xmm0, %s\n", OP_REG_[LEFT]);    //FIXME
    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitRex64(cntxt, NO_REG, (regCode_t)LEFT, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeCVTSI2SD);
    emitModRM(cntxt, REG_REG, XMM0_CODE, (regCode_t)LEFT);

    //fprintf(*cntxtByteFile(cntxt), "sqrtsd xmm0, xmm0\n");
    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeSQRTSD);
    emitModRM(cntxt, REG_REG, XMM0_CODE, XMM0_CODE);

    //fprintf(*cntxtByteFile(cntxt), "cvttsd2si %s, xmm0\n", OP_REG_[resultReg]); //FIXME
    emitByte(cntxt, DOUBLE_PRECISION_PREFIX);
    emitRex64(cntxt, (regCode_t)resultReg, NO_REG, NO_REG);
    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeCVVTSD2SI);
    emitModRM(cntxt, REG_REG, (uint8_t)resultReg, XMM0_CODE);

    return errorCode;
}

uint32_t pushSavedRegsByteCode (backendContext_t* cntxt, regSaveDecl_t saveDecl) {
    assert(cntxt);

    uint32_t pushedRegsMask = 0;

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++) {
        if (*regIsUsed(cntxt, (regCode_t)curReg) && regSaveDecl(cntxt, (regCode_t)curReg) == saveDecl) {

            emitPushReg(cntxt, (regCode_t)curReg);

            pushedRegsMask |= (1u << curReg);
            *regIsUsed(cntxt, (regCode_t)curReg) = false;
        }
    }

    return pushedRegsMask;
}

void popSavedRegsByteCode (backendContext_t* cntxt, uint32_t pushedRegsMask) {
    assert(cntxt);

    for (int curReg = NUM_OF_REGS - 1; curReg >= 0; curReg--) {
        if (pushedRegsMask & (1u << curReg)) {

            emitPopReg(cntxt, (regCode_t)curReg);

            *regIsUsed(cntxt, (regCode_t)curReg) = true;
        }
    }
}

int addStdLibInBuffer (backendContext_t* cntxt) {
    assert(cntxt);

    int errorCode = BACKEND_SUCCESS;

    emitByte(cntxt, opCodeJMP);
    errorCode = patchCurLabel(cntxt, "main");
    if (errorCode) return errorCode;

    errorCode = addLabelAddressInCntxt(cntxt, "stdExit");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDEXIT_OFFSET);
    if (errorCode) return errorCode;

    errorCode = addLabelAddressInCntxt(cntxt, "stdIn");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDIN_OFFSET);
    if (errorCode) return errorCode;

    errorCode = errorCode = addLabelAddressInCntxt(cntxt, "stdOut");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDOUT_OFFSET);
    if (errorCode) return errorCode;

    if (STDLIB_SIZE > *cntxtProgramBufCapacity(cntxt)) {
        int* newBuf = (int*)realloc(*cntxtProgramBuf(cntxt), STDLIB_SIZE * 2);
        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error realloc program buffer in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
    }

    memcpy(*cntxtProgramBuf(cntxt), STDLIB_CODE, STDLIB_SIZE);
    *cntxtProgramBufSize(cntxt) += STDLIB_SIZE;

    return BACKEND_SUCCESS;
}


