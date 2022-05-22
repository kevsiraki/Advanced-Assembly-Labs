.data
.balign 4
array:.int 13,7,22,34
format:	.asciz "Sum: %d\n"

.text
.global main
.extern printf

main:	push	{ip, lr}	
		mov		r1, #0					
		mov 	r2, #0 					
		mov 	r5, #4					
		ldr 	r3, =array				
for:	cmp 	r2, r5					
		bge 	endFor					
		ldr 	r4, [r3, r2, LSL #2]	
		add 	r1, r4					
		add 	r2, #1					
		b for							
endFor: ldr r0, =format					
		bl printf
		pop {ip, lr}
