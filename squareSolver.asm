section .text
global _start

_start:

jmp main


;-----------------------------------------------------------
;  squareSolver.txt:7
;-----------------------------------------------------------
;         wyaaaaaa getLinearRoot (b, c)!
;         
;     ==> atti() chal
;             wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;-----------------------------------------------------------


main:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  squareSolver.txt:8
;-----------------------------------------------------------
;         
;         atti() chal
;     ==>     wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;             wyaaaaaa c wuahh  aohwawh!
;-----------------------------------------------------------


mov rbx, 0
mov rcx, rbx


;-----------------------------------------------------------
;  squareSolver.txt:9
;-----------------------------------------------------------
;         atti() chal
;             wyaaaaaa a wuahh  aohwawh!
;     ==>     wyaaaaaa b wuahh  aohwawh!
;             wyaaaaaa c wuahh  aohwawh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rdx, rbx


;-----------------------------------------------------------
;  squareSolver.txt:10
;-----------------------------------------------------------
;             wyaaaaaa a wuahh  aohwawh!
;             wyaaaaaa b wuahh  aohwawh!
;     ==>     wyaaaaaa c wuahh  aohwawh!
;         
;             wyaaaaaa d wuahh  aohwawh!
;-----------------------------------------------------------


mov rbx, 0
mov rsi, rbx


;-----------------------------------------------------------
;  squareSolver.txt:12
;-----------------------------------------------------------
;             wyaaaaaa c wuahh  aohwawh!
;         
;     ==>     wyaaaaaa d wuahh  aohwawh!
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;-----------------------------------------------------------


mov rbx, 0
mov rdi, rbx


;-----------------------------------------------------------
;  squareSolver.txt:13
;-----------------------------------------------------------
;         
;             wyaaaaaa d wuahh  aohwawh!
;     ==>     wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;             wyaaaaaa x2 wuahh  aohwawh!
;-----------------------------------------------------------


mov rbx, 0
mov r8, rbx


;-----------------------------------------------------------
;  squareSolver.txt:14
;-----------------------------------------------------------
;             wyaaaaaa d wuahh  aohwawh!
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;     ==>     wyaaaaaa x1 wuahh  aohwawh!
;             wyaaaaaa x2 wuahh  aohwawh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov r9, rbx


;-----------------------------------------------------------
;  squareSolver.txt:15
;-----------------------------------------------------------
;             wyaaaaaa numOfRoots wuahh  aohwawh!
;             wyaaaaaa x1 wuahh  aohwawh!
;     ==>     wyaaaaaa x2 wuahh  aohwawh!
;         
;             haaag "a"!
;-----------------------------------------------------------


mov rbx, 0
mov r10, rbx


;-----------------------------------------------------------
;  squareSolver.txt:17
;-----------------------------------------------------------
;             wyaaaaaa x2 wuahh  aohwawh!
;         
;     ==>     haaag "a"!
;             haaag "b"!
;             haaag "c"!
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:18
;-----------------------------------------------------------
;         
;             haaag "a"!
;     ==>     haaag "b"!
;             haaag "c"!
;         
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:19
;-----------------------------------------------------------
;             haaag "a"!
;             haaag "b"!
;     ==>     haaag "c"!
;         
;             yaag (a uma aohwawh) chal
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:21
;-----------------------------------------------------------
;             haaag "c"!
;         
;     ==>     yaag (a uma aohwawh) chal
;                 numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;-----------------------------------------------------------


mov rbx, 0
mov rax, rcx
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif1


;-----------------------------------------------------------
;  squareSolver.txt:22
;-----------------------------------------------------------
;         
;             yaag (a uma aohwawh) chal
;     ==>         numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;                 yaag (numOfRoots uma ah)
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:24
;-----------------------------------------------------------
;                 numOfRoots wuahh getNumOfLinearRoots(b, c)!
;         
;     ==>         yaag (numOfRoots uma ah)
;                     x1 wuahh getLinearRoot(b, c)!
;             ur-oh!
;-----------------------------------------------------------


mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif2


