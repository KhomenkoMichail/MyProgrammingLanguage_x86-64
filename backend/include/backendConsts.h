#ifndef BACKEND_CONSTS_H
#define BACKEND_CONSTS_H

#define SET_ERR_AND_RETURN(cntxt, errCode, ...) do { \
    if (cntxt) { \
        (cntxt)->errCode = errCode; \
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
    BACKEND_ERR_LABELS_ARR_CTOR = 5,
    BACKEND_ERR_PROGRAM_BUF_CALLOC = 6,
    BACKEND_ERR_SRC_FILE_STRUCT_CALLOC = 7,
    BACKEND_ERR_NODE_TYPE = 8,
    BACKEND_ERR_UNEXPECTED_OPCODE = 9,
    BACKEND_ERR_NO_RIGHT_NODE = 10,
    BACKEND_ERR_NO_LEFT_NODE = 11,
};

enum resultReg_t {
    LEFT  = 0,
    RIGHT = 1,
};

const char* OP_REG_[2] = { "rax", "rbx" };

enum regCode_t {
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
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15
};

const size_t NUM_OF_VARS_REGS = 12;

enum regSaveDecl_t {
    callerSaved  = 1,
    calleeSaved  = 2,
    specialSaved = 3,
};

struct regInfo_t {
    const char* name;
    bool isUsed;
    const regSaveDecl_t regSaveDecl;
};

const regInfo_t INIT_REGS_ARRAY[] = { { "rax", true,  specialSaved },
                                      { "rbx", true,  calleeSaved  },
                                      { "rcx", false, callerSaved  },
                                      { "rdx", false, callerSaved  },
                                      { "rsp", true,  specialSaved },
                                      { "rbp", true,  specialSaved },
                                      { "rsi", false, callerSaved  },
                                      { "rdi", false, callerSaved  },
                                      { "r8",  false, callerSaved  },
                                      { "r9",  false, callerSaved  },
                                      { "r10", false, callerSaved  },
                                      { "r11", false, callerSaved  },
                                      { "r12", false, calleeSaved  },
                                      { "r13", false, calleeSaved  },
                                      { "r14", false, calleeSaved  },
                                      { "r15", false, calleeSaved  }, };

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
    intVector_t patchOffsets;
};

struct labelVector_t {
    label_t* labelArr;
    size_t curSize;
    size_t capacity;
};

const int BACKEND_ERR_MSG_LEN = 256;
const size_t INIT_PROGRAM_BUF_CAPASITY = 1024;

struct backendContext_t {
    file_t srcFile;
    sourceFile_t* sourceFile;

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
};

size_t INIT_LABELS_ARR_CAPACITY = 64;
size_t INIT_LABEL_PATCH_OFFSETS_CAPACITY = 16;

#endif
