#ifndef BACKEND_CONSTS_H
#define BACKEND_CONSTS_H

#define SET_ERR_AND_RETURN(cntxt, code, ...) do { \
    if (cntxt) { \
        (cntxt)->errCode = code; \
        snprintf((cntxt)->errMsg, sizeof((cntxt)->errMsg), __VA_ARGS__); \
    } \
    return code; \
} while(0)

typedef int (*bufFunc_t)(void);

enum backendErr_t {
    BACKEND_SUCCESS = 0,
    BACKEND_ERR_TREE_DUMP_CALLOC = 1,
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
    BACKEND_ERR_UNEXPECTED_NODE_TYPE = 12,
    BACKEND_ERR_OPENING_ASM_FILE = 13,
    BACKEND_ERR_LABELS_ARR_REALLOC = 14,
    BACKEND_ERR_PATCH_ARR_REALLOC = 15,
    BACKEND_ERR_PROGRAM_BUF_REALLOC = 16,
    BACKEND_ERR_OPEN_ELF_FILE = 17,
    BACKEND_ERR_CLOSING_ASM_FILE = 18,
    BACKEND_ERR_CLOSING_ELF_FILE = 19,
};

enum resultReg_t {
    LEFT  = 0,
    RIGHT = 3,
};

const char* const OP_REG_[2] = { "rax", "rbx" };

enum regCode_t {
    NO_REG = -1,
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
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
    regSaveDecl_t regSaveDecl;
};

const regInfo_t INIT_REGS_ARRAY[] = { { "rax", true,  specialSaved },
                                      { "rcx", false, callerSaved  },
                                      { "rdx", false, callerSaved  },
                                      { "rbx", true,  specialSaved },
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

const int NUM_OF_REGS = sizeof(INIT_REGS_ARRAY) / sizeof(regInfo_t);

enum opCode_t {
    opCodeADD = 0x01,
    opCodeSUB = 0x29,
    opCodeCMP = 0x39,
    opCodeTEST = 0x85,
    opCodeIMUL = 0xAF,
    opCodeJZ = 0x84,
    opCodeRET = 0xC3,
    opCodeCQO = 0x99,
    opCodePUSH = 0x50,
    opCodePOP = 0x58,
    opCodeIDIV = 0xF7,
    opCodeMOVsetDir = 0x8B,
    opCodeMOVcleanDir = 0x89,
    opCodeMOVregConst = 0xB8,
    opCodeJMP = 0xE9,
    opCodeSETe = 0x94,
    opCodeSETne = 0x95,
    opCodeSETl = 0x9C,
    opCodeSETg = 0x9F,
    opCodeSETle = 0x9E,
    opCodeSETge = 0x9D,
    opCodeMOVZXrr8 = 0xB6,
    opCodeCALL = 0xE8,
    opCodeCVTSI2SD = 0x2A,
    opCodeSQRTSD = 0x51,
    opCodeCVVTSD2SI = 0x2C,
    opCodeBREAKPOINT = 0xCC,
};

enum aluOpCode_t {
    aluADD = 0,
    aluSUB = 5,
};

const uint8_t ESCAPE_PREFIX = 0x0F;
const uint8_t DOUBLE_PRECISION_PREFIX = 0xF2;

const uint8_t REX_W_BYTE = 0x48;
const uint8_t REX_B_BYTE = 0x41;

const uint8_t XMM0_CODE = 0x00;

const int IDIV_EXTRA_OPCODE = 0x07;
const int NO_INDEX_REG = 0x04;

const int OFFSET_LEN = 4;

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

struct uint8Vector_t {
    uint8_t* buf;
    size_t curSize;
    size_t capacity;
};

struct label_t {
    const char* name;

    bool hasAddress;
    size_t address;

    unsigned long long hash;
    intVector_t patchAddresses;
};

struct labelVector_t {
    label_t* labelArr;
    size_t curSize;
    size_t capacity;
};

const int BACKEND_ERR_MSG_LEN = 256;
const size_t INIT_PROGRAM_BUF_CAPASITY = 1024;

struct backendContext_t {
    const char* elfFileName;
    file_t asmFile;
    sourceFile_t* sourceFile;

    tree_t* tree;
    dump* treeDump;
    char* astCopyBuffer;

    uint8Vector_t programBuf;

    regInfo_t* regsArr;

    int ifCounter;
    int whileCounter;

    size_t curFuncRSPsubOffset;
    size_t curFuncStackVarsCntr;

    labelVector_t labelsArr;

    backendErr_t errCode;
    char errMsg[BACKEND_ERR_MSG_LEN];
};

const size_t INIT_LABELS_ARR_CAPACITY = 64;
const size_t INIT_LABEL_PATCH_ADDRESSES_CAPACITY = 16;

const int64_t NO_DISP = 0xBADBABE;

union varAddrComp_t {
    regCode_t regCode;
    int rbpOffset;
};

struct varPos_t {
    bool inReg;
    varAddrComp_t varrAddrComp;
};

#endif
