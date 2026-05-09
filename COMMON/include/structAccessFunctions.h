#ifndef STRUCT_ACCESS_FUNCTIONS_H
#define STRUCT_ACCESS_FUNCTIONS_H

node_t** nodeParent (node_t* node);

node_t** nodeLeft (node_t* node);

node_t** nodeRight (node_t* node);

nodeType_t* nodeType (node_t* node);

nodeValue_t* nodeValue (node_t* node);

nameTable_t* getCurNameTable (tree_t* tree);

node_t** treeRoot (tree_t* treePtr);

size_t* treeSize (tree_t* treePtr);

size_t* nodeLine (node_t* node);

char* nodeVarName (node_t* node);

int* numOfTableLocalVars (nameTable_t* nametable);

const char** dumpNameOfDumpFile (dump*);

const char** dumpNameOfGraphFile (dump*);

size_t* nodeFuncNumOfLocalVars (node_t* node);

int* curScopeNumOfLocalVars (tree_t* tree);

void setNodeIsFuncBody (node_t* node);

bool getNodeIsFuncBody (node_t* node);

uint32_t* curScopePushedRegsMask (tree_t* tree);

int* varOffset (identifierInfo* varInfo);

int* varReg (identifierInfo* varInfo);

idType_t* infoIdType (identifierInfo* idStruct);

operatorCode_t* nodeOpCode (node_t* node);

char** nodeIdentifierName (node_t* node);

#endif
