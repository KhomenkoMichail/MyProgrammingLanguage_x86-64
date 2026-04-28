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

int rewriteAstToAsmCode (tree_t* tree, const char* nameOfAsmFile, const char* nameOfSourceFile) {
    assert(tree);
    assert(nameOfAsmFile);

    FILE* asmFile = fopen(nameOfAsmFile, "w");

    if (!asmFile) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfAsmFile);
        perror("");
        return 1;
    }

    sourceFile* srcFile = (sourceFile*)calloc(1, sizeof(sourceFile));
    getStructSourceFile(srcFile, nameOfSourceFile);
    srcFile->fileName = nameOfSourceFile;

    initNameTables(tree);

    if (rewriteNodeToAsmCode (tree, *treeRoot(tree), asmFile, srcFile))
        printf("Error of rewriting AST to Asm code.\n");

    destroyNameTables(tree);
    freeStructSourceFile(srcFile);

    if (fclose(asmFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfAsmFile);
        perror("");
        return 1;
    }

    return 0;
}

int rewriteNodeToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(node);
    assert(tree);
    assert(srcFile);
    assert(asmFile);

    fprintfCommentsToAsm(node, srcFile, asmFile);

    switch (*nodeType(node)) {
        case typeNumber:
            fprintf(asmFile, "mov %s, %d\n", OP_REG[resultReg], nodeValue(node)->constValue);
            return 0;
        case typeOperator:
            return rewriteOpNodeToAsmCode(tree, node, resultReg, asmFile, srcFile);
        case typeIdentifier:
            return rewriteIdNodeToAsmCode(tree, node, resultReg, asmFile, srcFile);
        case typeError:
        default:
            return 1;
    }
}

int rewriteOpNodeToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    switch ((*nodeValue(node)).opCode) {

        case opADD:
        case opSUB:
        case opMUL:
        case opDIV:
        case opSEPARATOR:
        case opCOMMA:
            return rewriteOpCalcToAsmCode(tree, node, resultReg, asmFile, srcFile);

        case opASSIGN: return rewriteOpAssignToAsmCode (tree, node, resultReg, asmFile, srcFile);
        case opWHILE: return rewriteOpWhileToAsmCode (tree, node, resultReg, asmFile, srcFile);
        case opIF: return rewriteOpIfToAsmCode (tree, node, asmFile, resultReg, srcFile);
        case opIN: return rewriteOpInToAsmCode (tree, node, resultReg, asmFile);
        case opOUT: return rewriteOpOutToAsmCode (tree, node, resultReg, asmFile);
        case opRET: return rewriteOpRetToAsmCode (tree, node, resultReg, asmFile, srcFile);

        case opHLT: fprintf (asmFile, "HLT\n"); return 0; //FIXME

        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return rewriteOpCompareToAsmCode(tree, node, resultReg, asmFile, srcFile);

        case opSQRT:       //FIXME
        case opUNITED_ON:
        case opUNITED_OFF:
        case opQUOTES:
        case opINIT:
        case opBRACK_ON:
        case opBRACK_OFF:
        case opUNKNOWN:
        default:
            printf("Error! Unexpected operator opCode:{%d}\n", (*nodeValue(node)).opCode);
            return 1;
    }

    return 0;
}

int rewriteOpCalcToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(node);
    assert(tree);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeRight(node)) {
        errorCode = rewriteNodeToAsmCode (tree, *nodeRight(node), resultReg, asmFile, srcFile);
    }
    else {
        printf("Error! Calc node does not have RIGHT.\n");
        return 1;
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (tree, *nodeLeft(node), asmFile, srcFile);
    else {
        printf("Error! Calc node does not have LEFT.\n");
        return 1;
    }

    if (nodeValue(node)->opCode == opADD)
        fprintf(asmFile, "add %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    if (nodeValue(node)->opCode == opSUB)
        fprintf(asmFile, "sub %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    if (nodeValue(node)->opCode == opMUL)
        fprintf(asmFile, "mul %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    if (nodeValue(node)->opCode == opDIV)
        fprintf(asmFile, "div %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    if (resultReg == RIGHT)
        fprintf(asmFile, "mov %s, %s\n", OP_REG[RIGHT], OP_REG[LEFT]);

    return errorCode;
}

int rewriteOpAssignToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(asmFile);
    assert(srcFile);

    int errorCode = 0;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), OP_REG[RIGHT], asmFile, srcFile);
    else {
        printf("Error! Assign node does not have RIGHT.\n");
        return 1;
    }

    if (*nodeLeft(node)) {
        fprintf (asmFile, "mov ");
        rewriteVarAddressToAsmCode (tree, node, asmFile);
        fprintf(asmFile, ", %s\n", OP_REG[RIGHT]);
    }
    else {
        printf("Error! Assign node does not have LEFT.\n");
        return 1;
    }

    if (resultReg == LEFT)
        fprintf(asmFile, "mov %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    return errorCode;
}


int rewriteOpWhileToAsmCode (tree_t* tree, node_t* node, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;

    static int whileCounter = 0;

    whileCounter++;
    int curWhileNum = whileCounter;

    fprintf(asmFile, ":while%d\n", curWhileNum);

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), LEFT, asmFile, srcFile);

    fprintf(asmFile, "test %s, %s\n", OP_REG[LEFT], OP_REG[LEFT]);
    fprintf(asmFile, "jz :endwhile%d\n", curWhileNum);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), RIGHT, asmFile, srcFile);

    fprintf(asmFile, "jmp :while%d\n", curWhileNum);
    fprintf(asmFile, ":endwhile%d\n", curWhileNum);

    return errorCode;
}

int rewriteOpIfToAsmCode (tree_t* tree, node_t* node, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;

    static int ifCounter = 0;
    ifCounter++;

    int curIfNum = ifCounter;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), LEFT, asmFile, srcFile);

    fprintf(asmFile, "test %s, %s\n", OP_REG[LEFT], OP_REG[LEFT]);
    fprintf(asmFile, "jz :endif%d\n", curIfNum);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), RIGHT, asmFile, srcFile);

    fprintf(asmFile, ":endif%d\n", curIfNum);

    return errorCode;
}

int rewriteOpInToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    return errorCode;
}


int rewriteOpOutToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    return errorCode;
}

int rewriteOpRetToAsmCode (tree_t* tree, node_t* node, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), LEFT, asmFile, srcFile);

    popSavedRegs(asmFile);
    fprintf(asmFile, "mov %s, %s\n", STACK_POINTER_REG, BASE_POINTER_REG);
    fprintf(asmFile, "pop %s\n", BASE_POINTER_REG);
    fprintf(asmFile, "ret\n");

    return errorCode;
}

int rewriteOpCompareToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(asmFile);
    assert(srcFile);

    int errorCode = 0;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), RIGHT, asmFile, srcFile);
    else {
        printf("Error! Compare node does not have RIGHT.\n");
        return 1;
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), LEFT, asmFile, srcFile);
    else {
        printf("Error! Compare node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "cmp %s, %s\n", OP_REG[LEFT], OP_REG[RIGHT]);

    if (nodeValue(node)->opCode == opEQUAL)
        fprintf(asmFile, "sete sil\n");
    if (nodeValue(node)->opCode == opNOT_EQUAL)
        fprintf(asmFile, "setne sil\n");
    if (nodeValue(node)->opCode == opBELOW)
        fprintf(asmFile, "setl sil\n");
    if (nodeValue(node)->opCode == opABOVE)
        fprintf(asmFile, "setg sil\n");
    if (nodeValue(node)->opCode == opE_BELOW)
        fprintf(asmFile, "setle sil\n");
    if (nodeValue(node)->opCode == opE_ABOVE)
        fprintf(asmFile, "setge sil\n");

    fprintf(asmFile, "movzx %s, sil\n", OP_REG[resultReg]);

    return errorCode;
}

int rewriteIdNodeToAsmCode(tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    if(*nodeRight(node))
        return rewriteFuncBodyToAsmCode(tree, node, asmFile, srcFile);

    const char* idName = nodeValue(node)->id.identifierName;
    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    nameTable_t* curNameTable = (tree->nameTableStack->data)[numOfCurNameTable];

    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return rewriteVarNodeToAsmCode (tree, node, resultReg, asmFile);
    else
        return rewriteFuncCallNodeToAsmCode (tree, node, asmFile, resultReg, srcFile);
}

int rewriteFuncBodyToAsmCode(tree_t* tree, node_t* node, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;

    fprintf(asmFile, "%s:\n", nodeValue(node)->id.identifierName);
    fprintf(asmFile, "push %s\n", BASE_POINTER_REG);
    fprintf(asmFile, "mov %s, %s\n", BASE_POINTER_REG, STACK_POINTER_REG);


    enterNewScope(tree);

    nameTable_t* curNameTable = getCurNameTable(tree);
    *numOfFrameCaleeSavedRegs(curNameTable)  = pushSavedRegs(calleeSaved, asmFile);

    if (*nodeLeft(node))
        errorCode = fprintfGettingParamsToAsmCode(tree, *nodeLeft(node), asmFile);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), LEFT, asmFile, srcFile);
    else {
        printf("Error! Func body does not have RIGHT!\n");
        return 1;
    }

    exitScope(tree);

    return errorCode;
}

int fprintfGettingParamsToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfGettingParamsToAsmCode (tree, *nodeLeft(node), asmFile);
        errorCode = fprintfGettingParamsToAsmCode (tree, *nodeRight(node), asmFile);
    }
    else
        paramVarId = addIdToCurrentScope(tree, nodeVarName(node), idPARAM);

    return errorCode;
}

int rewriteFuncCallNodeToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile, sourceFile* srcFile) {
    assert(tree);
    assert(node);
    assert(srcFile);
    assert(asmFile);

    int errorCode = 0;
    int numOfFuncParams = 0;

    pushSavedRegs(callerSaved, asmFile);

    if (*nodeLeft(node))
        numOfFuncParams = fprintfPassingParams (tree, *nodeLeft(node), asmFile, srcFile);

    fprintf(asmFile, "call :%s\n", nodeValue(node)->id.identifierName);

    if (*nodeLeft(node))
        fprintf(asmFile, "add %s, %d", STACK_POINTER_REG, numOfFuncParams * 8);

    if (resultReg == RIGHT)
        fprintf(asmFile, "mov %s, %s\n", OP_REG[RIGHT], OP_REG[LEFT]);

    popSavedRegs(asmFile);

    return errorCode;
}

int fprintfPassingParams (tree_t* tree, node_t* node, FILE* asmFile, sourceFile* srcFile) {

    static int curParamsCounter = 0;
    int oldParamsCounter = curParamsCounter;

    if(*nodeLeft(node) && *nodeRight(node)) {        //FIXME
        fprintfPassingParams (tree, *nodeRight(node), asmFile, srcFile);
        fprintfPassingParams (tree, *nodeLeft(node), asmFile, srcFile);
    }
    else {
        rewriteNodeToAsmCode (tree, *nodeLeft(node), asmFile, srcFile);
        fprintf(asmFile, "push %s\n", LEFT_OP_REG);
        curParamsCounter++;
    }

    return curParamsCounter - oldParamsCounter;
}

int pushSavedRegs (regSaveDecl_t saveDecl, FILE* asmFile) {
    int savedRegsCntr = 0;

    for (size_t curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (*regIsUsed(regsArray, curReg) && regSaveDecl(regsArray, curReg) == saveDecl) {
            fprintf(asmFile, "push %s\n", regName(regsArray, curReg));
            *regIsUsed(regsArray, curReg) = false;
            *regWasPushed(regsArray, curReg) = true;
            savedRegsCntr++;
        }

    return savedRegsCntr;
}

void popSavedRegs (FILE* asmFile) {
    for (size_t curReg = NUM_OF_REGS - 1; curReg >= 0; curReg--)
        if (*regWasPushed(regsArray, curReg)) {
            fprintf(asmFile, "pop %s\n", regName(regsArray, curReg));
            *regIsUsed(regsArray, curReg) = true;
            *regWasPushed(regsArray, curReg) = false;
        }
}

void rewriteVarAddressToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeType(node) != typeIdentifier) {
        printf("Error! In func %s unexpected type of node.\n", __func__);
        return 1;
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
            fprintf(asmFile, "[%s + %d]", BASE_POINTER_REG, *varOffset(searchedVarId));
            return;
        }
    }
    fprintf(asmFile, "%s", regName(regsArray, *varReg(searchedVarId)));

    return;
}

void rewriteVarNodeToAsmCode (tree_t* tree, node_t* node, resultReg_t resultReg, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    fprintf (asmFile, "mov %s, ", resultReg);
    rewriteVarAddressToAsmCode (tree, node, asmFile);
    fprintf(asmFile, "\n");
}

int findFreeReg (regInfo_t* regsArray) {
    assert(regsArray);

    for (int curReg = 0; curReg < NUM_OF_REGS; curReg++)
        if (!(*regIsUsed(regsArray, curReg)))
            return curReg;

    return NOT_IN_REG;
}
