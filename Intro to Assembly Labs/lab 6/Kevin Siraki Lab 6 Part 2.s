@Kevin Siraki Lab 6 Part 2
.data
.balign
cstring:.space  20,0 		@length of the string
prompt: .asciz  "Enter a sentence (less than 20 characters): "
format: .asciz  "%[^\n]s"	@format the input as a string	
output: .asciz  "Capitalized: %s\n"

.text
.global main
.extern printf
.extern scanf

@r4=cstring address.
@r2=current character.
@r1 is where we input the cstring

main:   push    {ip, lr}		@push 
        ldr     r0, =prompt		@print prompt
        bl      printf
        ldr     r0, =format     @scanf str 
        ldr     r1, =cstring	@pass r1 as scanf param
        bl      scanf
        ldr     r4, =cstring	@put address of str in r4 (first character/byte)
while:  ldrsb   r2, [r4]		@load the byte/char into r2
        cmp     r2, #0			@null character?
        beq     endwh			@yes. go to endwhile. no? continue.
        cmp     r2, #'a'       	@if the character is less than ascii 'a',skip it.
		blt     endif
        cmp     r2, #'z'        @if the character is greater than ascii 'z',skip it.
		bgt     endif
        and     r2, # 0xDF		@and the character with 11011111. for instance, a anded with 11011111 is the ascii value A.
        strb	r2, [r4]        @store the capitalized character in the current index of the string.
endif:  add     r4, #1          @go to next character of the string
        b       while			@reloop
endwh:	ldr     r0, =output 	@pass in r0 and r1 as printf parameters
        ldr     r1, =cstring
        bl      printf
        pop     {ip, pc}        @ pop return address into pc
