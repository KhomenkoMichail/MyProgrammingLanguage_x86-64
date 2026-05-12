section .text
global _start

_start:
    call main
    call stdExit
main:
push rbp
mov rbp, rsp
sub rsp, 0
mov rbx, 0
mov rcx, rbx
mov rbx, 0
mov rdx, rbx
mov rbx, 0
mov rsi, rbx
mov rbx, 0
mov rdi, rbx
mov rbx, 0
mov r8, rbx
mov rbx, 0
mov r9, rbx
mov rbx, 0
mov r10, rbx
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdIn
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rcx, rax
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdIn
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rdx, rax
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdIn
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rsi, rax
mov rbx, 0
mov rax, rcx
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif1
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
mov rax, rsi
push rax
mov rax, rdx
push rax
call getNumOfLinearRoots
add rsp, 16
mov rbx, rax
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov r8, rbx

mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif2
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
mov rax, rsi
push rax
mov rax, rdx
push rax
call getLinearRoot
add rsp, 16
mov rbx, rax
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov r9, rbx
endif2:
mov rbx, rax
endif1:
mov rbx, 0
mov rax, rcx
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif3
mov rbx, 0
mov rax, rsi
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif4
mov rbx, 0
mov r9, rbx
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
mov rax, rdx
push rax
mov rax, rcx
push rax
call getNumOfLinearRoots
add rsp, 16
mov rbx, rax
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov r8, rbx
mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif5
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
mov rax, rdx
push rax
mov rax, rcx
push rax
call getLinearRoot
add rsp, 16
mov rbx, rax
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov r10, rbx
endif5:
mov rbx, r10
mov rax, r9
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif6
mov rbx, 2
mov r8, rbx
endif6:
mov rbx, rax
endif4:
mov rbx, 0
mov rax, rsi
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif7
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
mov rax, rsi
push rax
mov rax, rdx
push rax
mov rax, rcx
push rax
call getDiscriminant
add rsp, 24
mov rbx, rax
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rdi, rbx
mov rbx, 0
mov rax, rdi
cmp rax, rbx
setl al
movzx rax, al
test rax, rax
jz endif8
mov rbx, 0
mov r8, rbx
endif8:
mov rbx, 0
mov rax, rdi
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif9
mov rbx, 1
mov r8, rbx
mov rax, 0
mov rbx, rdx
sub rax, rbx
push rax
mov rax, 2
mov rbx, rcx
imul rax, rbx
mov rbx, rax
pop rax
push rdx
cqo
idiv rbx
pop rdx
mov rbx, rax
mov r9, rbx
mov rbx, rax
endif9:
mov rbx, 0
mov rax, rdi
cmp rax, rbx
setg al
movzx rax, al
test rax, rax
jz endif10

mov rbx, 2
mov r8, rbx
mov rax, 0
mov rbx, rdx
sub rax, rbx
push rax
mov rax, rdi
cvtsi2sd xmm0, rax
sqrtsd xmm0, xmm0
cvttsd2si rbx, xmm0
pop rax
sub rax, rbx
push rax
mov rax, 2
mov rbx, rcx
imul rax, rbx
mov rbx, rax
pop rax
push rdx
cqo
idiv rbx
pop rdx
mov rbx, rax
mov r9, rbx
mov rax, 0
mov rbx, rdx
sub rax, rbx
push rax
mov rax, rdi
cvtsi2sd xmm0, rax
sqrtsd xmm0, xmm0
cvttsd2si rbx, xmm0
pop rax
add rax, rbx
push rax
mov rax, 2
mov rbx, rcx
imul rax, rbx
mov rbx, rax
pop rax
push rdx
cqo
idiv rbx
pop rdx
mov rbx, rax
mov r10, rbx
mov rbx, rax
endif10:
mov rbx, rax
endif7:
mov rbx, rax
endif3:
mov rax, r8
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdOut
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif11
mov rax, r9
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdOut
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
endif11:
mov rbx, 2
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif12
mov rax, r9
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdOut
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rax, r10
push rcx
push rdx
push rsi
push rdi
push r8
push r9
push r10
call stdOut
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rdx
pop rcx
mov rbx, rax
endif12:
call stdExit
getDiscriminant:
push rbp
mov rbp, rsp
sub rsp, 0
mov rax, [rbp + 24]
mov rbx, [rbp + 24]
imul rax, rbx
push rax
mov rax, 4
mov rbx, [rbp + 16]
imul rax, rbx
mov rbx, [rbp + 32]
imul rax, rbx
mov rbx, rax
pop rax
sub rax, rbx
mov rbx, rax
mov r11, rbx
mov rax, r11
mov rsp, rbp
pop rbp
ret
getNumOfLinearRoots:
push rbp
mov rbp, rsp
sub rsp, 0
mov rbx, 0
mov rax, [rbp + 16]
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif13
mov rbx, 0
mov rax, [rbp + 24]
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif14
mov rax, -1
mov rsp, rbp
pop rbp
ret
endif14:
mov rbx, 0
mov rax, [rbp + 24]
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif15mov rax, 0
mov rsp, rbp
pop rbp
ret
endif15:
mov rbx, rax
endif13:

mov rbx, 0
mov rax, [rbp + 16]
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif16

mov rax, 1
mov rsp, rbp
pop rbp
ret
endif16:mov rax, 0
mov rsp, rbp
pop rbp
ret
getLinearRoot:
push rbp
mov rbp, rsp
sub rsp, 0
mov rax, 0
mov rbx, [rbp + 24]
sub rax, rbx
mov rbx, [rbp + 16]
cqo
idiv rbx
mov rbx, rax
mov rcx, rbx
mov rax, rcx
mov rsp, rbp
pop rbp
ret
%include "./backend/src/stdlib.asm"
