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
