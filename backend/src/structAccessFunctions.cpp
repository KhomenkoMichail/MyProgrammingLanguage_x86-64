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

const char** cntxtSrcFileName (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->srcFile.name);
}

FILE** cntxtSrcFile (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->srcFile.filePtr);
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

intVector_t* cntxtProgramBuf (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->programBuf);
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



