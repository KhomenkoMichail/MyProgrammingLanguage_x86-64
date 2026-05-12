#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <elf.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/stdlibHex.h"
#include "../include/instructionsEncoding.h"
#include "../include/backendCntxtFuncs.h"
#include "../include/byteCodeWritingFuncs.h"
#include "../include/asmProgramWriter.h"

int writeElfFile(backendContext_t* cntxt) {
    assert(cntxt);

    FILE* elfFile = fopen(*cntxtElfFileName(cntxt), "wb");
    if (!elfFile) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_OPEN_ELF_FILE,
                        "Error of opening file \"%s\" in func %s, %s:%d\n",
                        *cntxtElfFileName(cntxt), __func__, __FILE__, __LINE__);
    }

    uint64_t entryAddr = 0x400000;
    size_t headerSize = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);

    Elf64_Ehdr ehdr = {
        .e_ident = {
            ELFMAG0,
            ELFMAG1,
            ELFMAG2,
            ELFMAG3,
            ELFCLASS64,
            ELFDATA2LSB,
            EV_CURRENT,
            ELFOSABI_SYSV,
            0, 0, 0, 0, 0, 0, 0, 0
        },
        .e_type = ET_EXEC,
        .e_machine = EM_X86_64,
        .e_version = EV_CURRENT,

        .e_entry = entryAddr + headerSize,
        .e_phoff = sizeof(Elf64_Ehdr),

        .e_ehsize = sizeof(Elf64_Ehdr),
        .e_phentsize = sizeof(Elf64_Phdr),
        .e_phnum = 1,
    };

    Elf64_Phdr phdr = {
        .p_type   = PT_LOAD,
        .p_flags  = PF_R | PF_X,
        .p_offset = 0,
        .p_vaddr  = entryAddr,
        .p_paddr  = entryAddr,
        .p_filesz = headerSize + *cntxtProgramBufSize(cntxt),
        .p_memsz  = headerSize + *cntxtProgramBufSize(cntxt),
        .p_align  = 0x1000,
    };

    fwrite(&ehdr, 1, sizeof(ehdr), elfFile);
    fwrite(&phdr, 1, sizeof(phdr), elfFile);
    fwrite(*cntxtProgramBuf(cntxt), 1, *cntxtProgramBufSize(cntxt), elfFile);

    if (fclose(elfFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", *cntxtElfFileName(cntxt));
        perror("");
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_CLOSING_ELF_FILE,
                           "Error of closing file \"%s\" in func %s, %s:%d\n",
                           *cntxtElfFileName(cntxt), __func__, __FILE__, __LINE__);
    }

    return BACKEND_SUCCESS;
}

int executeBuffer (backendContext_t* cntxt) {
    assert(cntxt);

    size_t pageSize = sysconf(_SC_PAGESIZE);

    uintptr_t startAddr = (uintptr_t)(*cntxtProgramBuf(cntxt));
    uintptr_t pageAlignedAddr = startAddr & ~(pageSize - 1);

    size_t offset = startAddr - pageAlignedAddr;
    size_t protectSize = *cntxtProgramBufSize(cntxt) + offset;

    if (mprotect((void*)pageAlignedAddr, protectSize, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect failed");
        return -1;
    }

    bufFunc_t bufCode = (bufFunc_t)(*cntxtProgramBuf(cntxt));
    int result = bufCode();

    printf("bufCode returned: %d\n", result);

    if (mprotect((void*)pageAlignedAddr, protectSize, PROT_READ | PROT_WRITE) == -1) {
        perror("mprotect restore failed");
    }

    return result;
}

int astToByteCode (backendContext_t* cntxt) {   //FIXME
    assert(cntxt);

    addStdLibInBuffer(cntxt);
    nodeToByteCode(cntxt, *treeRoot(*cntxtTree(cntxt)), LEFT);
    return writeElfFile(cntxt);
}

int nodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(node);
    assert(cntxt);

    //emitBreakpoint(cntxt, node);

    switch (*nodeType(node)) {
        case typeNumber:
            return emitMov(cntxt, REG_CONST, (regCode_t)resultReg, NO_REG, nodeValue(node)->constValue); //MOV_RC(resultReg, nodeValue(node)->constValue)
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

        case opASSIGN: return opAssignToByteCode(cntxt, node);
        case opWHILE: return opWhileToByteCode(cntxt, node);
        case opIF: return opIfToByteCode(cntxt, node);
        case opIN: return opInToByteCode(cntxt, node);
        case opOUT: return opOutToByteCode(cntxt, node);
        case opRET: return opRetToByteCode(cntxt, node);
        case opSQRT: return opSqrtToByteCode(cntxt, node, resultReg);

        case opHLT: emitByte(cntxt, opCodeJMP);    // JMP_("stdExit"); return *cntxtErrCode(cntxt);
        return patchCurLabel(cntxt, "stdExit");

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

    if (*nodeLeft(node))
        nodeToByteCode (cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: calc node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeRight(node)) {
        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            emitPushReg(cntxt, (regCode_t)LEFT);

        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            emitPopReg(cntxt, (regCode_t)LEFT);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: calc node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    switch (*nodeOpCode(node)) {
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
        emitMov(cntxt, REG_REG, (regCode_t)RIGHT, (regCode_t)LEFT, NO_DISP);

    return *cntxtErrCode(cntxt);
}

int opAssignToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*nodeRight(node))
        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: assign node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, *nodeLeft(node));
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

    return *cntxtErrCode(cntxt);
}


int opWhileToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    (*cntxtWhileCounter(cntxt))++;

    size_t whileStartBufPos = *cntxtProgramBufSize(cntxt);        //while:

    if (*nodeLeft(node)) {
        nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);
    }

    emitOpRegReg(cntxt, opCodeTEST, (regCode_t)LEFT, (regCode_t)LEFT);    //test rax rax

    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeJZ);                                      //jz 0000
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt);
    emit_32CurPos(cntxt, 0x00);

    if (*nodeRight(node))
        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);

    emitJMPorCALL(cntxt, false, opCodeJMP, whileStartBufPos);   //JMP while

    int32_t jzOffset = (int32_t)(*cntxtProgramBufSize(cntxt) - jzOffsetBufPos - OFFSET_LEN);
    emit_32givenPos(cntxt, jzOffsetBufPos, (uint32_t)jzOffset);

    return *cntxtErrCode(cntxt);
}

int opIfToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    (*cntxtIfCounter(cntxt))++;

    if (*nodeLeft(node)) {
        nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);
    }

    emitOpRegReg(cntxt, opCodeTEST, (regCode_t)LEFT, (regCode_t)LEFT);

    emitByte(cntxt, ESCAPE_PREFIX);
    emitByte(cntxt, opCodeJZ);
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt);
    emit_32CurPos(cntxt, 0x00);

    if (*nodeRight(node))
        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);

    int32_t jzOffset = (int32_t)(*cntxtProgramBufSize(cntxt) - jzOffsetBufPos - OFFSET_LEN);
    emit_32givenPos(cntxt, jzOffsetBufPos, (uint32_t)jzOffset);

    return *cntxtErrCode(cntxt);
}

int opInToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);
    //fprintf(*cntxtByteFile(cntxt), "call stdIn\n"); //FIXME
    emitByte(cntxt, opCodeCALL);
    patchCurLabel(cntxt, "stdIn");
    popSavedRegsByteCode(cntxt, pushedRegsMask);

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, *nodeLeft(node));
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

    return *cntxtErrCode(cntxt);
}


int opOutToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, *nodeLeft(node));
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
    emitByte(cntxt, opCodeCALL);
    patchCurLabel(cntxt, "stdOut");
    popSavedRegsByteCode(cntxt, pushedRegsMask);

    return *cntxtErrCode(cntxt);
}


int opRetToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if (*nodeLeft(node))
        nodeToByteCode(cntxt, *nodeLeft(node), LEFT);

    freeScopeRegs(cntxt);
    popSavedRegsByteCode(cntxt, *curScopePushedRegsMask(*cntxtTree(cntxt)));

    emitMov(cntxt, REG_REG, RSP, RBP, NO_DISP);
    emitPopReg(cntxt, RBP);
    emitRet(cntxt);

    return *cntxtErrCode(cntxt);
}

int opCompareToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    if (*nodeRight(node)) {
        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: compare node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node))
        nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: compare node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    emitOpRegReg(cntxt, opCodeCMP, (regCode_t)LEFT, (regCode_t)RIGHT);

    switch (*nodeOpCode(node)) {
        case opEQUAL:
            emitSETcc(cntxt, opCodeSETe, RAX);
            break;
        case opNOT_EQUAL:
            emitSETcc(cntxt, opCodeSETne, RAX);
            break;
        case opBELOW:
            emitSETcc(cntxt, opCodeSETl, RAX);
            break;
        case opABOVE:
            emitSETcc(cntxt, opCodeSETg, RAX);
            break;
        case opE_BELOW:
            emitSETcc(cntxt, opCodeSETle, RAX);
            break;
        case opE_ABOVE:
            emitSETcc(cntxt, opCodeSETge, RAX);
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

    return *cntxtErrCode(cntxt);
}

int idNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    if(*nodeRight(node))
        return funcBodyToByteCode(cntxt, node);

    const char* idName = *nodeIdentifierName(node);

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));
    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return varNodeToByteCode(cntxt, node, resultReg);
    else
        return funcCallNodeToByteCode(cntxt, node, resultReg);
}

