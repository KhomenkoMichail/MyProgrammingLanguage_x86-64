#ifndef ASM_PROGRAM_WRITER_H
#define ASM_PROGRAM_WRITER_H

int rewriteAstToAsmCode (backendContext_t* cntxt);

int rewriteNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int rewriteOpNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int rewriteOpCalcToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int rewriteOpAssignToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteOpWhileToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteOpIfToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteOpInToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteOpOutToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteOpRetToAsmCode (backendContext_t* cntxt, node_t* node);

void freeScopeRegs (backendContext_t* cntxt);

int rewriteOpCompareToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int rewriteIdNodeToAsmCode(backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int rewriteFuncBodyToAsmCode(backendContext_t* cntxt, node_t* node);

int fprintfGettingParamsToAsmCode (backendContext_t* cntxt, node_t* node);

int rewriteFuncCallNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int fprintfPassingParams (backendContext_t* cntxt, node_t* node);

int rewriteVarAddressToAsmCode(backendContext_t* cntxt, node_t* node);

int rewriteVarNodeToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

int findFreeReg (backendContext_t* cntxt);

void fprintfAsmFileHeader (FILE* asmFile);

int rewriteOpSqrtToAsmCode (backendContext_t* cntxt, node_t* node, resultReg_t resultReg);

uint32_t pushSavedRegs (backendContext_t* cntxt, regSaveDecl_t saveDecl);

void popSavedRegs (backendContext_t* cntxt, uint32_t pushedRegsMask);

#endif
