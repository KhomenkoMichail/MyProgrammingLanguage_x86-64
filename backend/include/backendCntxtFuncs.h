#ifndef BACKEND_CNTXT_FUNCTIONS_H
#define BACKEND_CNTXT_FUNCTIONS_H

int backendCntxtCtor (backendContext_t* cntxt, const char* astFileName, const char* asmFileName, const char* srcFileName);

labelVector_t* labelVectorCtor (labelVector_t* newLabelVector, size_t initCapacity);

void labelVectorDtor (labelVector_t* labelVector);

void backendCntxtDtor (backendContext_t* cntxt);

void reportBackendError(backendContext_t* cntxt);

#endif