int funcBodyToByteCode(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    addLabelAddressInCntxt(cntxt, *nodeIdentifierName(node));
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    emitPushReg(cntxt, RBP);
    emitMov(cntxt, REG_REG, RBP, RSP, NO_DISP);

    size_t numOfStackVars = (*nodeFuncNumOfLocalVars(node) > NUM_OF_VARS_REGS) ?
                            *nodeFuncNumOfLocalVars(node) - NUM_OF_VARS_REGS : 0;

    //fprintf(*cntxtByteFile(cntxt), "sub rsp, %zu\n", numOfStackVars * 8);
    emitAluRegConst(cntxt, RSP, (int32_t)numOfStackVars * 8, aluSUB);

    enterNewScope(*cntxtTree(cntxt));

    *curScopePushedRegsMask(*cntxtTree(cntxt)) = pushSavedRegsByteCode(cntxt, calleeSaved);

    if (*nodeLeft(node)) {
        gettingParamsToByteCode(cntxt, *nodeLeft(node));
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);
    }

    if (*nodeRight(node))
        nodeToByteCode(cntxt, *nodeRight(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: func body node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    exitScope(*cntxtTree(cntxt));

    return *cntxtErrCode(cntxt);
}

int gettingParamsToByteCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    if(*nodeLeft(node) && *nodeRight(node)) {
        gettingParamsToByteCode(cntxt, *nodeLeft(node));

        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

        gettingParamsToByteCode(cntxt, *nodeRight(node));
    }
    else
        addIdToCurrentScope(*cntxtTree(cntxt), nodeVarName(node), idPARAM);

    return *cntxtErrCode(cntxt);
}

int funcCallNodeToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int numOfFuncParams = 0;

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);

    if (*nodeLeft(node))
        numOfFuncParams = passingParamsToByteCode(cntxt, *nodeLeft(node));

    emitByte(cntxt, opCodeCALL);
    patchCurLabel(cntxt, *nodeIdentifierName(node));
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);
    //fprintf(*cntxtByteFile(cntxt), "call %s\n", *nodeIdentifierName(node));

    if (*nodeLeft(node))
        emitAluRegConst(cntxt, RSP, numOfFuncParams * 8, aluADD);
        //fprintf(*cntxtByteFile(cntxt), "add rsp, %d\n", numOfFuncParams * 8);

    if (resultReg == RIGHT)
        emitMov(cntxt, REG_REG, (regCode_t)RIGHT, (regCode_t)LEFT, NO_DISP);

    popSavedRegsByteCode(cntxt, pushedRegsMask);

    return *cntxtErrCode(cntxt);
}

int passingParamsToByteCode (backendContext_t* cntxt, node_t* node) {

    int curParamsCounter = 0;

    if(*nodeType(node) == typeOperator && (*nodeValue(node)).opCode == opCOMMA) {
        curParamsCounter += passingParamsToByteCode(cntxt, *nodeRight(node));
        curParamsCounter += passingParamsToByteCode(cntxt, *nodeLeft(node));
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
            }
                *varPosRbpOffset(&varPos) = *varOffset(searchedVarId);
                return varPos;
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
        emitMov(cntxt, REG_REG, (regCode_t)resultReg, *varPosRegCode(&varPos), NO_DISP);
    else
        emitMov(cntxt, REG_MEM, (regCode_t)resultReg, RBP, *varPosRbpOffset(&varPos));

    return *cntxtErrCode(cntxt);
}

int opSqrtToByteCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    if (*nodeLeft(node))
        nodeToByteCode(cntxt, *nodeLeft(node), LEFT);
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

    return *cntxtErrCode(cntxt);
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

int popSavedRegsByteCode (backendContext_t* cntxt, uint32_t pushedRegsMask) {
    assert(cntxt);

    for (int curReg = NUM_OF_REGS - 1; curReg >= 0; curReg--) {
        if (pushedRegsMask & (1u << curReg)) {

            emitPopReg(cntxt, (regCode_t)curReg);

            *regIsUsed(cntxt, (regCode_t)curReg) = true;
        }
    }

    return *cntxtErrCode(cntxt);
}

int addStdLibInBuffer (backendContext_t* cntxt) {
    assert(cntxt);

    emitByte(cntxt, opCodeJMP);
    patchCurLabel(cntxt, "main");
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdExit");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDEXIT_OFFSET);
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdIn");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDIN_OFFSET);
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdOut");
    emitByte(cntxt, opCodeJMP);
    emit_32CurPos(cntxt, STDOUT_OFFSET);
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    if (*cntxtProgramBufSize(cntxt) + STDLIB_SIZE > *cntxtProgramBufCapacity(cntxt)) {
        uint8_t* newBuf = (uint8_t*)realloc(*cntxtProgramBuf(cntxt), sizeof(uint8_t) * STDLIB_SIZE * 2);
        if (!newBuf) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_REALLOC,
                        "Error realloc program buffer in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }
        *cntxtProgramBuf(cntxt) = newBuf;
    }

    memcpy(*cntxtProgramBuf(cntxt) + *cntxtProgramBufSize(cntxt), STDLIB_CODE, STDLIB_SIZE);
    *cntxtProgramBufSize(cntxt) += STDLIB_SIZE;

    return BACKEND_SUCCESS;
}
