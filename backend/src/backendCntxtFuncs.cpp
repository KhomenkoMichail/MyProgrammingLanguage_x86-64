#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/readTreeFromFileFunc.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/sourceFileParser.h"
#include "../include/backendConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/backendCntxtFuncs.h"

int backendCntxtCtor (backendContext_t* cntxt, const char* astFileName, const char* asmFileName, const char* srcFileName) {
    assert(cntxt);
    assert(astFileName);
    assert(asmFileName);
    assert(srcFileName);

    *cntxtAsmFileName(cntxt) = asmFileName;

    *cntxtSrcFile(cntxt) = (sourceFile_t*)calloc(1, sizeof(sourceFile_t));
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
                                                       *cntxtTreeDump(cntxt), astFileName);
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

    for (int curReg = 0; curReg < NUM_OF_REGS; curReg++)
        (*cntxtRegsArr(cntxt))[curReg] = INIT_REGS_ARRAY[curReg];

    if (!labelVectorCtor(cntxtLabelVector(cntxt), INIT_LABELS_ARR_CAPACITY)) {
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

        *intVectorCapacity(labelPatchAddresses(curLabel)) = INIT_LABEL_PATCH_ADDRESSES_CAPACITY;
        *intVectorBuf(labelPatchAddresses(curLabel)) = (int*)calloc(INIT_LABEL_PATCH_ADDRESSES_CAPACITY, sizeof(int));

        if (!(*intVectorBuf(labelPatchAddresses(curLabel))))
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

        if (*intVectorBuf(labelPatchAddresses(curLabel)))
            free(*intVectorBuf(labelPatchAddresses(curLabel)));
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

    if (cntxtLabelsArr(cntxt))
        labelVectorDtor(cntxtLabelsArr(cntxt));
}

void reportBackendError(backendContext_t* cntxt) {
    assert(cntxt);

    if (*cntxtErrCode(cntxt) == BACKEND_SUCCESS) return;

    fprintf(stderr, "\n--- BACKEND ERROR ---\n");
    fprintf(stderr, "ErrCode: %d\n", *cntxtErrCode(cntxt));

    fprintf(stderr, "Message: %s\n", cntxtErrMessage(cntxt));
    fprintf(stderr, "-----------------------\n");
}

int addLabelAddressInCntxt (backendContext_t* cntxt, const char* newLabelName) {
    assert(cntxt);
    assert(newLabelName);

    labelVector_t* labelVector = *cntxtLabelsArr(cntxt);

    unsigned long long labelHash = getStringHash(newLabelName);
    label_t* searchedLabel = (label_t*)bsearch(&labelHash, *labelVectorArr(labelVector),
                                               *labelVectorSize(labelVector), sizeof(label_t),
                                               bsearchLabelComparator);

    if (searchedLabel && strcmp(newLabelName, *labelName(searchedLabel))) {
        *labelHasAddress(label) = true;
        *labelAddress(searchedLabel) = *cntxtProgramBufSize(cntxt);
        patchLabelWaitingAddresses(searchedLabel);
    }
    else {
        label_t* newLabel = addNewLabelInCntxt(cntxt, newLabelName);
        if (!newLabel) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_LABELS_ARR_REALLOC,
                        "Error labels array realloc in func addNewLabelInCntxt, %s:%d\n",
                        __FILE__, __LINE__);
        }
        *labelHasAddress(newLabel) = true;
        *labelAddress(newLabel) = *cntxtProgramBufSize(cntxt);
    }

    return BACKEND_SUCCESS;
}

label_t* addNewLabelInCntxt (backendContext_t* cntxt, const char* newLabelName, unsigned long long newlabelHash) {
    assert(cntxt);
    assert(newLabelName);

    labelVector_t* labelVector = *cntxtLabelsArr(cntxt);

    if (*labelVectorSize(labelVector) >= *labelVectorCapacity(labelVector))
        if (!reallocLabelArr(labelVector))
            return NULL;

    label_t* newLabel = getLabel(labelVector, *labelVectorSize(labelVector));
    *labelVectorSize(labelVector) += 1;

    *labelName(newLabel) = newLabelName;
    *labelHash(newLabel) = newlabelHash;

    qsort(*labelVectorArr(labelVector), *labelVectorSize(labelVector), sizeof(label_t), structLabelComparator);

    return newLabel;
}

