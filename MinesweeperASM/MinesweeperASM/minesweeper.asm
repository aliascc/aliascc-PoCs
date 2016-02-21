
.model flat, c

printf				PROTO arg1:Ptr BYTE, printlist:VARARG
scanf				PROTO arg2:Ptr BYTE, inputlist:VARARG
system				PROTO arg1:Ptr BYTE
helper_random		PROTO arg1:DWORD

.data

B_BLANK				equ	1
B_MINE				equ	2
B_NUMBER			equ	4
B_HIDDEN			equ 8
B_ALL				equ 15

B_INVALIDPOS		equ	0
B_GAME_OVER			equ 1
B_GAME_WIN			equ 2
B_OK				equ 3

;Game Table 
rowSize				equ	10
colSize				equ	10
maxMines			equ 10
tableSize			equ	(rowSize * colSize)
gameTableType		DD	(tableSize) DUP(B_HIDDEN) 
gameTableNumbers	DD	(tableSize) DUP(0) 
gameDisplayedLocs	DD	0

;Strings for Menus
strScanfInt			DB	"%d", 0
strEndLine			DB	10, 0
strChooseX			DB	"Choose X: ", 0
strChooseY			DB	"Choose Y: ", 0
strWinMsg			DB	"You've won the Game!", 0
strLostMsg			DB	"You've lost the game!", 0

;String for Game Table
strHidden			DB	"-", 0
strBlank			DB	" ", 0
strMine				DB	"*", 0
strNumber			DB	"%d", 0
strNumberBlank		DB	"%d ", 0
strNumber2Blank		DB	"%d  ", 0
strNumberTableSepFL	DB	"%d |", 0
strTableSep			DB	" | ", 0
strTableSepFL		DB	" |", 0
strTableBlank		DB	" ", 0
strTable2Blank		DB	"  ", 0

;Other Strings
strClearScreen		DB	"cls", 0

;Input to read
intToRead			DD	0

.code

; Clears the Screen
; no params
; no return
clear_screen PROC

	push OFFSET strClearScreen
	call system
	add esp, 4 ;  Stack cleaning

	ret
	
clear_screen ENDP

; Read and integer from a user
; no params
; return <eax>: integer that was read
read_int PROC

	push OFFSET intToRead
	push OFFSET strScanfInt
	call scanf
	add esp, 8 ;  Stack cleaning

	mov eax, intToRead

	ret

read_int ENDP

; Print String
; param <ecx>: String to print
; no return
print_str PROC

	push ecx
	call printf
	add esp, 4 ;  Stack cleaning

	ret
	
print_str ENDP

; Print end line
; no return
print_end_line PROC

	push OFFSET strEndLine
	call printf
	add esp, 4 ;  Stack cleaning

	ret
	
print_end_line ENDP

; Print String with a number
; param <edx>: Integer to print
; param <ecx>: String to print
; no return
print_str_num PROC

	push edx
	push ecx
	call printf
	add esp, 8 ; Stack Cleaning
	
	ret

print_str_num ENDP

; Get Location from X & Y
; param <ecx>: X Position
; param <edx>: Y Position
; return <eax>: location in table
get_location PROC

	;Save Y value
	push edx

	;Get offset for row
	mov eax, colSize
	mul ecx

	pop edx

	add eax, edx

	ret

get_location ENDP