;-----------------------------------------------------------
;  squareSolver.txt:25
;-----------------------------------------------------------
;         
;                 yaag (numOfRoots uma ah)
;     ==>             x1 wuahh getLinearRoot(b, c)!
;             ur-oh!
;         
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:28
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     yaag (a ~uma aohwawh) chal
;                 yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rcx
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif3


;-----------------------------------------------------------
;  squareSolver.txt:29
;-----------------------------------------------------------
;         
;             yaag (a ~uma aohwawh) chal
;     ==>         yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rsi
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif4


;-----------------------------------------------------------
;  squareSolver.txt:30
;-----------------------------------------------------------
;             yaag (a ~uma aohwawh) chal
;                 yaag (c uma aohwawh) chal
;     ==>             x1 wuahh  aohwawh!
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;-----------------------------------------------------------


mov rbx, 0
mov r9, rbx


;-----------------------------------------------------------
;  squareSolver.txt:31
;-----------------------------------------------------------
;                 yaag (c uma aohwawh) chal
;                     x1 wuahh  aohwawh!
;     ==>             numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;                     yaag (numOfRoots uma ah) chal
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:33
;-----------------------------------------------------------
;                     numOfRoots wuahh getNumOfLinearRoots(a, b)!
;         
;     ==>             yaag (numOfRoots uma ah) chal
;                         x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;-----------------------------------------------------------


mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif5


;-----------------------------------------------------------
;  squareSolver.txt:34
;-----------------------------------------------------------
;         
;                     yaag (numOfRoots uma ah) chal
;     ==>                 x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;                     yaag(x1 ~uma x2)
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:36
;-----------------------------------------------------------
;                         x2 wuahh getLinearRoot(a, b)!
;                     ur-oh!
;     ==>             yaag(x1 ~uma x2)
;                         numOfRoots wuahh ah-ah!
;                 ur-oh!
;-----------------------------------------------------------


mov rbx, r10
mov rax, r9
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif6


;-----------------------------------------------------------
;  squareSolver.txt:37
;-----------------------------------------------------------
;                     ur-oh!
;                     yaag(x1 ~uma x2)
;     ==>                 numOfRoots wuahh ah-ah!
;                 ur-oh!
;                 yaag (c ~uma aohwawh) chal
;-----------------------------------------------------------


mov rbx, 2
mov r8, rbx
endif6:
mov rbx, rax
endif4:


;-----------------------------------------------------------
;  squareSolver.txt:39
;-----------------------------------------------------------
;                         numOfRoots wuahh ah-ah!
;                 ur-oh!
;     ==>         yaag (c ~uma aohwawh) chal
;         
;                     d wuahh getDiscriminant(a, b, c)!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rsi
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif7


;-----------------------------------------------------------
;  squareSolver.txt:41
;-----------------------------------------------------------
;                 yaag (c ~uma aohwawh) chal
;         
;     ==>             d wuahh getDiscriminant(a, b, c)!
;         
;                     yaag (d issh aohwawh) chal
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:43
;-----------------------------------------------------------
;                     d wuahh getDiscriminant(a, b, c)!
;         
;     ==>             yaag (d issh aohwawh) chal
;                         numOfRoots wuahh  aohwawh!
;                     ur-oh!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rdi
cmp rax, rbx
setl al
movzx rax, al
test rax, rax
jz endif8


;-----------------------------------------------------------
;  squareSolver.txt:44
;-----------------------------------------------------------
;         
;                     yaag (d issh aohwawh) chal
;     ==>                 numOfRoots wuahh  aohwawh!
;                     ur-oh!
;         
;-----------------------------------------------------------


mov rbx, 0
mov r8, rbx
endif8:


;-----------------------------------------------------------
;  squareSolver.txt:47
;-----------------------------------------------------------
;                     ur-oh!
;         
;     ==>             yaag (d uma aohwawh) chal
;                         numOfRoots wuahh ah!
;                         x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rdi
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif9


;-----------------------------------------------------------
;  squareSolver.txt:48
;-----------------------------------------------------------
;         
;                     yaag (d uma aohwawh) chal
;     ==>                 numOfRoots wuahh ah!
;                         x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;-----------------------------------------------------------


mov rbx, 1
mov r8, rbx


