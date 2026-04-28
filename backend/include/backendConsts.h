#ifndef BACKEND_CONSTS_H
#define BACKEND_CONSTS_H

    const char* STACK_POINTER_REG = "rsp";
    const char* BASE_POINTER_REG = "rbp";

//    const char* LEFT_OP_REG       = "rax";
//    const char* RIGHT_OP_REG      = "rbx";

    const char* RET_REG           = "rax";

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
    R8  = 8,
    R9  = 9,
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
    specialSaved = 3,   //for rax, rsp and rbp
};

struct regInfo_t {
    const char* name;
    bool isUsed;
    const regSaveDecl_t regSaveDecl;
    bool wasPushed;
};

struct regInfo_t regsArray[] ={ { "rax", true,  specialSaved, false },
                                { "rbx", true, calleeSaved,   false },
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

struct argReg_t {
    const cahr* name;
    const regCode_t regCode;
};

struct argReg_t argRersArray = { { "rdi", RDI },
                                 { "rsi", RSI },
                                 { "rdx", RDX },
                                 { "rcx", RCX },
                                 { "r8",  R8  },
                                 { "r9",  R9  }, };



#endif
