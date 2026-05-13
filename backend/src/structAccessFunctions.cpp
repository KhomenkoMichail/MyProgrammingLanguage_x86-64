#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"


bool* regIsUsed (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);

    return &((cntxt->regsArr[regCode]).isUsed);
}

regSaveDecl_t regSaveDecl (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);

    return ((cntxt->regsArr[regCode]).regSaveDecl);
}

const char* regName (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);

    return (cntxt->regsArr[regCode]).name;
}

const char* regLowByteName (backendContext_t* cntxt, regCode_t regCode) {
    assert(cntxt);

    return (cntxt->regsArr[regCode]).lowByteRegName;
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

uint8_t** cntxtProgramBuf (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->programBuf.buf);
}

size_t* cntxtProgramBufSize(backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->programBuf.curSize);
}

size_t* cntxtProgramBufCapacity(backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->programBuf.capacity);
}

regInfo_t** cntxtRegsArr (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->regsArr);
}

int* cntxtIfCounter (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->ifCounter);
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

int* intVectorElem (intVector_t* intVec, size_t elemNum) {
    assert(intVec);
    return &((intVec->buf)[elemNum]);
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

label_t** labelVectorArr (labelVector_t* labelVec) {
    assert(labelVec);
    return &(labelVec->labelArr);
}

label_t* getLabel (labelVector_t* labelVec, size_t labelNum) {
    assert(labelVec);
    return &((labelVec->labelArr)[labelNum]);
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

intVector_t* labelPatchAddresses (label_t* label) {
    assert(label);
    return &(label->patchAddresses);
}

backendErr_t* cntxtErrCode (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->errCode);
};

char* cntxtErrMessage (backendContext_t* cntxt) {
    assert(cntxt);
    return cntxt->errMsg;
}

labelVector_t* cntxtLabelVector (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->labelsArr);
}

bool* varPosInReg (varPos_t* varPos) {
    assert(varPos);
    return &(varPos->inReg);
}
regCode_t* varPosRegCode (varPos_t* varPos) {
    assert(varPos);
    return &(varPos->varrAddrComp.regCode);
}

int* varPosRbpOffset (varPos_t* varPos) {
    assert(varPos);
    return &(varPos->varrAddrComp.rbpOffset);
}

bool* labelHasAddress (label_t* label) {
    assert(label);
    return &(label->hasAddress);
}

uint8_t* cntxtProgramBufElem (backendContext_t* cntxt, size_t elemNum) {
    assert(cntxt);
    return &((cntxt->programBuf.buf)[elemNum]);
}

const char** cntxtElfFileName (backendContext_t* cntxt) {
    assert(cntxt);
    return &(cntxt->elfFileName);
}
