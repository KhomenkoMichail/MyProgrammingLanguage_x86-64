#ifndef NAME_TABLE_CONSTS_H
#define NAME_TABLE_CONSTS_H

const size_t INITIAL_STACK_CAPACITY = 10;
const size_t INITIAL_TABLE_CAPACITY = 20;


enum idType_t {
    idVAR = 1,
    idFUNC = 2,
    idPARAM = 3,
};

struct funcInfo_t {
    size_t paramCount;

    size_t numOfLocalVars;

    bool isBodyNode;
    int haveBody;
};

struct varInfo_t {
    int varReg;
    int varOffset;
};


union idInfo_t {
    funcInfo_t funcInfo;
    varInfo_t varInfo;
};

struct identifierInfo {
    char* identifierName;
    unsigned long long identifierHash;

    idType_t idType;

    size_t scopeLevel;
    idInfo_t idInfo;
};

struct nameTable_t {
    identifierInfo* idArray;
    size_t size;

    uint32_t pushedRegsMask;

    int numOfParams;
    int numOfLocalVars;

    size_t capacity;
};

typedef struct nameTable_t* stackElement_t;

const stackElement_t POISON = NULL;
const int MAX_CAPACITY = 100000000;

enum stackErr_t {
    badStackPtr= 0b1,
    badDataPtr = 0b10,
    badSize = 0b100,
    badCapacity = 0b1000,
    badElementType = 0b10000,
    badRealloc = 0b100000,
    noElementsForPop = 0b1000000,
};

struct info {
    const char* nameOfFile;
    const char* nameOfFunct;
    unsigned int numOfLine;
};

struct stack {
    stackElement_t* data;
    ssize_t size;
    ssize_t capacity;
    const char* nameOfType;
    const char* nameOfStack;
    int errorCode;
    struct info stackInfo;
};


typedef struct stack stack_t;

#endif
