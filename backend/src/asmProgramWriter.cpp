#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/asmProgramWriter.h"

int rewriteAstToAsmCode (backendContext_t* cntxt) {
    assert(cntxt);

    *cntxtAsmFile(cntxt) = fopen(*cntxtAsmFileName(cntxt), "w");

    if (!(*cntxtAsmFile(cntxt))) {
        fprintf(stderr, "Error of opening file \"%s\"", *cntxtAsmFileName(cntxt));
        perror("");
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_OPENING_ASM_FILE,
                        "Error of opening file \"%s\" in func %s, %s:%d\n",
                        *cntxtAsmFileName(cntxt), __func__, __FILE__, __LINE__);
    }
    fprintfAsmFileHeader(*cntxtAsmFile(cntxt));

    rewriteNodeToAsmCode(cntxt, *treeRoot(*cntxtTree(cntxt)), LEFT);

    fprintf(*cntxtAsmFile(cntxt), "%%include \"./backend/src/stdlib.asm\"\n");

    if (fclose(*cntxtAsmFile(cntxt)) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", *cntxtAsmFileName(cntxt));
        perror("");
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_CLOSING_ASM_FILE,
                        "Error of closing file \"%s\" in func %s, %s:%d\n",
                        *cntxtAsmFileName(cntxt), __func__, __FILE__, __LINE__);
    }

    return BACKEND_SUCCESS;
}

int rewriteNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(node);
    assert(cntxt);

    fprintfCommentsToAsm(node, *cntxtSrcFile(cntxt), *cntxtAsmFile(cntxt));

    switch (*nodeType(node)) {
        case typeNumber:
            fprintf(*cntxtAsmFile(cntxt), "mov %s, %d\n", OP_REG_[resultReg], nodeValue(node)->constValue);
            return BACKEND_SUCCESS;
        case typeOperator:
            return rewriteOpNodeToAsmCode(cntxt, node, resultReg);
        case typeIdentifier:
            return rewriteIdNodeToAsmCode(cntxt, node, resultReg);
        case typeError:
        default:
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NODE_TYPE,
                        "Error of recognising a node type in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }
}

int rewriteOpNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    switch ((*nodeValue(node)).opCode) {
        case opADD:
        case opSUB:
        case opMUL:
        case opDIV:
        case opSEPARATOR:
        case opCOMMA:
            return rewriteOpCalcToAsmCode(cntxt, node, resultReg);

        case opASSIGN: return rewriteOpAssignToAsmCode(cntxt, node);
        case opWHILE: return rewriteOpWhileToAsmCode(cntxt, node);
        case opIF: return rewriteOpIfToAsmCode(cntxt, node);
        case opIN: return rewriteOpInToAsmCode(cntxt, node);
        case opOUT: return rewriteOpOutToAsmCode(cntxt, node);
        case opRET: return rewriteOpRetToAsmCode(cntxt, node);
        case opSQRT: return rewriteOpSqrtToAsmCode(cntxt, node, resultReg);

        case opHLT: fprintf (*cntxtAsmFile(cntxt), "call stdExit\n"); return BACKEND_SUCCESS;

        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return rewriteOpCompareToAsmCode(cntxt, node, resultReg);


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

int rewriteOpCalcToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(node);
    assert(cntxt);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: calc node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeRight(node)) {
        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            fprintf(*cntxtAsmFile(cntxt), "push %s\n", OP_REG_[LEFT]);

        errorCode = rewriteNodeToAsmCode (cntxt, *nodeRight(node), RIGHT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;

        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            fprintf(*cntxtAsmFile(cntxt), "pop %s\n", OP_REG_[LEFT]);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: calc node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (nodeValue(node)->opCode == opADD)
        fprintf(*cntxtAsmFile(cntxt), "add %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opSUB)
        fprintf(*cntxtAsmFile(cntxt), "sub %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opMUL)
        fprintf(*cntxtAsmFile(cntxt), "imul %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opDIV) {
        if (*regIsUsed(cntxt, RDX))
            fprintf(*cntxtAsmFile(cntxt), "push rdx\n");

        fprintf(*cntxtAsmFile(cntxt), "cqo\n");
        fprintf(*cntxtAsmFile(cntxt), "idiv %s\n", OP_REG_[RIGHT]);

        if (*regIsUsed(cntxt, RDX))
            fprintf(*cntxtAsmFile(cntxt), "pop rdx\n");
    }

    if (resultReg == RIGHT)
        fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", OP_REG_[RIGHT], OP_REG_[LEFT]);

    return errorCode;
}

int rewriteOpAssignToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: assign node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node)) {
        fprintf(*cntxtAsmFile(cntxt), "mov ");
        rewriteVarAddressToAsmCode(cntxt, *nodeLeft(node));
        fprintf(*cntxtAsmFile(cntxt), ", %s\n", OP_REG_[RIGHT]);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: assign node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    return errorCode;
}


int rewriteOpWhileToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    (*cntxtWhileCounter(cntxt))++;
    int curWhileCntr = *cntxtWhileCounter(cntxt);

    fprintf(*cntxtAsmFile(cntxt), "while%d:\n", curWhileCntr);

    if (*nodeLeft(node)) {
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    fprintf(*cntxtAsmFile(cntxt), "test %s, %s\n", OP_REG_[LEFT], OP_REG_[LEFT]);
    fprintf(*cntxtAsmFile(cntxt), "jz endwhile%d\n", curWhileCntr);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);

    fprintf(*cntxtAsmFile(cntxt), "jmp while%d\n", curWhileCntr);
    fprintf(*cntxtAsmFile(cntxt), "endwhile%d:\n", curWhileCntr);

    return errorCode;
}

int rewriteOpIfToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    (*cntxtIfCounter(cntxt))++;
    int curIfCntr = *cntxtIfCounter(cntxt);

    if (*nodeLeft(node)) {
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    fprintf(*cntxtAsmFile(cntxt), "test %s, %s\n", OP_REG_[LEFT], OP_REG_[LEFT]);
    fprintf(*cntxtAsmFile(cntxt), "jz endif%d\n", curIfCntr);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);

    fprintf(*cntxtAsmFile(cntxt), "endif%d:\n", curIfCntr);

    return errorCode;
}

int rewriteOpInToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    uint32_t pushedRegsMask = pushSavedRegs(cntxt, callerSaved);
    fprintf(*cntxtAsmFile(cntxt), "call stdIn\n");
    popSavedRegs(cntxt, pushedRegsMask);

    fprintf(*cntxtAsmFile(cntxt), "mov ");

    if (*nodeLeft(node))
        errorCode = rewriteVarAddressToAsmCode(cntxt, *nodeLeft(node));
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: in node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    fprintf(*cntxtAsmFile(cntxt), ", rax\n");

    return errorCode;
}


int rewriteOpOutToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    fprintf(*cntxtAsmFile(cntxt), "mov rax, ");
    if (*nodeLeft(node))
        errorCode = rewriteVarAddressToAsmCode(cntxt, *nodeLeft(node));
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                "Error: OUT op node does not have LEFT in func %s, %s:%d\n",
                __func__, __FILE__, __LINE__);
    }

    fprintf(*cntxtAsmFile(cntxt), "\n");

    uint32_t pushedRegsMask = pushSavedRegs(cntxt, callerSaved);
    fprintf(*cntxtAsmFile(cntxt), "call stdOut\n");
    popSavedRegs(cntxt, pushedRegsMask);

    return errorCode;
}


int rewriteOpRetToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);

    freeScopeRegs(cntxt);
    popSavedRegs(cntxt, *curScopePushedRegsMask(*cntxtTree(cntxt)));

    fprintf(*cntxtAsmFile(cntxt), "mov rsp, rbp\n");
    fprintf(*cntxtAsmFile(cntxt), "pop rbp\n");
    fprintf(*cntxtAsmFile(cntxt), "ret\n");

    return errorCode;
}

void freeScopeRegs (backendContext_t* cntxt) {
    assert(cntxt);

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (*regIsUsed(cntxt, (regCode_t)curReg) && (regSaveDecl(cntxt, (regCode_t)curReg) != specialSaved))
            *regIsUsed(cntxt, (regCode_t)curReg) = false;
}

int rewriteOpCompareToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeRight(node)) {
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: compare node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: compare node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    fprintf(*cntxtAsmFile(cntxt), "cmp %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opEQUAL)
        fprintf(*cntxtAsmFile(cntxt), "sete al\n");
    if (nodeValue(node)->opCode == opNOT_EQUAL)
        fprintf(*cntxtAsmFile(cntxt), "setne al\n");
    if (nodeValue(node)->opCode == opBELOW)
        fprintf(*cntxtAsmFile(cntxt), "setl al\n");
    if (nodeValue(node)->opCode == opABOVE)
        fprintf(*cntxtAsmFile(cntxt), "setg al\n");
    if (nodeValue(node)->opCode == opE_BELOW)
        fprintf(*cntxtAsmFile(cntxt), "setle al\n");
    if (nodeValue(node)->opCode == opE_ABOVE)
        fprintf(*cntxtAsmFile(cntxt), "setge al\n");

    fprintf(*cntxtAsmFile(cntxt), "movzx %s, al\n", OP_REG_[resultReg]);

    return errorCode;
}

int rewriteIdNodeToAsmCode(backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    if(*nodeRight(node))
        return rewriteFuncBodyToAsmCode(cntxt, node);

    const char* idName = nodeValue(node)->id.identifierName;

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));
    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return rewriteVarNodeToAsmCode(cntxt, node, resultReg);
    else
        return rewriteFuncCallNodeToAsmCode(cntxt, node, resultReg);
}

int rewriteFuncBodyToAsmCode(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    fprintf(*cntxtAsmFile(cntxt), "%s:\n", nodeValue(node)->id.identifierName);
    fprintf(*cntxtAsmFile(cntxt), "push rbp\n");
    fprintf(*cntxtAsmFile(cntxt), "mov rbp, rsp\n");

    size_t numOfStackVars = (*nodeFuncNumOfLocalVars(node) > NUM_OF_VARS_REGS) ?
                            *nodeFuncNumOfLocalVars(node) - NUM_OF_VARS_REGS : 0;

    fprintf(*cntxtAsmFile(cntxt), "sub rsp, %zu\n", numOfStackVars * 8);

    enterNewScope(*cntxtTree(cntxt));

    *curScopePushedRegsMask(*cntxtTree(cntxt))  = pushSavedRegs(cntxt, calleeSaved);

    if (*nodeLeft(node)) {
        errorCode = fprintfGettingParamsToAsmCode(cntxt, *nodeLeft(node));
        if (errorCode != BACKEND_SUCCESS) return errorCode;
    }

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: func body node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    exitScope(*cntxtTree(cntxt));

    return errorCode;
}

int fprintfGettingParamsToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfGettingParamsToAsmCode(cntxt, *nodeLeft(node));

        if (errorCode != BACKEND_SUCCESS) return errorCode;

        errorCode = fprintfGettingParamsToAsmCode(cntxt, *nodeRight(node));
    }
    else
        addIdToCurrentScope(*cntxtTree(cntxt), nodeVarName(node), idPARAM);

    return errorCode;
}

int rewriteFuncCallNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;
    int numOfFuncParams = 0;

    uint32_t pushedRegsMask = pushSavedRegs(cntxt, callerSaved);

    if (*nodeLeft(node))
        numOfFuncParams = fprintfPassingParams(cntxt, *nodeLeft(node));

    fprintf(*cntxtAsmFile(cntxt), "call %s\n", nodeValue(node)->id.identifierName);

    if (*nodeLeft(node))
        fprintf(*cntxtAsmFile(cntxt), "add rsp, %d\n", numOfFuncParams * 8);

    if (resultReg == RIGHT)
        fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", OP_REG_[RIGHT], OP_REG_[LEFT]);

    popSavedRegs(cntxt, pushedRegsMask);

    return errorCode;
}

int fprintfPassingParams (backendContext_t* cntxt, node_t* node) {

    int curParamsCounter = 0;

    if(*nodeType(node) == typeOperator&& (*nodeValue(node)).opCode == opCOMMA) {
        curParamsCounter += fprintfPassingParams(cntxt, *nodeRight(node));
        curParamsCounter += fprintfPassingParams(cntxt, *nodeLeft(node));
    }
    else {
        rewriteNodeToAsmCode (cntxt, node, LEFT);
        fprintf(*cntxtAsmFile(cntxt), "push %s\n", OP_REG_[LEFT]);
        curParamsCounter++;
    }

    return curParamsCounter;
}