; Traverse the table when selecting a node
; param <eax>: Table Position
; param <ecx>: X Position
; param <edx>: Y Position
; no returns
traverse_table PROC

	;if (ebx & B_MINE || ebx & B_HIDDEN)
	mov ebx, [gameTableType + (eax*4)]
	and ebx, B_MINE
	cmp ebx, B_MINE
	je traverse_table_is_mine
	mov ebx, [gameTableType + (eax*4)]
	and ebx, B_HIDDEN
	cmp ebx, B_HIDDEN
	je traverse_table_not_mine
	traverse_table_is_mine:
		ret

	;End if
	traverse_table_not_mine:

	;Remove Hidden
	xor [gameTableType + (eax*4)], B_HIDDEN

	;Increase Displayed Locations
	inc gameDisplayedLocs

	;if(ebx != B_BLANK)
	mov ebx, [gameTableType + (eax*4)]
	and ebx, B_BLANK
	cmp ebx, B_BLANK
	je traverse_table_is_blank

		ret

	;End if
	traverse_table_is_blank:

	;Save Table Position
	push eax

	;Save Y
	push edx

	;Save X
	push ecx

	;Look in top left
	;row -1, col -1
	;if (row > 0 && col > 0)
	cmp ecx, 0
	jle set_number_for_location_top_left_done
	cmp edx, 0
	jle set_number_for_location_top_left_done

		sub ecx, 1
		sub edx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_top_left_done:

	;Look in top center
	;row -1
	;if (row > 0)
	cmp ecx, 0
	jle set_number_for_location_top_center_done

		sub ecx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_top_center_done:

	;Look in top right
	;if (row > 0 && col < (colSize - 1))
	;row - 1, col +1
	cmp ecx, 0
	jle set_number_for_location_top_right_done
	mov ebx, colSize
	sub ebx, 1
	cmp edx, ebx
	jge set_number_for_location_top_right_done

		sub ecx, 1
		add edx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_top_right_done:

	;Look in left
	;if (col > 0)
	;col - 1
	cmp edx, 0
	jle set_number_for_location_left_done
	
		sub edx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_left_done:

	;Look in right
	;if (col < (colSize - 1))
	;col + 1
	mov ebx, colSize
	sub ebx, 1
	cmp edx, ebx
	jge set_number_for_location_right_done

		add edx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_right_done:

	;Look in botton left
	;if (row < (rowSize - 1) && col > 0)
	;row + 1, col -1
	mov ebx, rowSize
	sub ebx, 1
	cmp ecx, ebx
	jge set_number_for_location_bottom_left_done
	cmp edx, 0
	jle set_number_for_location_bottom_left_done

		add ecx, 1
		sub edx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_bottom_left_done:

	;Look in botton center
	;if (row < (rowSize - 1))
	;row + 1
	mov ebx, rowSize
	sub ebx, 1
	cmp ecx, ebx
	jge set_number_for_location_bottom_center_done

		add ecx, 1

		call get_location

		call traverse_table

		;Restore X & Y and Position
		mov ecx, [esp] ;X
		mov edx, [esp + 4] ;Y
		mov eax, [esp + 8] ;Table Position

	set_number_for_location_bottom_center_done:

	;Look in botton right
	;if (row < (rowSize - 1) && col < (colSize - 1))
	;row + 1, col +1
	mov ebx, rowSize
	sub ebx, 1
	cmp ecx, ebx
	jge set_number_for_location_bottom_right_done
	mov ebx, colSize
	sub ebx, 1
	cmp edx, ebx
	jge set_number_for_location_bottom_right_done

		add ecx, 1
		add edx, 1

		call get_location

		call traverse_table

	set_number_for_location_bottom_right_done:

	;pop X & Y
	pop ecx
	pop edx

	;Restore Table location
	pop eax

	ret

traverse_table ENDP

; Select Position
; param <ecx>: X Position
; param <edx>: Y Position
; return <eax>: return option
select_pos PROC

	;Verify correct locations
	;if (ecx >= rowSize || edx >= colSize)
	cmp ecx, rowSize
	jge select_pos_cont_wrong_x
	cmp edx, colSize
	jle select_pos_cont_correct_y

		select_pos_cont_wrong_x:
		;Incorrect Size
		mov eax, B_INVALIDPOS
		
		ret

	select_pos_cont_correct_y:

	;Get table location
	call get_location
	
	;If is mine
	mov ebx, [gameTableType + (eax*4)]
	and ebx, B_MINE
	cmp ebx, B_MINE
	jne select_pos_end_is_mine
	
		xor [gameTableType + (eax*4)], B_HIDDEN
		mov eax, B_GAME_OVER

		ret

	;End if
	select_pos_end_is_mine:

	;Traverse the Table
	call traverse_table

	mov ebx, tableSize
	sub ebx, maxMines
	cmp ebx, gameDisplayedLocs
	jne select_pos_end_is_game_won

		;Return Game Win
		mov eax, B_GAME_WIN
		ret

	select_pos_end_is_game_won:

	;Return Ok
	mov eax, B_OK

	ret

