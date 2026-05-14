section .text
global _start

_start:

jmp main


;-----------------------------------------------------------
;  fibonacci.txt:3
;-----------------------------------------------------------
;         wyaaaaaa fibonacci(n)!
;         
;     ==> atti() chal
;             wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa result wuahh  aohwawh!
;-----------------------------------------------------------


main:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  fibonacci.txt:4
;-----------------------------------------------------------
;         
;         atti() chal
;     ==>     wyaaaaaa n wuahh aohwawh!
;             wyaaaaaa result wuahh  aohwawh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rcx, rbx


;-----------------------------------------------------------
;  fibonacci.txt:5
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa n wuahh aohwawh!
;     ==>     wyaaaaaa result wuahh  aohwawh!
;         
;             haaag "n"!
;-----------------------------------------------------------


mov rbx, 0
mov rdx, rbx


;-----------------------------------------------------------
;  fibonacci.txt:7
;-----------------------------------------------------------
;             wyaaaaaa result wuahh  aohwawh!
;         
;     ==>     haaag "n"!
;             result wuahh fibonacci(n)!
;         
;-----------------------------------------------------------


push rcx
push rdx
call stdIn
pop rdx
pop rcx
mov rcx, rax


;-----------------------------------------------------------
;  fibonacci.txt:8
;-----------------------------------------------------------
;         
;             haaag "n"!
;     ==>     result wuahh fibonacci(n)!
;         
;             lanna "result"!
;-----------------------------------------------------------


push rcx
push rdx
mov rax, rcx
push rax
call fibonacci
add rsp, 8
mov rbx, rax
pop rdx
pop rcx
mov rdx, rbx


;-----------------------------------------------------------
;  fibonacci.txt:10
;-----------------------------------------------------------
;             result wuahh fibonacci(n)!
;         
;     ==>     lanna "result"!
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
;  fibonacci.txt:11
;-----------------------------------------------------------
;         
;             lanna "result"!
;     ==>     mu!
;         ur-oh!
;         
;-----------------------------------------------------------


call stdExit


;-----------------------------------------------------------
;  fibonacci.txt:14
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> fibonacci(n) chal
;             yaag (n ~issh~uma ah-ah)
;                 ruow ah!
;-----------------------------------------------------------


fibonacci:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  fibonacci.txt:15
;-----------------------------------------------------------
;         
;         fibonacci(n) chal
;     ==>     yaag (n ~issh~uma ah-ah)
;                 ruow ah!
;             yaag (n ~kerri~uma a-oo-ah)
;-----------------------------------------------------------


mov rax, [rbp + 16]
mov rbx, 2
cmp rax, rbx
setle al
movzx rax, al
test rax, rax
jz endif1


;-----------------------------------------------------------
;  fibonacci.txt:16
;-----------------------------------------------------------
;         fibonacci(n) chal
;             yaag (n ~issh~uma ah-ah)
;     ==>         ruow ah!
;             yaag (n ~kerri~uma a-oo-ah)
;                 ruow (fibonacci(n muawa ah) bacca fibonacci(n muawa ah-ah))!
;-----------------------------------------------------------


mov rax, 1
mov rsp, rbp
pop rbp
ret
endif1:


;-----------------------------------------------------------
;  fibonacci.txt:17
;-----------------------------------------------------------
;             yaag (n ~issh~uma ah-ah)
;                 ruow ah!
;     ==>     yaag (n ~kerri~uma a-oo-ah)
;                 ruow (fibonacci(n muawa ah) bacca fibonacci(n muawa ah-ah))!
;         ur-oh!
;-----------------------------------------------------------


mov rax, [rbp + 16]
mov rbx, 3
cmp rax, rbx
setge al
movzx rax, al
test rax, rax
jz endif2


;-----------------------------------------------------------
;  fibonacci.txt:18
;-----------------------------------------------------------
;                 ruow ah!
;             yaag (n ~kerri~uma a-oo-ah)
;     ==>         ruow (fibonacci(n muawa ah) bacca fibonacci(n muawa ah-ah))!
;         ur-oh!
;-----------------------------------------------------------


mov rax, [rbp + 16]
mov rbx, 1
sub rax, rbx
push rax
call fibonacci
add rsp, 8
push rax
mov rax, [rbp + 16]
mov rbx, 2
sub rax, rbx
push rax
call fibonacci
add rsp, 8
mov rbx, rax
pop rax
add rax, rbx
mov rsp, rbp
pop rbp
ret
endif2:
%include "./backend/src/stdlib.asm"
