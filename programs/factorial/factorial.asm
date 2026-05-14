section .text
global _start

_start:

jmp main


;-----------------------------------------------------------
;  factorial.txt:3
;-----------------------------------------------------------
;         wyaaaaaa factorial(n)!
;         
;     ==> atti() chal
;             wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa fact wuahh  aohwawh!
;-----------------------------------------------------------


main:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  factorial.txt:4
;-----------------------------------------------------------
;         
;         atti() chal
;     ==>     wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa fact wuahh  aohwawh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rcx, rbx


;-----------------------------------------------------------
;  factorial.txt:5
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa n wuahh aohwawh!
;     ==>     wyaaaaaa fact wuahh  aohwawh!
;         
;             haaag "n"!
;-----------------------------------------------------------


mov rbx, 0
mov rdx, rbx


;-----------------------------------------------------------
;  factorial.txt:7
;-----------------------------------------------------------
;             wyaaaaaa fact wuahh  aohwawh!
;         
;     ==>     haaag "n"!
;             fact wuahh factorial(n)!
;         
;-----------------------------------------------------------


push rcx
push rdx
call stdIn
pop rdx
pop rcx
mov rcx, rax


;-----------------------------------------------------------
;  factorial.txt:8
;-----------------------------------------------------------
;         
;             haaag "n"!
;     ==>     fact wuahh factorial(n)!
;         
;             lanna "fact"!
;-----------------------------------------------------------


push rcx
push rdx
mov rax, rcx
push rax
call factorial
add rsp, 8
mov rbx, rax
pop rdx
pop rcx
mov rdx, rbx


;-----------------------------------------------------------
;  factorial.txt:10
;-----------------------------------------------------------
;             fact wuahh factorial(n)!
;         
;     ==>     lanna "fact"!
;             mu!
;         ur-oh!
;-----------------------------------------------------------


mov rax, rdx
push rcx
push rdx
call stdOut
pop rdx
pop rcx


;-----------------------------------------------------------
;  factorial.txt:11
;-----------------------------------------------------------
;         
;             lanna "fact"!
;     ==>     mu!
;         ur-oh!
;         
;-----------------------------------------------------------


call stdExit


;-----------------------------------------------------------
;  factorial.txt:14
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> factorial(n) chal
;             yaag (n uma ah)
;                 ruow ah!
;-----------------------------------------------------------


factorial:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  factorial.txt:15
;-----------------------------------------------------------
;         
;         factorial(n) chal
;     ==>     yaag (n uma ah)
;                 ruow ah!
;             yaag (n ~uma ah)
;-----------------------------------------------------------


mov rax, [rbp + 16]
mov rbx, 1
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif1


;-----------------------------------------------------------
;  factorial.txt:16
;-----------------------------------------------------------
;         factorial(n) chal
;             yaag (n uma ah)
;     ==>         ruow ah!
;             yaag (n ~uma ah)
;                 ruow n muaarga factorial(n muawa ah)!
;-----------------------------------------------------------


mov rax, 1
mov rsp, rbp
pop rbp
ret
endif1:


;-----------------------------------------------------------
;  factorial.txt:17
;-----------------------------------------------------------
;             yaag (n uma ah)
;                 ruow ah!
;     ==>     yaag (n ~uma ah)
;                 ruow n muaarga factorial(n muawa ah)!
;         ur-oh!
;-----------------------------------------------------------


mov rax, [rbp + 16]
mov rbx, 1
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif2


;-----------------------------------------------------------
;  factorial.txt:18
;-----------------------------------------------------------
;                 ruow ah!
;             yaag (n ~uma ah)
;     ==>         ruow n muaarga factorial(n muawa ah)!
;         ur-oh!
;-----------------------------------------------------------


mov rax, [rbp + 16]
push rax
mov rax, [rbp + 16]
mov rbx, 1
sub rax, rbx
push rax
call factorial
add rsp, 8
mov rbx, rax
pop rax
imul rax, rbx
mov rsp, rbp
pop rbp
ret
endif2:
%include "./backend/src/stdlib.asm"
