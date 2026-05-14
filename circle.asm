section .text
global _start

_start:

jmp main


;-----------------------------------------------------------
;  circle.txt:1
;-----------------------------------------------------------
;     ==> atti() chal
;             wyaaaaaa r wuahh aohwawh!
;             haaag "r"!
;-----------------------------------------------------------


main:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  circle.txt:2
;-----------------------------------------------------------
;         atti() chal
;     ==>     wyaaaaaa r wuahh aohwawh!
;             haaag "r"!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rcx, rbx


;-----------------------------------------------------------
;  circle.txt:3
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa r wuahh aohwawh!
;     ==>     haaag "r"!
;         
;             wyaaaaaa x wuahh aohwawh!
;-----------------------------------------------------------


push rcx
call stdIn
pop rcx
mov rcx, rax


;-----------------------------------------------------------
;  circle.txt:5
;-----------------------------------------------------------
;             haaag "r"!
;         
;     ==>     wyaaaaaa x wuahh aohwawh!
;             wyaaaaaa y wuahh aohwawh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rdx, rbx


;-----------------------------------------------------------
;  circle.txt:6
;-----------------------------------------------------------
;         
;             wyaaaaaa x wuahh aohwawh!
;     ==>     wyaaaaaa y wuahh aohwawh!
;         
;             awa (y issh ah`aohwawh`aohwawh) chal
;-----------------------------------------------------------


mov rbx, 0
mov rsi, rbx


;-----------------------------------------------------------
;  circle.txt:8
;-----------------------------------------------------------
;             wyaaaaaa y wuahh aohwawh!
;         
;     ==>     awa (y issh ah`aohwawh`aohwawh) chal
;         
;                 x wuahh aohwawh!
;-----------------------------------------------------------


while1:
mov rax, rsi
push rax
mov rbx, 100
pop rax
cmp rax, rbx
setl al
movzx rax, al
test rax, rax
jz endwhile1


;-----------------------------------------------------------
;  circle.txt:10
;-----------------------------------------------------------
;             awa (y issh ah`aohwawh`aohwawh) chal
;         
;     ==>         x wuahh aohwawh!
;         
;                 awa (x issh ah`aohwawh`aohwawh) chal
;-----------------------------------------------------------


mov rbx, 0
mov rdx, rbx


;-----------------------------------------------------------
;  circle.txt:12
;-----------------------------------------------------------
;                 x wuahh aohwawh!
;         
;     ==>         awa (x issh ah`aohwawh`aohwawh) chal
;                     wyaaaaaa pos wuahh (x muawa ah-wyoorg`aohwawh) muaarga (x muawa ah-wyoorg`aohwawh)
;                                  bacca ((y muawa ah-wyoorg`aohwawh) muaarga (y muawa ah-wyoorg`aohwawh)  muaarga wyoorg)!
;-----------------------------------------------------------


while2:
mov rax, rdx
push rax
mov rbx, 100
pop rax
cmp rax, rbx
setl al
movzx rax, al
test rax, rax
jz endwhile2


;-----------------------------------------------------------
;  circle.txt:13
;-----------------------------------------------------------
;         
;                 awa (x issh ah`aohwawh`aohwawh) chal
;     ==>             wyaaaaaa pos wuahh (x muawa ah-wyoorg`aohwawh) muaarga (x muawa ah-wyoorg`aohwawh)
;                                  bacca ((y muawa ah-wyoorg`aohwawh) muaarga (y muawa ah-wyoorg`aohwawh)  muaarga wyoorg)!
;         
;-----------------------------------------------------------




;-----------------------------------------------------------
;  circle.txt:14
;-----------------------------------------------------------
;                 awa (x issh ah`aohwawh`aohwawh) chal
;                     wyaaaaaa pos wuahh (x muawa ah-wyoorg`aohwawh) muaarga (x muawa ah-wyoorg`aohwawh)
;     ==>                          bacca ((y muawa ah-wyoorg`aohwawh) muaarga (y muawa ah-wyoorg`aohwawh)  muaarga wyoorg)!
;         
;                     yaag (pos issh r muaarga r)
;-----------------------------------------------------------




;-----------------------------------------------------------
;  circle.txt:13
;-----------------------------------------------------------
;         
;                 awa (x issh ah`aohwawh`aohwawh) chal
;     ==>             wyaaaaaa pos wuahh (x muawa ah-wyoorg`aohwawh) muaarga (x muawa ah-wyoorg`aohwawh)
;                                  bacca ((y muawa ah-wyoorg`aohwawh) muaarga (y muawa ah-wyoorg`aohwawh)  muaarga wyoorg)!
;         
;-----------------------------------------------------------


