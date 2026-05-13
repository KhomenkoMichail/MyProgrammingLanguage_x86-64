#ifndef BACKEND_STRUCT_ACCESS_FUNCTIONS_H
#define BACKEND_STRUCT_ACCESS_FUNCTIONS_H

bool* regIsUsed (backendContext_t* cntxt, regCode_t regCode);

regSaveDecl_t regSaveDecl (backendContext_t* cntxt, regCode_t regCode);

const char* regName (backendContext_t* cntxt, regCode_t regCode);

const char* regLowByteName (backendContext_t* cntxt, regCode_t regCode);

sourceFile_t** cntxtSrcFile (backendContext_t* cntxt);

const char** cntxtElfFileName (backendContext_t* cntxt);

const char** cntxtAsmFileName (backendContext_t* cntxt);

FILE** cntxtAsmFile (backendContext_t* cntxt);

tree_t** cntxtTree (backendContext_t* cntxt);

uint8_t** cntxtProgramBuf (backendContext_t* cntxt);

size_t* cntxtProgramBufSize(backendContext_t* cntxt);

size_t* cntxtProgramBufCapacity(backendContext_t* cntxt);

regInfo_t** cntxtRegsArr (backendContext_t* cntxt);

int* cntxtIfCounter (backendContext_t* cntxt);

int* cntxtWhileCounter (backendContext_t* cntxt);

size_t* cntxtCurFuncRSPsubOffset (backendContext_t* cntxt);

size_t* cntxtCurFuncStackVarsCntr (backendContext_t* cntxt);

dump** cntxtTreeDump (backendContext_t* cntxt);

char** cntxtAstCopyBuffer (backendContext_t* cntxt);

int** intVectorBuf (intVector_t* intVec);

size_t* intVectorSize (intVector_t* intVec);

size_t* intVectorCapacity (intVector_t* intVec);

labelVector_t* cntxtLabelsArr (backendContext_t* cntxt);

size_t* labelVectorSize (labelVector_t* labelVec);

size_t* labelVectorCapacity (labelVector_t* labelVec);

label_t** labelVectorArr (labelVector_t* labelVec);

label_t* getLabel (labelVector_t* labelVec, size_t labelNum);

const char** labelName (label_t* label);

size_t* labelAddress (label_t* label);

unsigned long long* labelHash (label_t* label);

intVector_t* labelPatchAddresses (label_t* label);

int* intVectorElem (intVector_t* intVec, size_t elemNum);

backendErr_t* cntxtErrCode (backendContext_t* cntxt);

char* cntxtErrMessage (backendContext_t* cntxt);

labelVector_t* cntxtLabelVector (backendContext_t* cntxt);

bool* varPosInReg (varPos_t* varPos);

regCode_t* varPosRegCode (varPos_t* varPos);

int* varPosRbpOffset (varPos_t* varPos);

bool* labelHasAddress (label_t* label);

uint8_t* cntxtProgramBufElem (backendContext_t* cntxt, size_t elemNum);

#endif
