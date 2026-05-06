#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/backendCntxtFuncs.h"
#include "../include/asmProgramWriter.h"


int main (int argc, const char* argv[]) {

    if (argc != 4) return printf("Usage: %s input.txt output.asm programSource.txt\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const char* sourceFile = argv[3];

    backendContext_t cntxt = {};
    if (backendCntxtCtor(&cntxt, inputFile, outputFile, sourceFile) != BACKEND_SUCCESS) {
        reportBackendError(&cntxt);
        backendCntxtDtor(&cntxt);
        return *cntxtErrCode(&cntxt);
    }

    if (rewriteAstToAsmCode(&cntxt) != BACKEND_SUCCESS) {
        reportBackendError(&cntxt);
        backendCntxtDtor(&cntxt);
        return *cntxtErrCode(&cntxt);
    };

    backendCntxtDtor(&cntxt);

    return BACKEND_SUCCESS;
}



