@Kevin Siraki Lab 5 Non-Recursive
@ data section
.data
.balign 4
prompt:	.asciz	"Enter number: "
format:	.asciz	"%d"
num:	.int	0
val:	.int	0
output:	.asciz	"The factorial is: %d\n"

@ code section

.text
.global main
.extern printf
.extern scanf
@	factorial
@	r0 is parameter n
@	r1 is return value
fact:	push	{ip, lr}	@ save return
	mov	r4, r0		@ move a r0 into r4 
	mov	r5, #1		@ move a 1 into r5 (i = 1)
for:	cmp	r5, r0		@ is r5 equal to the parameter r0? go to endFor.
	beq	endFor
	mul	r4, r5, r4	@ no? multiply r5 and r4, store result in r4
	add	r5, #1		@ r5 + 1 (i++)
	b	for		@loop again
endFor:	mov	r1, r4		@ return r4
	pop	{ip, pc}	@ return

.global main
main:	push	{ip, lr}	@ push return address + dummy register
				@ for alignment
	ldr	r0, =prompt	@ print the prompt
	bl	printf
	ldr	r0, =format	@ call scanf, and pass address of format
	ldr	r1, =num	@ string and address of num in r0, and r1,
	ldr	r2, =val
	bl	scanf		@ respectively.
	ldr	r0, =num	@ get num
	ldr	r0, [r0]
	bl	fact		@ call fact
	ldr	r0, =output
	bl	printf
	pop	{ip, pc}	@ pop return address into pc
