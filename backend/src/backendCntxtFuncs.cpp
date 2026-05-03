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
    dump* treeDump;
    char* astCopyBuffer;

    intVector_t programBuf;

    regInfo_t* regsArr;

    int ifCounter;
    int whileCounter;

    size_t curFuncRSPsubOffset;
    size_t curFuncStackVarsCntr;

    labelVector_t* labelsArr;
}

int backendCntxtCtor (backendContext_t* cntxt, const char* astFile, const char* asmFileName, const char* srcFileName) {
    assert(cntxt);
    assert(astFile);
    assert(asmFileName);
    assert(srcFileName);

    *cntxtSrcFileName(cntxt) = srcFileName;
    *cntxtAsmFileName(cntxt) = asmFileName;

    *cntxtTreeDump(cntxt) = (dump*)calloc(1, sizeof(dump));
    if(!(*cntxtTreeDump(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_TREE_DUMP_CALLOC,
                        "Error tree dump calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    *dumpNameOfDumpFile(*cntxtTreeDump(cntxt)) = "DUMPS/backendTreeDump.html";
    *dumpNameOfGraphFile(*cntxtTreeDump(cntxt)) = "DUMPS/backGraph.txt";

    *cntxtTree(cntxt) = (tree_t*)calloc(1, sizeof(tree_t));
    if(!(*cntxtTree(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_TREE_CALLOC,
                        "Error tree calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    *cntxtAstCopyBuffer(cntxt) = readFileAndCreateTree(*cntxtTree(cntxt),
                                                       *cntxtTreeDump(cntxt), astFile);
    if(!(*cntxtAstCopyBuffer(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_CREATE_TREE,
                        "Error tree create in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    *cntxtRegsArr(cntxt) = (regInfo_t*)calloc(1, sizeof(INIT_REGS_ARRAY));
    if(!(*cntxtRegsArr(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_REGS_ARR_CALLOC,
                        "Error regs array calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    memcpy(*cntxtRegsArr(cntxt), INIT_REGS_ARRAY, sizeof(INIT_REGS_ARRAY));

}