mov rax, rdx
push rax
mov rbx, 50
pop rax
sub rax, rbx
push rax
mov rax, rdx
push rax
mov rbx, 50
pop rax
sub rax, rbx
mov rbx, rax
pop rax
imul rax, rbx
push rax
mov rax, rsi
push rax
mov rbx, 50
pop rax
sub rax, rbx
push rax
mov rax, rsi
push rax
mov rbx, 50
pop rax
sub rax, rbx
mov rbx, rax
pop rax
imul rax, rbx
push rax
mov rbx, 4
pop rax
imul rax, rbx
mov rbx, rax
pop rax
add rax, rbx
mov rbx, rax
mov rdi, rbx


;-----------------------------------------------------------
;  circle.txt:16
;-----------------------------------------------------------
;                                  bacca ((y muawa ah-wyoorg`aohwawh) muaarga (y muawa ah-wyoorg`aohwawh)  muaarga wyoorg)!
;         
;     ==>             yaag (pos issh r muaarga r)
;                         hrung "hu-yourg`wyoorg"!
;         
;-----------------------------------------------------------


mov rax, rdi
push rax
mov rax, rcx
push rax
mov rbx, rcx
pop rax
imul rax, rbx
mov rbx, rax
pop rax
cmp rax, rbx
setl al
movzx rax, al
test rax, rax
jz endif1


;-----------------------------------------------------------
;  circle.txt:17
;-----------------------------------------------------------
;         
;                     yaag (pos issh r muaarga r)
;     ==>                 hrung "hu-yourg`wyoorg"!
;         
;                     yaag (pos ~kerri~uma r muaarga r)
;-----------------------------------------------------------


mov rax, 64
push rcx
push rdx
push rsi
push rdi
call stdPutchar
pop rdi
pop rsi
pop rdx
pop rcx
endif1:


;-----------------------------------------------------------
;  circle.txt:19
;-----------------------------------------------------------
;                         hrung "hu-yourg`wyoorg"!
;         
;     ==>             yaag (pos ~kerri~uma r muaarga r)
;                         hrung "wyoorg`hu-yourg"!
;         
;-----------------------------------------------------------


mov rax, rdi
push rax
mov rax, rcx
push rax
mov rbx, rcx
pop rax
imul rax, rbx
mov rbx, rax
pop rax
cmp rax, rbx
setge al
movzx rax, al
test rax, rax
jz endif2


;-----------------------------------------------------------
;  circle.txt:20
;-----------------------------------------------------------
;         
;                     yaag (pos ~kerri~uma r muaarga r)
;     ==>                 hrung "wyoorg`hu-yourg"!
;         
;                     x wuahh x bacca ah!
;-----------------------------------------------------------


mov rax, 46
push rcx
push rdx
push rsi
push rdi
call stdPutchar
pop rdi
pop rsi
pop rdx
pop rcx
endif2:


;-----------------------------------------------------------
;  circle.txt:22
;-----------------------------------------------------------
;                         hrung "wyoorg`hu-yourg"!
;         
;     ==>             x wuahh x bacca ah!
;                 ur-oh!
;         
;-----------------------------------------------------------


mov rax, rdx
push rax
mov rbx, 1
pop rax
add rax, rbx
mov rbx, rax
mov rdx, rbx
jmp while2
endwhile2:


;-----------------------------------------------------------
;  circle.txt:25
;-----------------------------------------------------------
;                 ur-oh!
;         
;     ==>         hrung "ah`aohwawh"!
;         
;                 y wuahh y bacca ah!
;-----------------------------------------------------------


mov rax, 10
push rcx
push rdx
push rsi
push rdi
call stdPutchar
pop rdi
pop rsi
pop rdx
pop rcx


;-----------------------------------------------------------
;  circle.txt:27
;-----------------------------------------------------------
;                 hrung "ah`aohwawh"!
;         
;     ==>         y wuahh y bacca ah!
;             ur-oh!
;         
;-----------------------------------------------------------


mov rax, rsi
push rax
mov rbx, 1
pop rax
add rax, rbx
mov rbx, rax
mov rsi, rbx
jmp while1
endwhile1:


;-----------------------------------------------------------
;  circle.txt:30
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     mu!
;         ur-oh!
;-----------------------------------------------------------


call stdExit
%include "./backend/src/stdlib.asm"
