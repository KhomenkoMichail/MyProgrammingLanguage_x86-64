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
#include "../include/backendDSL.h"

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

    //emitBreakpoint(cntxt, node);         //FIXME

    switch (*nodeType(node)) {
        case typeNumber:
            return MOV_RC_((regCode_t)resultReg, *nodeConstValue(node));
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

        case opHLT:
            JMP_PATCH_("stdExit");
            return *cntxtErrCode(cntxt);


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
            PUSHR_((regCode_t)LEFT);

        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);
        if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

        if (*nodeLeft(*nodeRight(node)) && (*nodeValue(node)).opCode != opSEPARATOR
                                        && (*nodeValue(node)).opCode != opCOMMA)
            POPR_((regCode_t)LEFT);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: calc node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    switch (*nodeOpCode(node)) {
        case (opADD):
            ADD_RR_((regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opSUB):
            SUB_RR_((regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opMUL):
            IMUL_RR_((regCode_t)LEFT, (regCode_t)RIGHT);
            break;
        case (opDIV):
            if (*regIsUsed(cntxt, RDX))
                PUSHR_(RDX);

            CQO;
            IDIV_((regCode_t)RIGHT);

            if (*regIsUsed(cntxt, RDX))
                POPR_(RDX);
        default:
            break;
    }

    if (resultReg == RIGHT)
        MOV_RR_((regCode_t)RIGHT, (regCode_t)LEFT);

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
            MOV_RR_(*varPosRegCode(&varPos), (regCode_t)RIGHT);
        else
            MOV_MR_(RBP, *varPosRbpOffset(&varPos), (regCode_t)RIGHT);
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

    TEST_RR_((regCode_t)LEFT, (regCode_t)LEFT);

    JZ_(0x00);
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt) - OFFSET_LEN;   //FIXME

    if (*nodeRight(node))
        nodeToByteCode(cntxt, *nodeRight(node), RIGHT);

    //emitJMPorCALL(cntxt, false, opCodeJMP, whileStartBufPos);   //JMP while
    int32_t whileOffset = (int32_t)(whileStartBufPos - *cntxtProgramBufSize(cntxt));
    JMP_OFFSET_(whileOffset);

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

    TEST_RR_((regCode_t)LEFT, (regCode_t)LEFT);

    JZ_(0x00);
    size_t jzOffsetBufPos = *cntxtProgramBufSize(cntxt) - OFFSET_LEN;   //FIXME

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

    CALL_("stdIn");

    popSavedRegsByteCode(cntxt, pushedRegsMask);

    if (*nodeLeft(node)) {
        varPos_t varPos = getVarPos(cntxt, *nodeLeft(node));
        if (*varPosInReg(&varPos))
            MOV_RR_(*varPosRegCode(&varPos), RAX);
        else
            MOV_MR_(RBP, *varPosRbpOffset(&varPos), RAX);
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
            MOV_RR_(RAX, *varPosRegCode(&varPos));
        else
            MOV_RM_(RAX, RBP, *varPosRbpOffset(&varPos));
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                "Error: OUT op node does not have LEFT in func %s, %s:%d\n",
                __func__, __FILE__, __LINE__);
    }

    uint32_t pushedRegsMask = pushSavedRegsByteCode(cntxt, callerSaved);

    CALL_("stdOut");

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

    MOV_RR_(RSP, RBP);
    POPR_(RBP);
    RET;

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

    CMP_RR_((regCode_t)LEFT, (regCode_t)RIGHT);

    switch (*nodeOpCode(node)) {
        case opEQUAL:     SETE_(RAX);  break;
        case opNOT_EQUAL: SETNE_(RAX); break;
        case opBELOW:     SETL_(RAX);  break;
        case opABOVE:     SETG_(RAX);  break;
        case opE_BELOW:   SETE_(RAX);  break;
        case opE_ABOVE:   SETGE_(RAX); break;
        default:
            break;
    }

    MOVZX_RR_((regCode_t)resultReg, RAX);

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

    PUSHR_(RBP);
    MOV_RR_(RBP, RSP);

    size_t numOfStackVars = (*nodeFuncNumOfLocalVars(node) > NUM_OF_VARS_REGS) ?
                            *nodeFuncNumOfLocalVars(node) - NUM_OF_VARS_REGS : 0;

    if (numOfStackVars)
        SUB_RC_(RSP, (int32_t)numOfStackVars * 8);

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

    CALL_(*nodeIdentifierName(node));

    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    if (*nodeLeft(node))
        ADD_RC_(RSP, numOfFuncParams * 8);

    if (resultReg == RIGHT)
        MOV_RR_((regCode_t)RIGHT, (regCode_t)LEFT);

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
        PUSHR_((regCode_t)LEFT);
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
        MOV_RR_((regCode_t)resultReg, *varPosRegCode(&varPos));
    else
        MOV_RM_((regCode_t)resultReg, RBP, *varPosRbpOffset(&varPos));

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

    CVTSI2SD_(XMM0_CODE, (regCode_t)LEFT);
    SQRTSD_(XMM0_CODE, XMM0_CODE);
    CVTTSD2SI_((regCode_t)resultReg, XMM0_CODE);

    return *cntxtErrCode(cntxt);
}

uint32_t pushSavedRegsByteCode (backendContext_t* cntxt, regSaveDecl_t saveDecl) {
    assert(cntxt);

    uint32_t pushedRegsMask = 0;

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++) {
        if (*regIsUsed(cntxt, (regCode_t)curReg) && regSaveDecl(cntxt, (regCode_t)curReg) == saveDecl) {

            PUSHR_((regCode_t)curReg);

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

            POPR_((regCode_t)curReg);

            *regIsUsed(cntxt, (regCode_t)curReg) = true;
        }
    }

    return *cntxtErrCode(cntxt);
}

int addStdLibInBuffer (backendContext_t* cntxt) {
    assert(cntxt);

    JMP_PATCH_("main");
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdExit");
    JMP_OFFSET_(STDEXIT_OFFSET);
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdIn");
    JMP_OFFSET_(STDIN_OFFSET);
    if (*cntxtErrCode(cntxt)) return *cntxtErrCode(cntxt);

    addLabelAddressInCntxt(cntxt, "stdOut");
    JMP_OFFSET_(STDOUT_OFFSET);
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
