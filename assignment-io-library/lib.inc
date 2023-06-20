section .text
 
 
; Принимает код возврата и завершает текущий процесс
exit: 
    mov rax, 60
    ret 

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
	xor rax, rax				; используем rax как счетчик
	.char:
		cmp byte[rdi+rax], 0		; сравниваем текущий символ с нуль-терминатором
		je .end
		inc rax
		jmp .char
	.end:
		ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
	mov rsi, rdi				; адрес строки в rsi
	push rsi
	call string_length
	pop rsi
	mov rdx, rax				; длина строки в rdx		
	mov rax, 1					
	mov rdi, 1					
	syscall
	ret


; Принимает код символа и выводит его в stdout
print_char:
	push rdi
	mov rax, 1
	mov rdx, 1
	mov rdi, 1
	mov rsi, rsp				; записываем адрес вершины стека в rsi		
	syscall
	pop rdi
	ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
	mov rdi, 0xA				; записываем код символа перевода строки в rdi
	call print_char
	ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
	mov rcx, rsp				; записываем адрес вершины стека в rcx
	mov rax, rdi				; записываем число в rax
	mov r10, 10					; используем r10 как делитель
	dec rsp
	mov byte[rsp], 0
	.div:
		xor rdx, rdx
		div r10					; в rdx остаток деления на 10
		add rdx, '0' 			; добавляем к rdx '0' для получения кода в ASCII
		dec rsp
		mov byte[rsp], dl		; сохраняем цифру в стек
		test rax, rax
		jnz .div
	mov rdi, rsp				; записываем адрес первой цифры числа в rdi 
	push rcx
	call print_string
	pop rcx
	mov rsp, rcx
	ret


; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
	cmp rdi, 0
	jns .print_u
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
	.print_u:
		call print_uint
	ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
   	.loop:
		mov al, byte[rdi]
		mov dl, byte[rsi]
		cmp al, dl					; сравниваем символы двух строк
		jne .n_equals
		cmp al, 0					; проверяем не закончились ли строки
		je .equals
		inc rsi						
		inc rdi
		jmp .loop
	.n_equals:
		mov rax, 0
		ret
	.equals:
		xor rax, rax
		inc rax
		ret
		
		

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    	xor rax, rax
	mov rdx, 1
	xor rdi, rdi
	push 0
	mov rsi, rsp					 ; записываем адрес вершины стека в rsi
	syscall
	pop rax
    	ret 


; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
	push r12
	push r13
	push r14
	mov r13, 0
	mov r14, rdi					; записываем адрес начала буфера в rdi
	mov r12, rsi					; записываем размер буфера в rsi
	.whitespace:
		call read_char
		cmp al, 0x20				; сравниваем текущий символ с символом пробела
		je .whitespace
		cmp al, 0x09				; сравниваем текущий символ с символом табуляции
		je .whitespace
		cmp al, 0x0A				; сравниваем текущий символ с символом перевода строки
		je .whitespace
		cmp al, 0					; сравниваем текущий символ с нуль-терминатором
		je .success
	.read:
		dec r12
		cmp r12,0
		je .error
		mov byte[r14+r13], al
		inc r13						; увеличиваем счетчик длины строки
		call read_char
		cmp al, 0x20					; сравниваем текущий символ с символом пробела
		je .success
		cmp al, 0x09					; сравниваем текущий символ с табуляции
		je .success
		cmp al, 0x0A					; сравниваем текущий символ с символом перевода строки
		je .success
		cmp al, 0						; сравниваем текущий символ с нуль-терминатором
		je .success
		jmp .read
	.success:
		mov byte [r14+r13], 0			; дописываем нуль-терминатор к строке
		mov rax, r14					; записываем адрес буфера в rax
		mov rdx, r13					; записываем длину строки в rdx
		jmp .end
	.error:
		xor rax, rax
		xor rdx, rdx
		jmp .end
	.end:
		pop r14
		pop r13
		pop r12
		ret

		


; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
	xor rax, rax 
	xor rcx, rcx 				; используем rcx для посчета длины числа в символах
	mov rsi, 10 				; используем rsi как делитель
	.loop:
		xor r10, r10 
		mov r10b, byte [rdi]
		cmp r10b, '0'
		jb .end
		cmp r10b, '9'
		ja .end
		inc rcx					; увеличиваем счетчик длины
		inc rdi 
		sub r10b, '0' 			; вычитаем ноль, чтобы получить цифру
		mul rsi 				; умножаем rsi на 10
		add rax, r10 
		jmp .loop

	.end:
		mov rdx, rcx 			; записываем длину строки в rdx
		ret



; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
	cmp byte[rdi], 0x2d				; сравниваем символ с символом пробела
	je .n_int
	jne .uint
	.n_int:
		inc rdi
		call parse_uint 
		inc rdx						; увеличиваем счетчик длины
		neg rax						; меняем знак числа на противоположный 
		ret
	.uint:
		call parse_uint
		ret
	

	

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    	push rdx
	push rdi
	push rsi
	call string_length
	pop rsi
	pop rdi
	pop rdx
	cmp rdx, rax					; сравниваем длину строки с длинной буфера
	jb .null
	.loop:
		mov r10, 0		
		mov r10b, byte[rdi]			; копируем символ из строки в буфер
		mov byte[rsi], r10b			
		cmp r10, 0				; проверяем, не закончилась ли строка
		je .end
		inc rsi						
		inc rdi
		jmp .loop
	.null:
		mov rax, 0
	.end:
		ret
  
