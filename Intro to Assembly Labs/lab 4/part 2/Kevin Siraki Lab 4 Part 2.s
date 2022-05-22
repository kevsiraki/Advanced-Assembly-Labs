@Kevin Siraki Lab 4 Part 2

.data
.balign 4	
prompt: .asciz "Enter a number: "
format: .asciz "%d"
a:	.int	0
b:	.int	0
output: .asciz "Product is: %d\n" 

.text
.global main
.extern printf
.extern scanf

main:	
	push {ip, lr}	@ push return address + dummy register
					@ for alignment
						
	ldr	r0, =prompt	@prompt user input
	bl	printf
	
	ldr	r0, =format	@call scanf,pass &format, pass &a in r0 and r1
	ldr	r1, =a
	bl 	scanf

	ldr	r0,=prompt	@prompt user input
	bl	printf

	ldr	r0,=format	@call scanf, pass &format, pass &b in r0 and r1
	ldr	r1,=b
	bl 	scanf
	
	ldr	r1,=a		@pass &a
	ldr	r1,[r1]
	ldr	r2,=b		@pass &b
	ldr	r2,[r2]
	mul	r3, r2, r1	@multiply r2 and r1, store result in r3

	ldr	r0, =a		@output the product
	str	r3,[r0]
	ldr r1, =a
	ldr	r1,[r1]
	ldr	r0, =output
	bl	printf

    pop {ip, pc}	@ pop return address into pc
