#include <assert.h>
#include <stdio.h>
#include <cstdint>

#include "../include/structsAndConsts.h"
#include "../include/structAccessFunctions.h"

node_t** nodeParent (node_t* node) {
    assert(node);

    return &(node->parent);
}

node_t** nodeLeft (node_t* node) {
    assert(node);

    return &(node->left);
}

node_t** nodeRight (node_t* node) {
    assert(node);

    return &(node->right);
}

nodeType_t* nodeType (node_t* node) {
    assert(node);

    return &(node->type);
}

nodeValue_t* nodeValue (node_t* node) {
    assert(node);

    return &(node->value);
}

node_t** treeRoot (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->rootNode);
}

size_t* treeSize (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->size);
}

size_t* nodeLine (node_t* node) {
    assert(node);

    return &(node->numOfLine);
}

char* nodeVarName (node_t* node) {
    assert(node);
    return nodeValue(node)->id.identifierName;
}

nameTable_t* getCurNameTable (tree_t* tree) {
    assert(tree);

    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    return (tree->nameTableStack->data)[numOfCurNameTable];
}

int* numOfTableLocalVars (nameTable_t* nameTable) {
    assert(nameTable);

    return &(nameTable->numOfLocalVars);
}

const char** dumpNameOfDumpFile (dump* curDump) {
    assert(curDump);
    return &(curDump->nameOfDumpFile);
}

const char** dumpNameOfGraphFile (dump* curDump) {
    assert(curDump);
    return &(curDump->nameOfGraphFile);
}

size_t* nodeFuncNumOfLocalVars (node_t* node) {
    assert(node);
    return &(node->value.id.idInfo.funcInfo.numOfLocalVars);
}

int* curScopeNumOfLocalVars (tree_t* tree) {
    assert(tree);
    return &((tree->nameTableStack->data[tree->nameTableStack->size - 1])->numOfLocalVars);
}

uint32_t* curScopePushedRegsMask (tree_t* tree) {
    assert(tree);
    return &((tree->nameTableStack->data[tree->nameTableStack->size - 1])->pushedRegsMask);
}

void setNodeIsFuncBody (node_t* node) {
    assert(node);
    node->value.id.idInfo.funcInfo.isBodyNode = true;
}

bool getNodeIsFuncBody (node_t* node) {
    assert(node);

    if (*nodeType(node) != typeIdentifier || node->value.id.idType != idFUNC)
        return false;

    return node->value.id.idInfo.funcInfo.isBodyNode;
}

int* varOffset (identifierInfo* varInfo) {
    assert(varInfo);
    return &(varInfo->idInfo.varInfo.varOffset);
}

int* varReg (identifierInfo* varInfo) {
    assert(varInfo);
    return &(varInfo->idInfo.varInfo.varReg);
}

idType_t* infoIdType (identifierInfo* idStruct) {
    assert(idStruct);
    return &(idStruct->idType);
}