select_pos ENDP

; Set Mines
; No params
; no return
set_mines PROC

	;Set Number of mines to set
	mov ecx, maxMines

	;Loop 1
	set_mines_while:

		;Save loop counter
		push ecx

		;Get Random T
		push colSize
		call helper_random
		pop ecx
		push eax

		;Get Random X
		push rowSize
		call helper_random
		pop ecx
		mov ecx, eax

		;Retrieve Y
		pop edx

		;Get table location
		call get_location

		;Check if Table Location has mine
		mov edx, [gameTableType + (eax*4)]

		;Retrieve loop counter
		pop ecx

		;if (edx & B_MINE)
		and edx, B_MINE
		cmp edx, B_MINE
		jne set_mines_else_is_mine

			;Mine has been set, continue looping
			inc ecx
			jmp set_mines_end_if_is_mine

		set_mines_else_is_mine:

			;Set Mine to location
			or [gameTableType + (eax*4)], B_MINE

		;End if
		set_mines_end_if_is_mine:

	loop set_mines_while

	ret

set_mines ENDP

; Checks the table location for a mine
; if one is found increases counter
; restores X & Y to original value, they must be in the stack X on top, Y next
; param <eax>: location in the table
; param <ebx>: counter to increases
; no returns
check_if_mine_inc_counter_restore_pos PROC

	mov edx, [gameTableType + (eax * 4)]
	and edx, B_MINE
	cmp edx, B_MINE	
	jne set_number_for_location_top_left_no_mine

		inc ebx

	set_number_for_location_top_left_no_mine:

	;Restore X & Y
	mov ecx, [esp + 4] ;X
	mov edx, [esp + 8] ;Y

	ret

check_if_mine_inc_counter_restore_pos ENDP

; Sets the number of mines around a locations in the table
; param <eax>: location in the table
; param <ecx>: X Position
; param <edx>: Y Position
; no returns
set_number_for_location PROC

	;Save Table Position
	push eax

	;Save Y
	push edx

	;Save X
	push ecx

	;Number counter
	mov ebx, 0

	;Look in top left
	;row -1, col -1
	;if (row > 0 && col > 0)
	cmp ecx, 0
	jle set_number_for_location_top_left_done
	cmp edx, 0
	jle set_number_for_location_top_left_done

		sub ecx, 1
		sub edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_top_left_done:

	;Look in top center
	;row -1
	;if (row > 0)
	cmp ecx, 0
	jle set_number_for_location_top_center_done

		sub ecx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_top_center_done:

	;Look in top right
	;if (row > 0 && col < (colSize - 1))
	;row - 1, col +1
	cmp ecx, 0
	jle set_number_for_location_top_right_done
	mov eax, colSize
	sub eax, 1
	cmp edx, eax
	jge set_number_for_location_top_right_done

		sub ecx, 1
		add edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_top_right_done:

	;Look in left
	;if (col > 0)
	;col - 1
	cmp edx, 0
	jle set_number_for_location_left_done

		sub edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_left_done:

	;Look in right
	;if (col < (colSize - 1))
	;col + 1
	mov eax, colSize
	sub eax, 1
	cmp edx, eax
	jge set_number_for_location_right_done

		add edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_right_done:

	;Look in botton left
	;if (row < (rowSize - 1) && col > 0)
	;row + 1, col -1
	mov eax, rowSize
	sub eax, 1
	cmp ecx, eax
	jge set_number_for_location_bottom_left_done
	cmp edx, 0
	jle set_number_for_location_bottom_left_done

		add ecx, 1
		sub edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_bottom_left_done:

	;Look in botton center
	;if (row < (rowSize - 1))
	;row + 1
	mov eax, rowSize
	sub eax, 1
	cmp ecx, eax
	jge set_number_for_location_bottom_center_done

		add ecx, 1
		
		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_bottom_center_done:

	;Look in botton right
	;if (row < (rowSize - 1) && col < (colSize - 1))
	;row + 1, col +1
	mov eax, rowSize
	sub eax, 1
	cmp ecx, eax
	jge set_number_for_location_bottom_right_done
	mov eax, colSize
	sub eax, 1
	cmp edx, eax
	jge set_number_for_location_bottom_right_done

		add ecx, 1
		add edx, 1

		;Get Table location
		call get_location

		call check_if_mine_inc_counter_restore_pos

	set_number_for_location_bottom_right_done:

	;pop X & Y
	pop ecx
	pop edx

	;Restore Table location
	pop eax

	; if (ebx != 0)
	cmp ebx, 0
	je set_number_for_location_else_if_num_zero

		or [gameTableType + (eax*4)], B_NUMBER
		mov [gameTableNumbers + (eax*4)], ebx
		jmp set_number_for_location_end_if_num_zero

	;else
	set_number_for_location_else_if_num_zero:

		or [gameTableType + (eax*4)], B_BLANK

	;End if
	set_number_for_location_end_if_num_zero:

	ret

