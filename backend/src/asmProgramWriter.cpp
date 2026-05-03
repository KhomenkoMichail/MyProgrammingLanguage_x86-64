#include <stdio.h>
#include <assert.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/sourceFileParser.h"
#include "../include/asmProgramWriter.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"

int rewriteAstToAsmCode (backendContext_t* cntxt) {
    assert(cntxt);

    *cntxtAsmFile(cntxt) = fopen(nameOfAsmFile, "w");

    if (!(*cntxtAsmFile(cntxt))) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfAsmFile);
        perror("");
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_OPENING_ASM_FILE,
                        "Error of opening file \"%s\" in func %s, %s:%d\n",
                        nameOfAsmFile, __func__, __FILE__, __LINE__);
    }

    rewriteNodeToAsmCode(cntxt, *treeRoot(*cntxtTree(cntxt)), LEFT);

    if (fclose(asmFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfAsmFile);
        perror("");
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_OPENING_ASM_FILE,
                        "Error of closing file \"%s\" in func %s, %s:%d\n",
                        nameOfAsmFile, __func__, __FILE__, __LINE__);
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
            return rewriteIdNodeToAsmCode(cntxt, node, resultReg, asmFile);
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

        case opASSIGN: return rewriteOpAssignToAsmCode(cntxt, node, resultReg);
        case opWHILE: return rewriteOpWhileToAsmCode(cntxt, node, resultReg);
        case opIF: return rewriteOpIfToAsmCode(cntxt, node, resultReg);
        case opIN: return rewriteOpInToAsmCode(cntxt, node, resultReg);   //FIXME
        case opOUT: return rewriteOpOutToAsmCode(cntxt, node, resultReg); //FIXME
        case opRET: return rewriteOpRetToAsmCode(cntxt, node, resultReg);

        case opHLT: fprintf (*cntxtAsmFile(cntxt), "HLT\n"); return BACKEND_SUCCESS; //FIXME

        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return rewriteOpCompareToAsmCode(cntxt, node, resultReg);

        case opSQRT:       //FIXME
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

    if (*nodeRight(node)) {
        errorCode = rewriteNodeToAsmCode (cntxt, *nodeRight(node), RIGHT);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: calc node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (cntxt, *nodeLeft(node), LEFT);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_NO_LEFT_NODE,
                        "Error: calc node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (nodeValue(node)->opCode == opADD)
        fprintf(*cntxtAsmFile(cntxt), "add %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opSUB)
        fprintf(*cntxtAsmFile(cntxt), "sub %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opMUL)
        fprintf(*cntxtAsmFile(cntxt), "imul %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    if (nodeValue(node)->opCode == opDIV) {
        if (*regIsUsed(regsArray, RDX))
            fprintf(*cntxtAsmFile(cntxt), "push rdx\n");

        fprintf(*cntxtAsmFile(cntxt), "mov rax, %s\n", OP_REG_[LEFT]);
        fprintf(*cntxtAsmFile(cntxt), "cqo\n");
        fprintf(*cntxtAsmFile(cntxt), "idiv %s\n", OP_REG_[RIGHT]);
        fprintf(*cntxtAsmFile(cntxt), "mov %s, rax\n", OP_REG_[LEFT]);

        if (*regIsUsed(regsArray, RDX))
            fprintf(*cntxtAsmFile(cntxt), "pop rdx\n");
    }

    if (resultReg == RIGHT)
        fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", OP_REG_[RIGHT], OP_REG_[LEFT]);

    return errorCode;
}

int rewriteOpAssignToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCESS;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), OP_REG_[RIGHT]);
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_RIGHT_NODE,
                        "Error: assign node does not have RIGHT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (*nodeLeft(node)) {
        fprintf(*cntxtAsmFile(cntxt), "mov ");
        rewriteVarAddressToAsmCode(cntxt, node);
        fprintf(*cntxtAsmFile(cntxt), ", %s\n", OP_REG_[RIGHT]);
    }
    else {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_NO_LEFT_NODE,
                        "Error: assign node does not have LEFT in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    if (resultReg == LEFT)
        fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", OP_REG_[LEFT], OP_REG_[RIGHT]);

    return errorCode;
}


int rewriteOpWhileToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCEESS;

    (*cntxtWhileCounter(cntxt))++;

    fprintf(*cntxtAsmFile(cntxt), "while%d:\n", *cntxtWhileCounter(cntxt));

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);

    fprintf(*cntxtAsmFile(cntxt), "test %s, %s\n", OP_REG_[LEFT], OP_REG_[LEFT]);
    fprintf(*cntxtAsmFile(cntxt), "jz endwhile%d\n", *cntxtWhileCounter(cntxt));

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);

    fprintf(*cntxtAsmFile(cntxt), "jmp while%d\n", *cntxtWhileCounter(cntxt));
    fprintf(*cntxtAsmFile(cntxt), "endwhile%d\n", *cntxtWhileCounter(cntxt));

    return errorCode;
}

int rewriteOpIfToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCEESS;

    (*cntxtIfCounter(cntxt))++;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);

    fprintf(*cntxtAsmFile(cntxt), "test %s, %s\n", OP_REG_[LEFT], OP_REG_[LEFT]);
    fprintf(*cntxtAsmFile(cntxt), "jz endif%d\n", *cntxtIfCounter(cntxt));

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);

    fprintf(*cntxtAsmFile(cntxt), "endif%d:\n", *cntxtIfCounter(cntxt));

    return errorCode;
}

int rewriteOpInToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);


    return errorCode;
}


int rewriteOpOutToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);


    return errorCode;
}

int rewriteOpRetToAsmCode (backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);

    int errorCode = BACKEND_SUCCEESS;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);

    nameTable_t* curNameTable = getCurNameTable(*cntxtTree(cntxt));
    fprintf(*cntxtAsmFile(cntxt), "add %s, %d\n", STACK_POINTER_REG, *numOfTableLocalVars(curNameTable) * 8);

    freeScopeRegs(regsArray);
    popSavedRegs(asmFile);
    fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", STACK_POINTER_REG, BASE_POINTER_REG);
    fprintf(*cntxtAsmFile(cntxt), "pop %s\n", BASE_POINTER_REG);
    fprintf(*cntxtAsmFile(cntxt), "ret\n");

    return errorCode;
}

void freeScopeRegs (regInfo_t regsArray) {
    for (int curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (*regIsUsed(regsArray, curReg) && !(*regWasPushed(regsArray, curReg)))
            *regIsUsed(regsArray, curReg) = false;
}

int rewriteOpCompareToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);



    int errorCode = BACKEND_SUCCEESS;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), RIGHT);
    else {
        printf("Error! Compare node does not have RIGHT.\n");
        return 1;
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeLeft(node), LEFT);
    else {
        printf("Error! Compare node does not have LEFT.\n");
        return 1;
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
    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    nameTable_t* curNameTable = (tree->nameTableStack->data)[numOfCurNameTable];

    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return rewriteVarNodeToAsmCode(cntxt, node, resultReg, asmFile);
    else
        return rewriteFuncCallNodeToAsmCode(cntxt, node, asmFile, resultReg, srcFile);
}

int rewriteFuncBodyToAsmCode(backendContext_t* cntxt, node_t* node) {
    assert(cntxt);
    assert(node);



    int errorCode = BACKEND_SUCCEESS;

    fprintf(*cntxtAsmFile(cntxt), "%s:\n", nodeValue(node)->id.identifierName);
    fprintf(*cntxtAsmFile(cntxt), "push %s\n", BASE_POINTER_REG);
    fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", BASE_POINTER_REG, STACK_POINTER_REG);


    enterNewScope(tree);

    nameTable_t* curNameTable = getCurNameTable(tree);
    *numOfFrameCaleeSavedRegs(curNameTable)  = pushSavedRegs(calleeSaved, asmFile);

    if (*nodeLeft(node))
        errorCode = fprintfGettingParamsToAsmCode(cntxt, *nodeLeft(node), asmFile);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(cntxt, *nodeRight(node), LEFT);
    else {
        printf("Error! Func body does not have RIGHT!\n");
        return 1;
    }

    exitScope(tree);

    return errorCode;
}

int fprintfGettingParamsToAsmCode (backendContext_t* cntxt, node_t* node, FILE* asmFile) {
    assert(cntxt);
    assert(node);


    int errorCode = BACKEND_SUCCEESS;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfGettingParamsToAsmCode (cntxt, *nodeLeft(node), asmFile);
        errorCode = fprintfGettingParamsToAsmCode (cntxt, *nodeRight(node), asmFile);
    }
    else
        addIdToCurrentScope(tree, nodeVarName(node), idPARAM);

    return errorCode;
}

int rewriteFuncCallNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg) {
    assert(cntxt);
    assert(node);



    int errorCode = BACKEND_SUCCEESS;
    int numOfFuncParams = 0;

    pushSavedRegs(callerSaved, asmFile);

    if (*nodeLeft(node))
        numOfFuncParams = fprintfPassingParams (cntxt, *nodeLeft(node));

    fprintf(*cntxtAsmFile(cntxt), "call %s\n", nodeValue(node)->id.identifierName);

    if (*nodeLeft(node))
        fprintf(*cntxtAsmFile(cntxt), "add %s, %d", STACK_POINTER_REG, numOfFuncParams * 8);

    if (resultReg == RIGHT)
        fprintf(*cntxtAsmFile(cntxt), "mov %s, %s\n", OP_REG_[RIGHT], OP_REG_[LEFT]);

    popSavedRegs(asmFile);

    return errorCode;
}

int fprintfPassingParams (backendContext_t* cntxt, node_t* node) {

    int curParamsCounter = 0;

    if(*nodeType(node) == typeOperator&& (*nodeValue(node)).opCode == opCOMMA) {
        curParamsCounter += fprintfPassingParams (cntxt, *nodeRight(node));
        curParamsCounter += fprintfPassingParams (cntxt, *nodeLeft(node));
    }
    else {
        rewriteNodeToAsmCode (cntxt, node, LEFT);
        fprintf(*cntxtAsmFile(cntxt), "push %s\n", OP_REG_[LEFT]);
        curParamsCounter++;
    }

    return curParamsCounter;
}

int pushSavedRegs (regSaveDecl_t saveDecl, FILE* asmFile) {
    int savedRegsCntr = 0;

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (*regIsUsed(regsArray, curReg) && regSaveDecl(regsArray, curReg) == saveDecl) {
            fprintf(*cntxtAsmFile(cntxt), "push %s\n", regName(regsArray, curReg));
            *regIsUsed(regsArray, curReg) = false;
            *regWasPushed(regsArray, curReg) = true;
            savedRegsCntr++;
        }

    return savedRegsCntr;
}

void popSavedRegs (FILE* asmFile) {
    for (int curReg = NUM_OF_REGS - 1; curReg >= 0; curReg--)
        if (*regWasPushed(regsArray, curReg)) {
            fprintf(*cntxtAsmFile(cntxt), "pop %s\n", regName(regsArray, curReg));
            *regIsUsed(regsArray, curReg) = true;
            *regWasPushed(regsArray, curReg) = false;
        }
}

void rewriteVarAddressToAsmCode (backendContext_t* cntxt, node_t* node, FILE* asmFile) {
    assert(cntxt);
    assert(node);


    int errorCode = BACKEND_SUCCEESS;

    if (*nodeType(node) != typeIdentifier) {
        printf("Error! In func %s unexpected type of node.\n", __func__);
        return;
    }

    char* varName = nodeVarName(node);

    nameTable_t* curNameTable = getCurNameTable(tree);

    identifierInfo* searchedVarId = findIdInTable(curNameTable, varName);

    if(!searchedVarId)
        searchedVarId = addIdToCurrentScope(tree, varName, idVAR);

    if (*varReg(searchedVarId) == NOT_IN_REG) {
        int freeRegCode = findFreeReg(regsArray);

        if (freeRegCode != NOT_IN_REG) {
            *varReg(searchedVarId) = freeRegCode;
            *regIsUsed(regsArray, freeRegCode) = true;
        }
        else {
            if (*varOffset(searchedVarId) == NOT_IN_MEMORY) {
                *numOfTableLocalVars(curNameTable) += 1;
                *varOffset(searchedVarId) = -((*numOfTableLocalVars(curNameTable) +
                                               *numOfFrameCaleeSavedRegs(curNameTable)) * 8);
            }
            fprintf(*cntxtAsmFile(cntxt), "[%s + %d]", BASE_POINTER_REG, *varOffset(searchedVarId));
            return;
        }
    }
    fprintf(*cntxtAsmFile(cntxt), "%s", regName(regsArray, *varReg(searchedVarId)));

    return;
}

void rewriteVarNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg, FILE* asmFile) {
    assert(cntxt);
    assert(node);


    fprintf(*cntxtAsmFile(cntxt), "mov %s, ", OP_REG_[resultReg]);
    rewriteVarAddressToAsmCode (cntxt, node, asmFile);
    fprintf(*cntxtAsmFile(cntxt), "\n");
}

int findFreeReg (regInfo_t* regsArray) {
    assert(regsArray);

    for (int curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (!(*regIsUsed(regsArray, curReg)))
            return curReg;

    return NOT_IN_REG;
}