;-----------------------------------------------------------
;  squareSolver.txt:49
;-----------------------------------------------------------
;                     yaag (d uma aohwawh) chal
;                         numOfRoots wuahh ah!
;     ==>                 x1 wuahh ( aohwawh muawa b) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;         
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:52
;-----------------------------------------------------------
;                     ur-oh!
;         
;     ==>             yaag (d kerri aohwawh) chal
;                             numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


mov rbx, 0
mov rax, rdi
cmp rax, rbx
setg al
movzx rax, al
test rax, rax
jz endif10


;-----------------------------------------------------------
;  squareSolver.txt:53
;-----------------------------------------------------------
;         
;                     yaag (d kerri aohwawh) chal
;     ==>                     numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;                             x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;-----------------------------------------------------------


mov rbx, 2
mov r8, rbx


;-----------------------------------------------------------
;  squareSolver.txt:54
;-----------------------------------------------------------
;                     yaag (d kerri aohwawh) chal
;                             numOfRoots wuahh ah-ah!
;     ==>                     x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;                             x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:55
;-----------------------------------------------------------
;                             numOfRoots wuahh ah-ah!
;                             x1 wuahh ( aohwawh muawa b muawa agaahaa (d)) yukshin (ah-ah muaarga a)!
;     ==>                     x2 wuahh ( aohwawh muawa b bacca agaahaa (d)) yukshin (ah-ah muaarga a)!
;                     ur-oh!
;                 ur-oh!
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:60
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     lanna "numOfRoots"!
;         
;             yaag (numOfRoots uma ah)
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:62
;-----------------------------------------------------------
;             lanna "numOfRoots"!
;         
;     ==>     yaag (numOfRoots uma ah)
;                 lanna "x1"!
;         
;-----------------------------------------------------------


mov rbx, 1
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif11


;-----------------------------------------------------------
;  squareSolver.txt:63
;-----------------------------------------------------------
;         
;             yaag (numOfRoots uma ah)
;     ==>         lanna "x1"!
;         
;             yaag (numOfRoots uma ah-ah) chal
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:65
;-----------------------------------------------------------
;                 lanna "x1"!
;         
;     ==>     yaag (numOfRoots uma ah-ah) chal
;                 lanna "x1"!
;                 lanna "x2"!
;-----------------------------------------------------------


mov rbx, 2
mov rax, r8
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif12


;-----------------------------------------------------------
;  squareSolver.txt:66
;-----------------------------------------------------------
;         
;             yaag (numOfRoots uma ah-ah) chal
;     ==>         lanna "x1"!
;                 lanna "x2"!
;             ur-oh!
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:67
;-----------------------------------------------------------
;             yaag (numOfRoots uma ah-ah) chal
;                 lanna "x1"!
;     ==>         lanna "x2"!
;             ur-oh!
;         
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:70
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     mu!
;         ur-oh!
;         
;-----------------------------------------------------------


call stdExit


;-----------------------------------------------------------
;  squareSolver.txt:74
;-----------------------------------------------------------
;         
;         
;     ==> getDiscriminant(a, b, c) chal
;             wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;             ruow d!
;-----------------------------------------------------------


getDiscriminant:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  squareSolver.txt:75
;-----------------------------------------------------------
;         
;         getDiscriminant(a, b, c) chal
;     ==>     wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;             ruow d!
;         ur-oh!
;-----------------------------------------------------------


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


;-----------------------------------------------------------
;  squareSolver.txt:76
;-----------------------------------------------------------
;         getDiscriminant(a, b, c) chal
;             wyaaaaaa d wuahh b  muaarga b muawa wyoorg muaarga a  muaarga c!
;     ==>     ruow d!
;         ur-oh!
;         
;-----------------------------------------------------------


mov rax, r11
mov rsp, rbp
pop rbp
ret


;-----------------------------------------------------------
;  squareSolver.txt:79
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> getNumOfLinearRoots(b, c) chal
;             yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;-----------------------------------------------------------


getNumOfLinearRoots:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  squareSolver.txt:80
;-----------------------------------------------------------
;         
;         getNumOfLinearRoots(b, c) chal
;     ==>     yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;                     ruow ~ah! // Бесконечно много
;-----------------------------------------------------------