set_number_for_location ENDP

; Sets the number of mines around all the locations of the table
; no params
; no returns
set_numbers PROC

	;Initialize table location
	mov eax, tableSize - 1

	;Initialize counter for Loop 1
	mov ecx, rowSize

	;Loop 1
	set_numbers_for1:

		;Save Loop 1 counter
		push ecx

		;Get a copy of the current counter
		mov ebx, ecx

		;Initialize counter for Loop 2
		mov ecx, colSize

		;Loop 2
		set_numbers_for2:

			;Save registers
			push ebx
			push ecx
			push eax

			;Get which Table String to print
			mov edx, [gameTableType + (eax*4)]

			;if(edx & B_MINE)
			and edx, B_MINE
			cmp edx, B_MINE
			je set_numbers_end_if_not_mine

				;Set Y
				mov edx, ecx
				sub edx, 1

				;Set X
				mov ecx, ebx
				sub ecx, 1

				;Set the numberr for the location
				call set_number_for_location

			;end if
			set_numbers_end_if_not_mine:

			;Restore Registers
			pop eax
			pop ecx
			pop ebx

			;Increase table location
			dec eax

		;End Loop 2
		loop set_numbers_for2

		;Restore Loop 1 counter
		pop ecx

	;End Loop 1
	loop set_numbers_for1

	ret 

set_numbers ENDP

; Prints the character for the Location of the table
; param <eax>: Table location
; no return
print_character_table_position PROC

	;Get which Table String to print
	mov edx, [gameTableType + (eax*4)]

	;switch edx
	;;;;;;;;;;;;;;;

	;case B_BLANK:
	mov ebx, edx
	and ebx, B_BLANK
	cmp ebx, B_BLANK
	jne p_case_mine

		;Print Blank
		mov ecx, OFFSET strBlank
		call print_str
		jmp p_end_switch

	;case B_MINE:
	p_case_mine:
	mov ebx, edx
	and ebx, B_MINE
	cmp ebx, B_MINE
	jne p_case_number

		;Print Blank
		mov ecx, OFFSET strMine
		call print_str
		jmp p_end_switch

	;case B_NUMBER:
	p_case_number:

		;Print Blank
		mov edx, [gameTableNumbers + (eax*4)]
		mov ecx, OFFSET strNumber
		call print_str_num

	p_end_switch:

	ret

print_character_table_position ENDP

print_first_line PROC

	push ecx
	push eax
	push ebx
	push edx

	;Print Blank 
	mov ecx, OFFSET strTable2Blank
	call print_str

	mov ecx, rowSize
	mov eax, 0

	print_first_line_loop:

		;Save values
		push eax
		push ecx

		;Print Blank Space
		mov ecx, OFFSET strBlank
		call print_str

		;Restore counter
		mov eax, [esp + 4]

		;Print Number
		mov ecx, OFFSET strNumberTableSepFL
		mov edx, eax
		call print_str_num

		;Restore values
		pop ecx
		pop eax

		;Increase counter
		inc eax

	loop print_first_line_loop

	;Print end line
	call print_end_line

	pop edx
	pop ebx
	pop eax
	pop ecx

	ret

