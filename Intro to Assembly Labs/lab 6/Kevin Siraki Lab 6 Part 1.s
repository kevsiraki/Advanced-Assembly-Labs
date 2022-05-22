@Kevin Siraki Lab 6 Part 1
.data
.balign 4
array:.int 13,7,22,34
format:	.asciz "Sum is: %d\n"

.text
.global main
.extern printf

@ r1=sum
@ r2=index
@ r3=address of array
@ r4=current element at index r2
@ r5=length of array

main:	push	{ip, lr}	
		mov		r1, #0					@sum
		mov 	r2, #0 					@index
		mov 	r5, #4					@array length
		ldr 	r3, =array				@array
for:	cmp 	r2, r5					@see if index r2 is at length of arrau r5
		bge 	endFor					@yes? go to endfor and print the sum
		ldr 	r4, [r3, r2, LSL #2]	@no. load the next element r2 of the array
		add 	r1, r4					@add sum by the current element r4
		add 	r2, #1					@increment the index
		b for							@loop again
endFor: ldr r0, =format					@print the format string and sum
		bl printf
		pop {ip, lr}
