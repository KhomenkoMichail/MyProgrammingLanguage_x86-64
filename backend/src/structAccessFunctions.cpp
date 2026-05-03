#include <stdio.h>
#include <assert.h>

#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"


bool* regIsUsed (regInfo_t* regInfoArr, regCode_t regCode) {
    assert(regInfoArr);

    return &((regInfoArr[regCode]).isUsed);
}

bool regSaveDecl (regInfo_t* regInfoArr, regCode_t regCode) {
    assert(regInfoArr);

    return (regInfoArr[regCode]).regSaveDecl;
}

const char* regName (regInfo_t* regInfoArr, regCode_t regCode) {
    assert(regInfoArr);

    return (regInfoArr[regCode]).name;
}

bool* regWasPushed (regInfo_t* regInfoArr, regCode_t regCode) {
    assert(regInfoArr);

    return &((regInfoArr[regCode]).wasPushed);
}

int* varOffset (identifierInfo* varInfo) {
    assert(varInfo);
    return &(varInfo->idInfo.varInfo.varOffset);
}

int* varReg (identifierInfo* varInfo) {
    assert(varInfo);
    return &(varInfo->idInfo.varInfo.varReg);
}

sourceFile_t** cntxtSrcFile (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->sourceFile);
}

const char** cntxtAsmFileName (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->asmFile.name);
}

FILE** cntxtAsmFile (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->asmFile.filePtr);
}

tree_t** cntxtTree (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->tree);
}

int** cntxtProgramBuf (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt.programBuf.buf);
}

size_t* cntxtProgramBufSize(backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt.programBuf.curSize);
}

size_t* cntxtProgramBufCapacity(backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt.programBuf.capacity);
}

regInfo_t** cntxtRegsArr (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->regsArr);
}

int* cntxtIfCounter (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->ifCounter)
}

int* cntxtWhileCounter (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->whileCounter);
}

size_t* cntxtCurFuncRSPsubOffset (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->curFuncRSPsubOffset);
}

size_t* cntxtCurFuncStackVarsCntr (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->curFuncStackVarsCntr);
}

dump** cntxtTreeDump (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->treeDump);
}

char** cntxtAstCopyBuffer (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->astCopyBuffer);
}

int** intVectorBuf (intVector_t* intVec) {
    assert(intVec);
    return &(intVec->buf);
}

size_t* intVectorSize (intVector_t* intVec) {
    assert(intVec);
    return &(intVec->curSize);
}

size_t* intVectorCapacity (intVector_t* intVec) {
    assert(intVec);
    return &(intVec->capacity);
}

labelVector_t* cntxtLabelsArr (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->labelsArr);
}

size_t* labelVectorSize (labelVector_t* labelVec) {
    assert(labelVec);
    return &(labelVec->curSize);
}

size_t* labelVectorCapacity (labelVector_t* labelVec) {
    assert(labelVec);
    return &(labelVec->capacity);
}

label_t** lableVectorArr (labelVector_t* labelVec) {
    assert(labelVec);
    return &(labelVec->labelArr);
}

label_t* getLable (labelVector_t* labelVec, size_t lableNum) {
    assert(labelVec);
    return &((labelVec->labelArr)[lableNum]);
}

const char** labelName (label_t* label) {
    assert(label);
    return &(label->name);
}

size_t* labelAddress (label_t* label) {
    assert(label);
    return &(label->address);
}

unsigned long long* labelHash (label_t* label) {
    assert(label);
    return &(label->hash);
}

intVector_t* labelPatchOffsets (label_t* label) {
    assert(label);
    return &(label->patchOffsets);
}



