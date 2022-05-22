@Kevin Siraki Lab 5 RECURSIVE VERSION
.data
.balign 4
prompt:	.asciz "Enter number: "
format:	.asciz "%d"
output: .asciz "The factorial is: %d\n"

.text
.global main
.extern printf
.extern scanf

fact:	str lr, [sp,#-4]!	@push link register
	str r4, [sp,#-4]!	@push r4
    	mov r4, r0         	@move a copy of r0 into r4
    	cmp r0, #0         	@compare r0 to 0 
    	bne else    		@are they equal? go to else.
    	mov r0, #1         	@no, return a 1
    	b end
else:	sub r0, r0, #1     	@r0=r0-1
    	bl fact			@fact(n-1)           
    	mov r1, r4         	@now, put initial value of r0 (which is in r4) into r1
    	mul r0, r1, r0     	@r0=r0*r1
end:	ldr r4, [sp], #+4	@pop top of stack, put in r4
    	ldr lr, [sp], #+4  	@pop top of stack, put in link register
    	bx lr			@return

.global main
main:	str lr, [sp,#-4]!	@push link register
    	sub sp, sp, #4		@push 4 bytes (int in ARM is 4 bytes, used for number entered thru scanf)
    	ldr r0, addprompt	@print prompt
    	bl printf                    
    	ldr r0, addformat	@format scanf
    	mov r1, sp		@get input                                    
    	bl scanf                     
    	ldr r0, [sp]		@put int read by scanf into r0, making it the parameter of fact             
    	bl fact			@call fact
    	mov r1, r0		@get the retVal of factorial and move it to r1 */                	 
    	ldr r0, addoutput	@print output  
    	bl printf                    
    	add sp, sp, #+4		@pop scanf entered value
    	ldr lr, [sp], #+4       @pop top of stack, put in link register
    	bx lr                   @return 0

addprompt:	.word prompt
addoutput:	.word output
addformat:	.word format
