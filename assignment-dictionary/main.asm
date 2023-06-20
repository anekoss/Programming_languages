%include "colon.inc"
global _start

%define BUFFER_SIZE 256
%define EXIT_CODE 0
%define EXIT_CODE 1

section .bss

section .rodata
%include "words.inc"
error: db "Length of string is longer than buffer or empty", 0x0A, 0
no_found: db "Value of this key was not found", 0x0A, 0
found: db "Value: ", 0

%include "dict.inc"
%include "lib.inc"
section .text
_start:
	sub rsp, BUFFER_SIZE
	mov rdi, rsp
	mov rsi, BUFFER_SIZE
	call read_word
	test rax, rax
	jz .err
	mov rdi, peak
	mov rsi, rsp
	call find_word
	add rsp, BUFFER_SIZE
	test rax, rax
	jz .not_found
	mov rdi, rax
	push rdi
	call string_length
	pop rdi
	add rdi, rax
	inc rdi 
	call print_string
	call print_newline_err
	mov rdi, EXIT_CODE
	call exit
.not_found:
	mov rdi, message_error_key
	call print_error
	mov rdi, ERROR_EXIT_CODE
	call exit
.err:
	add rsp, BUFFER_SIZE
	mov rdi, message_error_key_len
	call print_error
	mov rdi, ERROR_EXIT_CODE
	call exit

