#ifndef BACKEND_CONSTS_H
#define BACKEND_CONSTS_H

#define SET_ERR_AND_RETURN(cntxt, errCode, ...) do { \
    if (cntxt) { \
        (cntxt)->errCode = code; \
        snprintf((cntxt)->errMsg, sizeof((cntxt)->errMsg), __VA_ARGS__); \
    } \
    return errCode; \
} while(0)

enum backendErr_t {
    BACKEND_SUCCESS = 0;
    BACKEND_ERR_TREE_DUMP_CALLOC = 1;
    BACKEND_ERR_TREE_CALLOC = 2,
    BACKEND_ERR_CREATE_TREE = 3,
    BACKEND_ERR_REGS_ARR_CALLOC = 4,

}

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

const regInfo_t INIT_REGS_ARRAY[] = { { "rax", true,  specialSaved, false },
                                      { "rbx", true,  calleeSaved,  false },
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

const size_t NUM_OF_REGS = sizeof(INIT_REGS_ARRAY) / sizeof(regInfo_t);

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
    const char* name;
    size_t address;

    unsigned long hash;
    intVector_t* patchOffsets;
};

labelVector_t {
    label_t* labelArr;
    size_t curSize;
    size_t capacity;
};

const int BACKEND_ERR_MSG_LEN = 256;

struct backendContext_t {
    file_t srcFile;
    file_t asmFile;

    tree_t* tree;
    dump* treeDump;
    char* astCopyBuffer;

    intVector_t programBuf;

    regInfo_t* regsArr;

    int ifCounter;
    int whileCounter;

    size_t curFuncRSPsubOffset;
    size_t curFuncStackVarsCntr;

    labelVector_t labelsArr;

    backendError_t errCode;
    char errMsg[BACKEND_ERR_MSG_LEN];
}

#endif
