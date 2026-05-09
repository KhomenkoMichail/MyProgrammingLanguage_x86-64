#ifndef BACKEND_CNTXT_FUNCTIONS_H
#define BACKEND_CNTXT_FUNCTIONS_H

int backendCntxtCtor (backendContext_t* cntxt, const char* astFileName, const char* asmFileName, const char* srcFileName);

labelVector_t* labelVectorCtor (labelVector_t* newLabelVector, size_t initCapacity);

void labelVectorDtor (labelVector_t* labelVector);

void backendCntxtDtor (backendContext_t* cntxt);

void reportBackendError(backendContext_t* cntxt);

int addLabelAddressInCntxt (backendContext_t* cntxt, const char* newLabelName);

label_t* addNewLabelInCntxt (backendContext_t* cntxt, const char* newLabelName, unsigned long long newlabelHash);

int structLabelComparator (const void* firstLabelPtr, const void* secondLabelPtr);

labelVector_t* reallocLabelArr (labelVector_t* labelVector);

int patchLabelWaitingAddresses (label_t label);

int patchCurLabel (backendContext_t* cntxt, const char* patchLabelName);

int addWaitingPatchAddress (backendContext_t* cntxt, label_t* label);

#endif
