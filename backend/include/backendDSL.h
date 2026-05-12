#ifndef BACKEND_DSL_H
#define BACKEND_DSL_H

#define MOV_RC(reg, imm) emitMov(cntxt, REG_CONST, (regCode_t)reg, NO_REG, imm);

#define JMP_(label) emitByte(cntxt, opCodeJMP);\\
                    patchCurLabel(cntxt, label);

#endif