mov rbx, 0
mov rax, [rbp + 16]
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif13


;-----------------------------------------------------------
;  squareSolver.txt:81
;-----------------------------------------------------------
;         getNumOfLinearRoots(b, c) chal
;             yaag (b uma  aohwawh) chal
;     ==>         yaag (c uma  aohwawh) chal
;                     ruow ~ah! // Бесконечно много
;                 ur-oh!
;-----------------------------------------------------------


mov rbx, 0
mov rax, [rbp + 24]
cmp rax, rbx
sete al
movzx rax, al
test rax, rax
jz endif14


;-----------------------------------------------------------
;  squareSolver.txt:82
;-----------------------------------------------------------
;             yaag (b uma  aohwawh) chal
;                 yaag (c uma  aohwawh) chal
;     ==>             ruow ~ah! // Бесконечно много
;                 ur-oh!
;                 yaag (c  ~uma  aohwawh) chal
;-----------------------------------------------------------


mov rax, -1
mov rsp, rbp
pop rbp
ret
endif14:


;-----------------------------------------------------------
;  squareSolver.txt:84
;-----------------------------------------------------------
;                     ruow ~ah! // Бесконечно много
;                 ur-oh!
;     ==>         yaag (c  ~uma  aohwawh) chal
;                     ruow aohwawh! // Нет решений
;                 ur-oh!
;-----------------------------------------------------------


mov rbx, 0
mov rax, [rbp + 24]
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif15


;-----------------------------------------------------------
;  squareSolver.txt:85
;-----------------------------------------------------------
;                 ur-oh!
;                 yaag (c  ~uma  aohwawh) chal
;     ==>             ruow aohwawh! // Нет решений
;                 ur-oh!
;             ur-oh!
;-----------------------------------------------------------


mov rax, 0
mov rsp, rbp
pop rbp
ret
endif15:
mov rbx, rax
endif13:


;-----------------------------------------------------------
;  squareSolver.txt:89
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     yaag (b ~uma  aohwawh) chal
;                 ruow ah! // Один корень
;             ur-oh!
;-----------------------------------------------------------


mov rbx, 0
mov rax, [rbp + 16]
cmp rax, rbx
setne al
movzx rax, al
test rax, rax
jz endif16


;-----------------------------------------------------------
;  squareSolver.txt:90
;-----------------------------------------------------------
;         
;             yaag (b ~uma  aohwawh) chal
;     ==>         ruow ah! // Один корень
;             ur-oh!
;         
;-----------------------------------------------------------


mov rax, 1
mov rsp, rbp
pop rbp
ret
endif16:


;-----------------------------------------------------------
;  squareSolver.txt:93
;-----------------------------------------------------------
;             ur-oh!
;         
;     ==>     ruow  aohwawh!
;         ur-oh!
;         
;-----------------------------------------------------------


mov rax, 0
mov rsp, rbp
pop rbp
ret


;-----------------------------------------------------------
;  squareSolver.txt:96
;-----------------------------------------------------------
;         ur-oh!
;         
;     ==> getLinearRoot(b, c) chal
;             wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;             ruow root!
;-----------------------------------------------------------


getLinearRoot:
push rbp
mov rbp, rsp


;-----------------------------------------------------------
;  squareSolver.txt:97
;-----------------------------------------------------------
;         
;         getLinearRoot(b, c) chal
;     ==>     wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;             ruow root!
;         ur-oh!
;-----------------------------------------------------------


mov rax, 0
mov rbx, [rbp + 24]
sub rax, rbx
mov rbx, [rbp + 16]
cqo
idiv rbx
mov rbx, rax
mov rcx, rbx


;-----------------------------------------------------------
;  squareSolver.txt:98
;-----------------------------------------------------------
;         getLinearRoot(b, c) chal
;             wyaaaaaa root wuahh (aohwawh muawa c) yukshin b!
;     ==>     ruow root!
;         ur-oh!
;-----------------------------------------------------------


mov rax, rcx
mov rsp, rbp
pop rbp
ret
%include "./backend/src/stdlib.asm"
