#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/backendCntxtFuncs.h"
#include "../include/byteCodeWritingFuncs.h"


int main (int argc, const char* argv[]) {

    if (argc != 5) return printf("Usage: %s astFile.txt output.asm executable.elf programSource.txt\n", argv[0]), 1;

    const char* astFile = argv[1];
    const char* asmFile = argv[2];
    const char* elfFile = argv[3];
    const char* sourceFile = argv[4];

    backendContext_t cntxt = {};
    if (backendCntxtCtor(&cntxt, astFile, asmFile, elfFile, sourceFile) != BACKEND_SUCCESS) {
        reportBackendError(&cntxt);
        backendCntxtDtor(&cntxt);
        return *cntxtErrCode(&cntxt);
    }

    if (astToAsmAndByteCode(&cntxt) != BACKEND_SUCCESS) {
        reportBackendError(&cntxt);
        backendCntxtDtor(&cntxt);
        return *cntxtErrCode(&cntxt);
    };

    backendCntxtDtor(&cntxt);

    return BACKEND_SUCCESS;
}



