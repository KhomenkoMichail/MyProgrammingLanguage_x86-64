#ifndef BACKEND_DSL_H
#define BACKEND_DSL_H


#define MOV_RC_(reg, imm)                emitMov(cntxt, REG_CONST, reg, NO_REG, imm)
#define MOV_RR_(dst, src)                emitMov(cntxt, REG_REG, dst, src, NO_DISP)
#define MOV_MR_(baseReg, offset, srcReg) emitMov(cntxt, MEM_REG, baseReg, srcReg, offset)
#define MOV_RM_(dstReg, baseReg, offset) emitMov(cntxt, REG_MEM, dstReg, baseReg, offset)


#define MOVZX_RR_(dst, src) emitMovzxRR8(cntxt, dst, src)


#define PUSHR_(reg) emitPushReg(cntxt, reg)
#define POPR_(reg) emitPopReg(cntxt, reg)


#define ADD_RR_(dst, src) emitOpRegReg(cntxt, opCodeADD, dst, src)
#define SUB_RR_(dst, src) emitOpRegReg(cntxt, opCodeSUB, dst, src)
#define IMUL_RR_(dst, src) emitOpRegReg(cntxt, opCodeIMUL, dst, src)
#define TEST_RR_(dst, src) emitOpRegReg(cntxt, opCodeTEST, dst, src)
#define CMP_RR_(dst, src) emitOpRegReg(cntxt, opCodeCMP, dst, src)

#define SUB_RC_(reg, imm) emitAluRegConst(cntxt, reg, imm, aluSUB)
#define ADD_RC_(reg, imm) emitAluRegConst(cntxt, reg, imm, aluADD)


#define CQO emitCqo(cntxt)
#define IDIV_(reg) emitIdiv(cntxt, reg)


#define JMP_(label)  emitPatchCALLorJMP(cntxt, opCodeJMP, false, label)
#define JZ_(label)   emitPatchCALLorJMP(cntxt, opCodeJZ, true, label);
#define CALL_(label) emitPatchCALLorJMP(cntxt, opCodeCALL, false, label)

#define JMP_OFFSET_(offset) emitByte(cntxt, opCodeJMP);\
                            emit_32CurPos(cntxt, offset)

#define LABEL_(name) fprintf(*cntxtAsmFile(cntxt), "%s:\n", name);\
                      addLabelAddressInCntxt(cntxt, name);

#define RET emitRet(cntxt)

#define SETE_(reg)  emitSETcc(cntxt, opCodeSETe, reg)
#define SETNE_(reg) emitSETcc(cntxt, opCodeSETne, reg)
#define SETL_(reg)  emitSETcc(cntxt, opCodeSETl, reg)
#define SETG_(reg)  emitSETcc(cntxt, opCodeSETg, reg)
#define SETLE_(reg) emitSETcc(cntxt, opCodeSETle, reg)
#define SETGE_(reg) emitSETcc(cntxt, opCodeSETge, reg)

#define CVTSI2SD_(dst, src) emitCVTSI2SD(cntxt, dst, src)
#define SQRTSD_(dst, src) emitSQRTSD(cntxt, dst, src)
#define CVTTSD2SI_(dst, src) emitCVTTSD2SI(cntxt, dst, src)

#endif