print_first_line ENDP

; Print Game Table
; param <ebx>: if not zero it will ignore the hidden value
; no return
print_table PROC

	;Initialize table location
	mov eax, 0

	;Initialize counter for Loop 1
	mov ecx, rowSize

	;Loop 1
	for1:

		;Save Loop 1 counter
		push ecx

		;copy loop 1 counter
		mov edx, ecx

		;Initialize counter for Loop 2
		mov ecx, colSize

		;Loop 2
		for2:

			;if (x == rowSize && y == colSize)
			cmp edx, rowSize
			jne print_table_first_line_end
			cmp ecx, colSize
			jne print_table_first_line_end

				call print_first_line

			print_table_first_line_end:

			;if (y == 0)
			cmp ecx, colSize
			jne print_table_first_blank_space_end

				push ecx
				push eax
				push ebx

				;Print Number
				mov eax, edx
				mov edx, rowSize
				sub edx, eax
				mov ecx, OFFSET strNumber2Blank
				call print_str_num

				pop ebx
				pop eax
				pop ecx

			print_table_first_blank_space_end:

			;Save registers
			push ebx
			push ecx
			push eax

			;Get which Table String to print
			mov edx, [gameTableType + (eax*4)]

			;if(ebx != 0 && edx & B_HIDDEN)
			cmp ebx, 0
			jne else_hidden
			and edx, B_HIDDEN
			cmp edx, B_HIDDEN
			jne else_hidden

				;Print Blank
				mov ecx, OFFSET strHidden
				call print_str

				jmp end_if_hidden

			;else
			else_hidden:

				call print_character_table_position

			;end if
			end_if_hidden:

			;Print Separator
			mov ecx, OFFSET strTableSep
			call print_str

			;Restore Registers
			pop eax
			pop ecx
			pop ebx

			;Increase table location
			inc eax

		;End Loop 2
		loop for2

		;Print end line
		push eax
		push ebx
		call print_end_line
		pop ebx
		pop eax

		;Restore Loop 1 counter
		pop ecx

	;End Loop 1
	loop for1

	ret

print_table ENDP

; Play Game
play_asm PROC

	;Init Game Table
	call set_mines
	call set_numbers

	;Init ecx loop counter
	mov ecx, 1
	
	while1:

		;Save loop counter
		push ecx

		;Print Table
		mov ebx, 0
		call print_table

		;Get X
		mov ecx, OFFSET strChooseX
		call print_str
		call read_int
		push eax

		;Get Y
		mov ecx, OFFSET strChooseY
		call print_str
		call read_int
		mov edx, eax

		;Retrieve X
		pop ecx

		;Select Position in the table
		call select_pos

		;Save register
		push eax

		;clear the screen
		call clear_screen

		;Retrieve register
		pop eax

		;Retrieve loop counter
		pop ecx

		;if (eax != B_GAME_OVER && eax != B_GAME_WIN)
		cmp eax, B_GAME_OVER
		je play_asm_else_game_end
		cmp eax, B_GAME_WIN
		je play_asm_else_game_end

			;Increase to make loop infinite
			inc ecx
			jmp play_asm_end_if_game_end

		;else
		play_asm_else_game_end:
			;End loop
			mov ecx, 1

		;End if
		play_asm_end_if_game_end:

	loop while1

	;if(eax == B_GAME_WIN)
	cmp eax, B_GAME_WIN
	jne play_asm_else_game_over_msg

		;Print Win Message
		mov ecx, OFFSET strWinMsg
		call print_str

		jmp play_asm_end_if_game_over_msg

	;else
	play_asm_else_game_over_msg:

		;Print lost Message
		mov ecx, OFFSET strLostMsg
		call print_str

	;End if
	play_asm_end_if_game_over_msg:

	;End line
	call print_end_line

	;Print Table
	mov ebx, 1
	call print_table

	;End line
	call print_end_line

	ret

play_asm ENDP

END
