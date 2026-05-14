#ifndef BYTE_CODE_WRITITNG_FUNCS_H
#define BYTE_CODE_WRITITNG_FUNCS_H


int astToAsmAndByteCode (backendContext_t* cntxt);

//int executeBuffer (backendContext_t* cntxt);

int nodeToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int opNodeToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int opCalcToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int opAssignToByteCode (backendContext_t* cntxt, node_t* node);

int opWhileToByteCode (backendContext_t* cntxt, node_t* node);

int opIfToByteCode (backendContext_t* cntxt, node_t* node);

int opInToByteCode (backendContext_t* cntxt, node_t* node);

int opPutcharToByteCode(backendContext_t* cntxt, node_t* node);

int opOutToByteCode (backendContext_t* cntxt, node_t* node);

int opRetToByteCode (backendContext_t* cntxt, node_t* node);

int opCompareToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int idNodeToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int funcBodyToByteCode(backendContext_t* cntxt, node_t* node);

int getFuncArgs (backendContext_t* cntxt, node_t* node);

int callNodeToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int passingParamsToByteCode (backendContext_t* cntxt, node_t* node);

varPos_t getVarPos(backendContext_t* cntxt, node_t* node);

int varNodeToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

int opSqrtToByteCode (backendContext_t* cntxt, node_t* node, regCode_t resultReg);

uint32_t pushSavedRegs (backendContext_t* cntxt, regSaveDecl_t saveDecl);

int popSavedRegs (backendContext_t* cntxt, uint32_t pushedRegsMask);

int addStdLibInBuffer (backendContext_t* cntxt);

int writeElfFile(backendContext_t* cntxt);

void freeScopeRegs (backendContext_t* cntxt);

int findFreeReg (backendContext_t* cntxt);

#endif
