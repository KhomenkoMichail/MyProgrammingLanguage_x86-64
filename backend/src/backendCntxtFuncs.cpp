#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"

#include "../include/backendConsts.h"
#include "../include/backendCntxtFuncs.h"
#include "../include/sourceFileParser.h"

int backendCntxtCtor (backendContext_t* cntxt, const char* astFile, const char* asmFileName, const char* srcFileName) {
    assert(cntxt);
    assert(astFile);
    assert(asmFileName);
    assert(srcFileName);

    *cntxtAsmFileName(cntxt) = asmFileName;

    *cntxtSrcFile(cntxt) = (sourceFile*)calloc(1, sizeof(sourceFile));
    if (!*cntxtSrcFile(cntxt)) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_SRC_FILE_STRUCT_CALLOC,
                        "Error src file struct calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }
    getStructSourceFile(*cntxtSrcFile(cntxt), srcFileName);


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
    initNameTables(*cntxtTree(cntxt));


    *cntxtAstCopyBuffer(cntxt) = readFileAndCreateTree(*cntxtTree(cntxt),
                                                       *cntxtTreeDump(cntxt), astFile);
    if(!(*cntxtAstCopyBuffer(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_CREATE_TREE,
                        "Error tree create in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }


    *cntxtProgramBuf(cntxt) = (int*)calloc(INIT_PROGRAM_BUF_CAPASITY, sizeof(int));
    if(!(*cntxtProgramBuf(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PROGRAM_BUF_CALLOC,
                        "Error program buf calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }
    *cntxtProgramBufSize(cntxt) = 0;
    *cntxtProgramBufCapacity(cntxt) = INIT_PROGRAM_BUF_CAPASITY;


    *cntxtRegsArr(cntxt) = (regInfo_t*)calloc(NUM_OF_REGS, sizeof(regInfo_t));
    if(!(*cntxtRegsArr(cntxt))) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_REGS_ARR_CALLOC,
                        "Error regs array calloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }
    memcpy(*cntxtRegsArr(cntxt), INIT_REGS_ARRAY, sizeof(INIT_REGS_ARRAY));


    if (!labelVectorCtor(*cntxtLabelVector(cntxt), INIT_LABELS_ARR_CAPACITY)) {
        SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_LABELS_ARR_CTOR,
                        "Error labels array ctor in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
    }

    return BACKEND_SUCCESS;
}

labelVector_t* labelVectorCtor (labelVector_t* newLabelVector, size_t initCapacity) {
    assert(newLabelVector);

    *labelVectorSize(newLabelVector) = 0;
    *labelVectorCapacity(newLabelVector) = initCapacity;

    *labelVectorArr(newLabelVector) = (label_t*)calloc(initCapacity, sizeof(label_t));
    if (!(*labelVectorArr(newLabelVector)))
        return NULL;

    for (size_t numOfLabel = 0; numOfLabel < initCapacity; numOfLabel++) {
        label_t* curLabel = getLabel(newLabelVector, numOfLabel);

        *intVectorCapacity(*(labelPatchOffsets(curLabel))) = INIT_LABEL_PATCH_OFFSETS_CAPACITY;
        *intVectorBuf(*(labelPatchOffsets(curLabel))) = (int*)calloc(INIT_LABEL_PATCH_OFFSETS_CAPACITY, sizeof(int));

        if (!(*intVectorBuf(*labelPatchOffsets(curLabel))))
            return NULL;
    }

    return (newLabelVector);
}

void labelVectorDtor (labelVector_t* labelVector) {
    assert(labelVector);

    for (size_t numOfLabel = 0; numOfLabel < *labelVectorCapacity(labelVector); numOfLabel++) {
        label_t* curLabel = getLabel(labelVector, numOfLabel);

        //if (*labelName(curLabel))           //FIXME ??
        //    free(*labelName(curLabel));

        if (*intVectorBuf(*labelPatchOffsets(curLabel)))
            free(*intVectorBuf(*labelPatchOffsets(curLabel)));
    }

    free(*labelVectorArr(labelVector));
}

void backendCntxtDtor (backendContext_t* cntxt) {
    assert(cntxt);

    if (*cntxtTree(cntxt)) {
        destroyNameTables(*cntxtTree(cntxt));
        deleteTree(*cntxtTree(cntxt));
        free(*cntxtTree(cntxt));
    }

    if (*cntxtSrcFile(cntxt))
        freeStructSourceFile(*cntxtSrcFile(cntxt));

    if (*cntxtTreeDump(cntxt))
        free(*cntxtTreeDump(cntxt));

    if (*cntxtAstCopyBuffer(cntxt))
        free(*cntxtAstCopyBuffer(cntxt));

    if (*cntxtProgramBuf(cntxt))
        free(*cntxtProgramBuf(cntxt));

    if (*cntxtRegsArr(cntxt))
        free(*cntxtRegsArr(cntxt));

    if (*cntxtLabelsArr(cntxt))
        labelVectorDtor(*cntxtLabelsArr(cntxt));
}
