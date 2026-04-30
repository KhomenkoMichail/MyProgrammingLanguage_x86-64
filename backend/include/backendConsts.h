#ifndef BACKEND_CONSTS_H
#define BACKEND_CONSTS_H

    const char* STACK_POINTER_REG = "rsp";
    const char* BASE_POINTER_REG = "rbp";

    const char* RET_REG           = "rax";

enum resultReg_t {
    LEFT  = 0,
    RIGHT = 1,
};

const char* OP_REG_[2] = { "rax", "rbx" };

enum regCode_t {
    NO_REG = -1,
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15
};

const int NOT_IN_REG = 16;
const int NOT_IN_MEMORY = 0xBADDEAD;

enum regSaveDecl_t {
    callerSaved  = 1,
    calleeSaved  = 2,
    specialSaved = 3,
};

struct regInfo_t {
    const char* name;
    bool isUsed;
    const regSaveDecl_t regSaveDecl;
    bool wasPushed;
};

regInfo_t regsArray[] = { { "rax", true,  specialSaved, false },
                          { "rbx", true,  calleeSaved,   false },
                          { "rcx", false, callerSaved,  false },
                          { "rdx", false, callerSaved,  false },
                          { "rsp", true,  specialSaved, false },
                          { "rbp", true,  specialSaved, false },
                          { "rsi", false, callerSaved,  false },
                          { "rdi", false, callerSaved,  false },
                          { "r8",  false, callerSaved,  false },
                          { "r9",  false, callerSaved,  false },
                          { "r10", false, callerSaved,  false },
                          { "r11", false, callerSaved,  false },
                          { "r12", false, calleeSaved,  false },
                          { "r13", false, calleeSaved,  false },
                          { "r14", false, calleeSaved,  false },
                          { "r15", false, calleeSaved,  false }, };

const size_t NUM_OF_REGS = sizeof(regsArray) / sizeof(regInfo_t);

enum opRegRegCode_t {
    ADD = 0x01,
    SUB = 0x28,
    CMP = 0x39,
    TEST = 0x85,
    IMUL = 0xAF,
};

enum modARGS_t {
    MEM_NO_OFFSET = 0x00,
    MEM_8_OFFSET = 0x01,
    MEM_32_OFFSET = 0x02,
    REG_REG = 0x03,
    REG_MEM = 0x04,
    MEM_REG = 0x05,
    MEM_MEM = 0x06,
    REG_CONST = 0x07,
};

struct file_t {
    const char* name;
    FILE* filePtr;
};

struct intVector_t {
    int* buf;
    size_t curSize;
    size_t capacity;
};

struct label_t {
    const char* label;
    size_t labelAddress;

    unsigned long long stringLabelHash;
    intVector_t* patchOffsets;
};

labelVector_t {
    label_t*
    size_t curSize;
    size_t capacity;
};

struct backendContext_t {
    file_t srcFile;
    file_t asmFile;

    tree_t* tree;
    vector_t* programBuf;

    regInfo_t* regsArr;

    int ifCounter;
    int whileCounter;

    size_t curFuncRSPsubOffset;
    size_t curFuncStackVarsCntr;

    vector_t* labelsArr;
}

#endif