int structLabelComparator (const void* firstLabelPtr, const void* secondLabelPtr) {
    assert(firstStruct);
    assert(secondStruct);

    const label_t* firstLable = (const label_t*)firstLabelPtr;
    const label_t* secondLable = (const label_t*)secondLabelPtr;

    return (int)(*labelHash(firstLable) - *labelHash(secondLable));
}

labelVector_t* reallocLabelArr (labelVector_t* labelVector) {
    assert(labelVector);

    label_t* newLabelArr = (label_t*)realloc(*labelVectorArr(labelVector),
                                                 *labelVectorCapacity(labelVector) * 2);
    if (!newLabelArr)
        return NULL;

    *labelVectorCapacity(labelVector) *= 2;
    *labelVectorArr(labelVector) = newLabelArr;

    for (size_t numOfLabel = *labelVectorSize(labelVector);
                numOfLabel < *labelVectorCapacity(labelVector); numOfLabel++) {

        label_t* curLabel = getLabel(labelVector, numOfLabel);

        *intVectorCapacity(labelPatchAddresses(curLabel)) = INIT_LABEL_PATCH_ADDRESSES_CAPACITY;
        *intVectorBuf(labelPatchAddresses(curLabel)) = (int*)calloc(INIT_LABEL_PATCH_ADDRESSES_CAPACITY,
                                                                    sizeof(int));

        if (!(*intVectorBuf(labelPatchAddresses(curLabel))))
            return NULL;
    }

    return labelVector;
}

int patchLabelWaitingAddresses (label_t label) {
    assert(label);

    intVector_t* addressesVector = labelPatchAddresses(label);

    for (size_t curPatch = 0; curPatch < intVectorSize(addressesVector); curPatch++) {
        int32_t patchOffset = (int)*labelAddress(label) - *intVectorElem(addressesVector, curPatch);

        if (emit_32GivenPos(cntxt, (size_t)*intVectorElem(addressesVector, curPatch), patchOffset))
            return *cntxtErrCode(cntxt);

        *intVectorElem(addressesVector, curPatch) = 0;
    }
    *intVectorSize(addressesVector) = 0;

    return BACKEND_SUCCESS;
}

int patchCurLabel (backendContext_t* cntxt, const char* patchLabelName) {
    assert(cntxt);
    assert(patchLabelName);

    labelVector_t* labelVector = *cntxtLabelsArr(cntxt);

    unsigned long long labelHash = getStringHash(patchLabelName);
    label_t* searchedLabel = (label_t*)bsearch(&labelHash, *labelVectorArr(labelVector),
                                               *labelVectorSize(labelVector), sizeof(label_t),
                                               bsearchLabelComparator);

    if (searchedLabel && strcmp(patchLabelName, *labelName(searchedLabel))) {
        if (*labelHasAddress(searchedLabel)) {
            int32_t patchOffset = (int32_t)*labelAddress(searchedLabel) - (int32_t)*cntxtProgrambufSize(cntxt);
            return emit_32givenPos(cntxt, *cntxtProgramBufSize(cntxt), patchOffset);
        }
        else
            return addWaitingPatchAddress(cntxt, searchedLabel);
    }
    else {
        label_t* newLabel = addNewLabelInCntxt(cntxt, patchLabelName);
        if (!newLabel) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_LABELS_ARR_REALLOC,
                        "Error labels array realloc in func addNewLabelInCntxt, %s:%d\n",
                        __FILE__, __LINE__);
        }
        *labelHasAddress(newLabel) = false;
        return addWaitingPatchAddress(cntxt, newLabel);
    }

    return BACKEND_SUCCESS;
}

int addWaitingPatchAddress (backendContext_t* cntxt, label_t* label) {
    assert(cntxt);
    assert(label);

    intVector_t* addressesVector = labelPatchAddresses(label);

    if (*intVectorSize(addressesVector) >= *intVectorCapacity(addressesVector)) {
        int* newAddrArr = (int*)realloc(*intVectorBuf(addressesVector),
                                        *intVectorCapacity(addressesVector) * 2);
        if (!newAddrArr) {
            SET_ERR_AND_RETURN(cntxt, BACKEND_ERR_PATCH_ARR_REALLOC,
                        "Error patch addresses array realloc in func %s, %s:%d\n",
                        __func__, __FILE__, __LINE__);
        }

        *intVectorCapacity(addressesVector) *= 2;
        *intVectorBuf(addressesVector) = newAddrArr;
    }
    *intVectorElem(addressesVector, *intVectorSize(addressesVector)) = *cntxtProgramBufSize(cntxt);
    *intVectorSize(addressesVector) += 1;

    return BACKEND_SUCCESS;
}
