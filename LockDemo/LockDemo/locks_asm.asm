
.model flat, c

.code

test_and_set_cmpxchg_asm PROC

	;will compare with eax
	mov eax, 0
	mov edx, [esp + 4]
	mov ecx, 1

	lock cmpxchg DWORD PTR [edx], ecx

	;old value is set in eax
	ret

test_and_set_cmpxchg_asm ENDP


test_and_set_btr_asm PROC

	mov edx, [esp + 4]

	lock btr DWORD PTR [edx], 0
	jnc not_grab

	mov eax, 0
	ret

not_grab:
	mov eax, 1
	ret

test_and_set_btr_asm ENDP

; Parameters
; 1- uint32_t* pri_sembit_lock
; 2- uint32_t* sem_count
semaphore_lock_asm PROC

	mov eax, 0

	mov ebx, [esp + 4]
	mov ecx, [esp + 8]

spin:
	lock btr DWORD PTR [ebx], 0
	jnc spin

	cmp DWORD PTR [ecx], 0
	je end_lock

	dec DWORD PTR [ecx]
	mov eax, 1

end_lock:
	bts DWORD PTR [edx], 0

	;return eax
	ret

semaphore_lock_asm ENDP

; Parameters
; 1- uint32_t* pri_sembit_lock
; 2- uint32_t* sem_count
; 3- uint32_t max_sem_count
semaphore_unlock_asm PROC

	mov eax, 0

	mov ebx, [esp + 4]
	mov ecx, [esp + 8]
	mov edx, [esp + 12]

spin:
	lock btr DWORD PTR [ebx], 0
	jnc spin

	cmp DWORD PTR [ecx], edx
	je end_lock

	inc DWORD PTR [ecx]

end_lock:
	bts DWORD PTR [edx], 0

	ret

semaphore_unlock_asm ENDP

END
