#ifndef BACKEND_STRUCT_ACCESS_FUNCTIONS_H
#define BACKEND_STRUCT_ACCESS_FUNCTIONS_H

bool* regIsUsed (regInfo_t* regInfoArr, regCode_t regCode);

const char* regName (regInfo_t* regInfoArr, regCode_t regCode);

int* varOffset (identifierInfo* varInfo);

#endif
