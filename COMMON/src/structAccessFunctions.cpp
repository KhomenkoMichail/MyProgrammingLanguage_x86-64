#include <assert.h>
#include <stdio.h>

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

int* numOfTableLocalVars (nameTable_t* nametable) {
    assert(nameTable);

    return &(currentTable->numOfLocalVars);
}

int* numOfFrameCaleeSavedRegs (nameTable_t* nametable) {
    assert(nameTable);

    return &(currentTable->numOfCaleeSavedRegs);
}

const char** dumpNameOfDumpFile (dump*) {
    assert(dump);
    return &(dump->nameOfDumpFile);
}

const char** dumpNameOfGraphFile (dump*) {
    assert(dump);
    return &(dump->nameOfGraphFile);
}

size_t* nodeFuncNumOfLocalVars (node_t* node) {
    assert(node);
    return &(node->value.id.idInfo.funcInfo.numOfLocalVars);
}

size_t* curScopeNumOfLocalVars (tree_t* tree) {
    assert(tree);
    return &((tree->nameTableStack->data[tree->nameTableStack->size - 1])->numOfLocalVars);
}

bool* nodeIsFuncBody (node_t* node) {
    assert(node);
    return &(node->value.id.idInfo.funcInfo.isBodyNode);
}
