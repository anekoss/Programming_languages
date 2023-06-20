global find_word
%include "lib.inc"
section .text
find_word:
    test rsi, rsi 
	jz .fail 
    push rsi
    push rdi
    add rsi, 8 
    call string_equals
    pop rdi
    pop rsi
    test rax, rax
    jnz .success
    mov rsi, [rsi] ; 
	jmp find_word
.success:
    mov rax, rsi
	ret
.fail:
    xor rax, rax
    ret
