#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../include/backendConsts.h"
#include "../include/backendCntxtFuncs.h"


struct backendContext_t {
    file_t srcFile;
    file_t asmFile;

    tree_t* tree;
    vector_t* programBuf;

    regInfo_t* regsArray;

    int ifCounter;
    int whileCounter;

    size_t curFuncRSPsubOffset;
    size_t curFuncStackVarsCntr;

    vector_t* labelsArr;
}

int backendCntxtCtor (backendContext_t* cntxt, const char* asmFileName, const char* srcFileName) {
    assert(cntxt);
    assert(asmFileName);
    assert(srcFileName);




}