int rewriteVarAddressToAsmCode(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*nodeType(node) != typeIdentifier) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_UNEXPECTED_NODE_TYPE,
                        "Error: node type <%d> is unexpected in func %s, %s:%d\n",
                        *nodeType(node), __func__, __FILE__, __LINE__);
    }

    char* varName = nodeVarName(node);

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));

    identifierInfo* searchedVarId = findIdInTable(curNameTable, varName);

    if(!searchedVarId)
        searchedVarId = addIdToCurrentScope(*cntxtTree(cntxt), varName, idVAR);

    if (*varReg(searchedVarId) == NOT_IN_REG) {           //FIXME NOT_IN_REG??
        int freeRegCode = findFreeReg(cntxt);

        if (freeRegCode != NOT_IN_REG  && *infoIdType(searchedVarId) == idVAR) {
            *varReg(searchedVarId) = freeRegCode;
            *regIsUsed(cntxt, (regCode_t)freeRegCode) = true;
        }
        else {
            if (*varOffset(searchedVarId) == NOT_IN_MEMORY) {
                *numOfTableLocalVars(curNameTable) += 1;
                *varOffset(searchedVarId) = -((*numOfTableLocalVars(curNameTable) + 1) * 8);
            }
            fprintf(*cntxtAsmFile(cntxt), "[rbp + %d]", *varOffset(searchedVarId));
            return BACKEND_SUCCESS;
        }
    }
    fprintf(*cntxtAsmFile(cntxt), "%s", regName(cntxt, (regCode_t)(*varReg(searchedVarId))));

    return BACKEND_SUCCESS;
}

int rewriteVarNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);


    fprintf(*cntxtAsmFile(cntxt), "mov %s, ", OP_REG_[resultReg]);
    rewriteVarAddressToAsmCode(cntxt, node);
    fprintf(*cntxtAsmFile(cntxt), "\n");

    return BACKEND_SUCCESS;
}

int findFreeReg (backendContext_t* cntxt) {
    assert(cntxt);

    for (int curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (!(*regIsUsed(cntxt, (regCode_t)curReg)))
            return curReg;

    return NOT_IN_REG;
}

void fprintfAsmFileHeader (FILE* asmFile) {
    assert(asmFile);

    fprintf(asmFile,
    "section .text\n"
    "global _start\n\n"

    "_start:\n"
    "    call main\n"
    "    call stdExit\n\n");
}

int rewriteOpSqrtToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeLeft(node)) {
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
    } else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                           "Error: SQRT node does not have LEFT in func %s, %s:%d\n",
                           __func__, __FILE__, __LINE__);
    }

    fprintf(*cntxtAsmFile(cntxt), "cvtsi2sd xmm0, %s\n", OP_REG_[LEFT]);
    fprintf(*cntxtAsmFile(cntxt), "sqrtsd xmm0, xmm0\n");
    fprintf(*cntxtAsmFile(cntxt), "cvttsd2si %s, xmm0\n", OP_REG_[resultReg]);

    return errorCode;
}

uint32_t pushSavedRegs (backendContext_t* cntxt, regSaveDecl_t saveDecl) {
    assert(cntxt);

    uint32_t pushedRegsMask = 0;

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++) {
        if (*regIsUsed(cntxt, (regCode_t)curReg) && regSaveDecl(cntxt, (regCode_t)curReg) == saveDecl) {

            fprintf(*cntxtAsmFile(cntxt), "push %s\n", regName(cntxt, (regCode_t)curReg));

            pushedRegsMask |= (1u << curReg);
            *regIsUsed(cntxt, (regCode_t)curReg) = false;
        }
    }

    return pushedRegsMask;
}

void popSavedRegs (backendContext_t* cntxt, uint32_t pushedRegsMask) {
    assert(cntxt);

    for (int curReg = NUM_OF_REGS - 1; curReg >= 0; curReg--) {
        if (pushedRegsMask & (1u << curReg)) {

            fprintf(*cntxtAsmFile(cntxt), "pop %s\n", regName(cntxt, (regCode_t)curReg));

            *regIsUsed(cntxt, (regCode_t)curReg) = true;
        }
    }
}
