%macro PUT_CHAR 1
    mov al, %1
    stosb
    inc r11
%endmacro

INT_MAX_LEN EQU 21

section .text
;----------------------------------------------------------------------------------------------
;Exit the program.
;----------------------------------------------------------------------------------------------
stdExit:
                        mov rax, 60
                        xor rdi, rdi
                        syscall

;----------------------------------------------------------------------------------------------
;Outputs a number in decimal notation.
;Entry: rax = output number
;Exit:
;Expected:
;Destroyed: rax, rcx, rdx, rdi, rsi, r9, r11
;----------------------------------------------------------------------------------------------
stdOut:
                        push rbp
                        mov rbp, rsp
                        cld

                        sub rsp, INT_MAX_LEN
                        mov rdi, rsp

                        xor r11, r11                        ; curBufSize

                        test rax, rax
                        jnz .notZero

                        PUT_CHAR '0'
                        jmp .end

.notZero:
                        test rax, rax
                        jns .isPositive

                        neg rax
                        PUT_CHAR '-'

.isPositive:
                        xor rcx, rcx                        ; rcx is a numCounter
                        mov r9, 10                          ; divider

.nextNum:
                        xor rdx, rdx
                        div r9

                        push rdx                            ; push remainder
                        inc rcx

                        test rax, rax                       ; while (quotient  != 0)
                        jnz .nextNum


.printNum:
                        pop rax                             ; pop remainder
                        add al, '0'                         ; num to ASCII
                        PUT_CHAR al

                        dec rcx                             ; numCounter--
                        test rcx, rcx
                        jnz .printNum

.end:

                        mov rax, 1                                 ; write
                        mov rdi, 1                                 ; stdout

                        lea rsi, [rbp - INT_MAX_LEN]               ; buffer address
                        mov rdx, r11                               ; buffer size
                        syscall

                        mov rsp, rbp
                        pop rbp
                        ret

;----------------------------------------------------------------------------------------------
;Assigns the decimal number read from stdin to the RAX register.
;Entry:
;Exit: rax = entered number
;Expected:
;Destroyed: rax, rcx, rdx, rdi, rsi, r8, r9
;----------------------------------------------------------------------------------------------
stdIn:
                        push rbp
                        mov rbp, rsp

                        sub rsp, INT_MAX_LEN

                        mov rax, 0                ; read
                        mov rdi, 0                ; stdin
                        mov rsi, rsp              ; buffer address
                        mov rdx, INT_MAX_LEN      ; bufferSize
                        syscall

                        test rax, rax
                        jle .error

                        mov rsi, rsp        ; buffer
                        mov rcx, rax        ; numLen
                        xor rax, rax        ; result

                        xor r8, r8          ; numSign
                        mov rdx, 10

                        cmp byte [rsi], '-'
                        jne .nextChar
                        mov r8, 1
                        inc rsi
                        dec rcx

                        test rcx, rcx
                        jz .exit

.nextChar:
                        movzx r9, byte [rsi]

                        cmp r9b, '\n'
                        je .processSign

                        cmp r9b, '0'
                        jl .processSign
                        cmp r9b, '9'
                        jg .processSign

                        sub r9b, '0'
                        imul rax, rdx
                        add rax, r9

                        inc rsi
                        loop .nextChar


.processSign:
                        test r8, r8
                        jz .exit
                        neg rax

.exit:
                        mov rsp, rbp
                        pop rbp
                        ret

.error:
                        xor rax, rax
                        jmp .exit
